/** Seehwan yoo

	Copy right is with seehwan.yoo@dankook.ac.kr

	Do not modifiy this comment. The source code comes with AS-IS.
	I allow re-distribution of this source code only under the education purpose
*/

/** Subin yoo

	I modified the original source code.
	Copy right is with myjihun1015@naver.com
*/ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
//#include "stored_filename.h"
#include "node.h"
#include "edit.h"

/* simple line editor */
/* when it begins, starts with command mode
	At command mode, we can move cursor.
	initial cursor position is 1,1

	cursor moves left, up, down, right by 'h','j','k','l' key input
*/

/* messages to display the current state*/ 
static char* initial_message = "                      \n";
static char* save_file_message = "[The file is saved] \n";
static char* input_message = "[Input mode]        \n";
static char* loaded_message = "[The file is loaded]\n";
static void ClearLineFromReadBuffer(void)
{
	while(getchar()!='\n');
}
enum MODES {
	COMMAND_MODE,
	INPUT_MODE
};
  
int main(int argc, char * argv[])
{
	List* list = (List*)malloc(sizeof(list));
	list->head = NULL;
	list->tail = NULL;

	char buff[255];
	enum MODES mode = COMMAND_MODE;

	int cur_line = 1;
	int cur_col = 1;
	const int min_line = 1;
	const int min_col = 1;
	const int max_line = 20;
	const int max_col = 79;
	int key, ret;
	char data[80][20];

	struct termios oldt, curt, newt; //structure to store the information regarding option for terminal 
	
	 /* Some initialization */
 	fputs("\033[2J", stdout); //clear screen
    fputs("\033[1;1H", stdout); //display the current cursor position in screen 

	/* Change the current properties of terminal 
		to the terminal with properties of ICANON OFF and ECHO OFF */
	tcgetattr (STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr ( STDIN_FILENO, TCSANOW, &newt);

	/*now,we get input*/
	while (1) {

		key = getchar(); //get a character from user
		ret = feof(stdin);
		if (ret != 0) {
			printf ("EOF\n");
			return 0;
		}
		ret = ferror(stdin);
		if (ret != 0) {
			printf ("ERROR getchar() by %d\n", ret);
			return 0;
		}
 
	if (mode == COMMAND_MODE) {
		switch (key) {
		case 'i':
			mode = INPUT_MODE; //change the mode from COMMAND to INPUT     
			tcgetattr ( STDIN_FILENO, &curt );
			newt = curt;
			newt.c_lflag &= ~( ECHO ); //ECHO OFF
			tcsetattr ( STDIN_FILENO, TCSANOW, &newt );
			sprintf(buff, "\033[%d;%dH%s",22,1,input_message);
			fputs(buff,stdout); //print input message at (22,1)
			break;
		/* make movement with the following keys */
		case 'h': /* If the user presses 'h', then move cursor to the left */ 
			cur_col--;
			if (cur_col < min_col) cur_col = min_col;
			break;
		case 'k': /* If the user presses 'k', then move cursor to the down */
			cur_line++;
			if (cur_line > max_line) cur_line = max_line;
			break; 
		case 'j': /* If the user presses 'j', then move cursor to the up */
			cur_line--;
			if (cur_line < min_line) cur_line = min_line;
			break;
		case 'l': /* If the user presses 'l', then move cursor to the right */
			cur_col++;
			if (cur_col > max_col) {
				cur_col = 1;
				cur_line++;
				if (cur_line > max_line) cur_line = max_line;
			}
			break;
		case 'w' :
			/* Store in the file : data in all nodes is stored in the file */
			sprintf(buff, "\033[%d;%dH%s",22,1,save_file_message);     
			fputs(buff, stdout); //print save message at (22,1)
			Saved_list(list); //Store data in all nodes into file 
			break;
		case 'o' : 
   	   	    /* Load file : Delete all nodes,
					      then store contents of file into nodes again */
 			Delete_all_node(list); //Delete all nodes connected in linked list
			fputs("\033[2J", stdout);//clear screen
	        fputs("\033[1;1H", stdout); //display the current cursor position in screen
      		tcgetattr ( STDIN_FILENO, &curt );
      		newt = curt;
      		newt.c_lflag &= ~( ICANON | ECHO ); //ECHO off, ICANON off
     		tcsetattr ( STDIN_FILENO, TCSANOW, &newt );
     	
        	FILE* fp = fopen("test.txt","rt"); //Create the stream with 'test.txt' file using read mode
         	int ch;
         	while((ch = fgetc(fp))!=EOF)
            {
					DataInsertion(list,(char)ch);
					fputc(ch,stdout);
			}//Read all characters in the file, then Connect them with linked list again
       		fclose(fp); //free the stream with 'test.txt' file
     		sprintf(buff, "\033[%d;%dH%s",22,1,loaded_message);
       		fputs(buff, stdout); //print loaded message at (22,1)
			
			break;			
			
		}	// end switch

	
// end COMMAND_MODE
	} //if
	else if (mode == INPUT_MODE)
	{
		switch (key) 
		{
			case 27: /* escape key code : change the mode from INPUT to COMMAND*/
				mode = COMMAND_MODE;
				tcgetattr ( STDIN_FILENO, &curt );
				newt = curt;
				newt.c_lflag &= ~( ECHO );//ECHO OFF
				tcsetattr ( STDIN_FILENO, TCSANOW, &newt );
	 			sprintf(buff, "\033[%d;%dH%s",22,1,initial_message);//Clear the message at (22,1)
           		fputs(buff, stdout);
				break;
			case 10: /* Enter key code : If user presses 'enter'key,
									move to the next line */ 
				cur_line++;
				cur_col=1;
				DataInsertion(list,key);
				Print_key(list);
				if(cur_line > max_line) cur_line = max_line;
				break;
			case 9: /* Delete key code(tab) : If user presses 'tab'key,
											one character is deleted. */
 		        fputs(" ",stdout);
				cur_col--; 
			
				if (cur_col<min_col)
				{
					if(list->tail->data==10)//In case that data of the last node is 'enter' key
					{
						cur_col = min_col;
						cur_line--;
						if(cur_line < min_line) cur_line = min_line;
					}
					else //In case that data of the last node is not 'enter'key 
					{
						cur_line--;
						cur_col=80;
					}
				}
				delete_key(list); //delete one character
			break;
			default:
				mode = INPUT_MODE;
				//calibrate cursor
				cur_col++;
				//save contents
				data[cur_col-1][cur_line-1] = key; //store the key into data array
				DataInsertion(list,key); //store the key into linked list
				Print_key(list);			
				if (cur_col > max_col) {
					cur_col = 1;
					cur_line++;
					if (cur_line > max_line) cur_line = max_line;
				}//If exceed the designated column size, move the cursor position into the next line	
			break;
		}	// end switch
	// end INPUT_MODE
	}
	
	sprintf(buff, "\033[%d;%dH%3d:%3d", 21,70,cur_line,cur_col);
	fputs(buff, stdout); //display the current cursor position at point (21,70)
	sprintf(buff, "\033[%d;%dH", cur_line, cur_col);
	fputs(buff, stdout); //print the current cursor position


	}	// end while

 
	/* return termio back */ 
	tcsetattr ( STDIN_FILENO, TCSANOW, &oldt );

	return 0;
}

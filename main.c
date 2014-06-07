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
	initial cursor position is 0,0

	cursor moves left, up, down, right by 'h','j','k','l' key input
*/
static char* initial_message = "                      \n";
static char* save_file_message = "[The file is saved]\n";
static char* input_message = "[Input mode]\n";
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
	const int max_col = 80;
	int key, ret;
	char data[80][20];

	struct termios oldt, curt, newt;
	
	 /* Some initialization */
 	fputs("\033[2J", stdout);
    fputs("\033[1;1H", stdout);

	tcgetattr (STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr ( STDIN_FILENO, TCSANOW, &newt);

	/*now,we get input*/
	while (1) {

		key = getchar();
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
			mode = INPUT_MODE;     
			tcgetattr ( STDIN_FILENO, &curt );
			newt = curt;
			newt.c_lflag &= ~( ECHO );
			tcsetattr ( STDIN_FILENO, TCSANOW, &newt );
			sprintf(buff, "\033[%d;%dH%s",22,1,input_message);
			fputs(buff,stdout);
			break;
		/* make movement with the following keys */
		case 'h':  
			cur_col--;
			if (cur_col < min_col) cur_col = min_col;
			break;
		case 'j':
			cur_line++;
			if (cur_line > max_line) cur_line = max_line;
			break; 
		case 'k':
			cur_line--;
			if (cur_line < min_line) cur_line = min_line;
			break;
		case 'l':
			cur_col++;
			if (cur_col > max_col) {
				cur_col = 1;
				cur_line++;
				if (cur_line > max_line) cur_line = max_line;
			}
			break;
		case 'w' :
			sprintf(buff, "\033[%d;%dH%s",22,1,save_file_message);     
			fputs(buff, stdout);
//			ClearLineFromReadBuffer();
			Saved_list(list);
			break;
		case 'o' : 
   	   	    /*Load file : Delete all nodes,
					      then store contents of file into nodes again*/
			Delete_all_node(list);
			//list->head ==NULL;
			//list->tail ==NULL;
			fputs("\033[2J", stdout);
	        fputs("\033[1;1H", stdout);
      		tcgetattr ( STDIN_FILENO, &curt );
      		newt = curt;
      		newt.c_lflag &= ~( ICANON | ECHO );
     		tcsetattr ( STDIN_FILENO, TCSANOW, &newt );
     		
        	FILE* fp = fopen("test.txt","rt");
         	int ch;
         	while((ch = fgetc(fp))!=EOF)
            {
					DataInsertion(list,(char)ch);
					fputc(ch,stdout);
			}
       		fclose(fp);
			break;			
			
		}	// end switch

	
// end COMMAND_MODE
	} //if
	else if (mode == INPUT_MODE)
	{
		switch (key) 
		{
			case 27: // escape key code
				mode = COMMAND_MODE;
				tcgetattr ( STDIN_FILENO, &curt );
				newt = curt;
				newt.c_lflag &= ~( ECHO );
				tcsetattr ( STDIN_FILENO, TCSANOW, &newt );
	 		sprintf(buff, "\033[%d;%dH%s",22,1,initial_message);
             fputs(buff, stdout);

				break;
			case 10: //Enter key code
				cur_line++;
				cur_col=1;
				DataInsertion(list,key);
				Print_key(list);
				break;
			case 9: //Delete key code : tab	
 		        fputs(" ",stdout);
				cur_col--;
				delete_key(list);
			
				if (cur_col<min_col)
				{
					cur_col=1;
					cur_col=min_col;
				}
			break;
			default:
				mode = INPUT_MODE;
				//calibrate cursor
				cur_col++;
				//save contents
				data[cur_col-1][cur_line-1] = key;
				DataInsertion(list,key);
				Print_key(list);			
				if (cur_col > max_col) {
					cur_col = 1;
					cur_line++;
					if (cur_line > max_line) cur_line = max_line;
			}
		break;
		}	// end switch
	// end INPUT_MODE
	}
	
	sprintf(buff, "\033[%d;%dH%3d:%3d", 21,70,cur_line,cur_col);
	fputs(buff, stdout);
	sprintf(buff, "\033[%d;%dH", cur_line, cur_col);
	fputs(buff, stdout); 


	}	// end while

 
	/* return termio back */ 
	tcsetattr ( STDIN_FILENO, TCSANOW, &oldt );

	return 0;
}

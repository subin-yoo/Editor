Editor_sample : main.o edit.o 
	gcc -lm -o Editor_sample main.o edit.o 

main.o : main.c node.h edit.h  
	gcc -c main.c

edit.o : node.h edit.c edit.h 
	gcc -c edit.c


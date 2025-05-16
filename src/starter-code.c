#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>




void allocate(char* PID, int size, char *type){

/* TODO*/

/*
allocates memory from a hole to a process based on the algorithm chosen.
Type = 'F' or 'f' for first fit, 'B' or 'b' for best fit, 'W' or 'w' for worst fit.
*/
}


void deallocate(char* PID){

/* TODO*/

/*
Deallocates memory from a process to a hole.
If the hole is adjacent to another hole, the two holes should be merged.
*/
}


void status(){

/* TODO*/

/*
Print the status of the memory manager.
Includes each hole / process address, the amount of free memory, and the amount of allocated memory; ordered by starting memory address.

Addresses [0:315000] Process P1
Addresses [315001: 512500] Process P3
Addresses [512501:625575] Unused
Addresses [625575:725100] Process P6
Addresses [725001] . . .

*/
}


void compact(){

/* TODO*/

/*
Compacts the memory allocations by shifting all current allocations to the top and combining all holes into a single hole.
*/
}


void printError(char *error){

/* TODO*/

/*
Prints an error message to the screen.
*/
}




int main(int argc, char *argv[]) {
	/* TODO: fill the line below with your names and ids */
	printf(" Group Name: Hole Lotta Trouble  \n Student(s) Name: Sinemis Toktaş & Atalay Görgün \n Student(s) ID: 76644 & 79925");
    
    // initialize first hole
    if(argc == 2) {
        
		/* TODO: Interactive mode */
        
		printf("HOLE INITIALIZED AT ADDRESS %d WITH %d BYTES\n",/* TODO*/, /* TODO*/);
    } else if(argc == 3) {
		/* TODO: Scripted mode*/
		
		
    } else {
        printError("ERROR Invalid number of arguments.\n");
        return 1;
    }
    
    while(1){
        char *input = malloc(sizeof(char) * 100);
        printf("allocator>");
        fgets(input, 100, stdin);
        input[strcspn(input, "\n")] = '\0'; // remove newline from input

        if(input[0] == '\0') { // empty input = do nothing 
            continue;
        }


        char* arguments[3];
        char* token = strtok(input, " ");
        int tokenCount = 0;

        // get all arguments from input
        while(token != NULL){
            arguments[tokenCount] = token;
            token = strtok(NULL, " ");
            tokenCount++;
        }
		
		// TODO: make commands case insensitive, i.e. should accept rq,RQ,rl,RL,stat,STAT,c,C,exit,EXIT
        
        // RQ (Request Memory / allocate): Needs 4 arguments and must check if they are valid arguments
        if(strcmp(arguments[0], "rq") == 0){
            if(  /* TODO */  ){
                allocate(  /* TODO*/  );
            }
            else{
                printError("ERROR Expected expression: RQ \"PID\" \"Bytes\" \"Algorithm\".");
            }
        }
        // RL (Release Memory / Deallocate): Needs 2 arguments and must check if they are valid arguments
        else if(strcmp(arguments[0], "rl") == 0){
            if(  /* TODO */  ){
                deallocate(  /* TODO */  );
            }
            else{
                printError("ERROR Expected expression: RL \"PID\".");
            }
        }
        // STATUS: Needs 1 argument
        else if(strcmp(arguments[0], "stat") == 0){
            if(  /* TODO*/  ){
                status();
            }
            else{
                printError("ERROR Expected expression: STATUS.");
            }
        }
        // C (Compact): Needs 1 argument
        else if(strcmp(arguments[0], "c") == 0){
            if(  /* TODO*/  ){
				
                /* TODO*/
            
			}
            else{
                printError("ERROR Expected expression: C.");
            }
        }
        // EXIT: Needs 1 argument
        else if(strcmp(arguments[0], "exit") == 0){
            if(tokenCount == 1){
                printf("Exiting program.\n");
                exit(0);
            }
            else{
                printError("ERROR Expected expression: EXIT.");
            }
        }
        // If command is not recognized, print error message and continue
        else{
            printError("ERROR Invalid command.");
        }
    }
}


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// *************************************** BLOCK ***************************************
// linked list structure for memory -> blocks are nodes
typedef struct Block {
    char PID[10];         // process id name -> "hole" if unused
    int base;   // start address
    int limit;  // number of bytes 
    struct Block *next;   // pointer to next block
} Block;

// block constructor
Block *createBlock(const char *PID, int base, int limit) {
    Block *newBlock = (Block *) malloc(sizeof(Block)); // allocate memory to hold a Block
    strcpy(newBlock->PID, PID); // set process id name of the new block
    newBlock->base = base; // set base
    newBlock->limit = limit; // set limit
    newBlock->next = NULL; // set new block's next pointer to null
    return newBlock; // return pointer to new block
}

// getter for start and end addresses of blocks based on base and limit values
int getStart(Block *b) { return b->base; } // start address
int getEnd(Block *b)   { return b->base + b->limit - 1; } // end address = base + limit - 1

// checks if memory block is a hole
bool isHole(Block *b){ return strcmp(b->PID, "hole") == 0; }

// checks if memory block has enough space
bool hasEnoughSpace (Block *b, int size){ return b->limit >= size; }

// *************************************************************************************

// *************************************** MEMORY **************************************
// linked list structure for memory

typedef struct {
    Block *head;       // pointer to start of memory blocks -> head pointer of linked list
    int total_memory;  // size of the memory 
} Memory;

// *************************************************************************************


// ********************************* helper functions **********************************

// Converts a string to lowercase
char* lowercase(char *s) {
    // source for tolower() : https://www.geeksforgeeks.org/tolower-function-in-c/

    size_t len = strlen(s);
    char *lowered = malloc(len + 1); // copy string
    if (!lowered) return NULL;

    for (int i = 0; i < len; i++) { // loop each char to lowercase
        lowered[i] = tolower(s[i]);
    }

    lowered[len] = '\0'; // add last 
    //printf("\n%s\n", lowered); // debug
    return lowered;
}


// *************************************************************************************



void allocate(char* PID, int size, char *type){

/* TODO*/

/*
allocates memory from a hole to a process based on the algorithm chosen.
Type = 'F' or 'f' for first fit, 'B' or 'b' for best fit, 'W' or 'w' for worst fit.
*/
    char flag = tolower(type[0]);

    if (flag == 'f') {
        // First-fit logic

        Block *previous = NULL; // previous node pointer
        Block *current = memory.head; // current pointer to head

        while (current != NULL) { // loop through linked list
            if (isHole(current) && hasEnoughSpace(current, size) ){
                // this block is allocatable
                if (current->limit == size){ // size is equal, no fragmentation 
                    strcpy(current->PID, PID); // just change name of current block
                }
                else{ // current->limit > size
                    Block *newProcess = createBlock(PID, current->base, size); // create memory block for new process

                    // insert new process block before current block
                    newProcess->next = current;
                    if (previous == NULL) {
                        memory.head = newProcess; // inserting at head
                    } else {
                        previous->next = newProcess; // inserting in middle
                    }

                    // update current block's base and limit 
                    current->base = current->base + size;
                    current->limit = current->limit - size;
                }
                return;
            }
            
            // update pointers after each loop
            previous = current;
            current = current->next;
        }

        printError("ERROR: Not enough memory");
        return;

    } else if (flag == 'b' || flag == 'w') {

        // Best-fit logic


    } else if () {
        // Worst-fit logic


    } else {
        printError("ERROR: Invalid allocation strategy");
        return;
    }
    
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
	printf(" Group Name: Hole Lotta Trouble  \n Student(s) Name: Sinemis Toktaş \n Student(s) ID: 0076644\n ");

    
    // initialize first hole
    if(argc == 2) {
		/* TODO: Interactive mode */

        int int_memory_amount = atoi(argv[1]);  // get initial memory amount from arguments

        // initialize memory as linked list
        Memory memory; 
        memory.total_memory = int_memory_amount;
        memory.head = createBlock("hole", 0, int_memory_amount); // init main memory as one GIANT hole

		printf("HOLE INITIALIZED AT ADDRESS %d WITH %d BYTES\n", memory.head->base, memory.head->limit);

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
        arguments[0] = lowercase(arguments[0]);

        // RQ (Request Memory / allocate): Needs 4 arguments and must check if they are valid arguments
        if(strcmp(arguments[0], "rq") == 0){
            if(  tokenCount == 4  ){
                allocate(  arguments[1], atoi(arguments[2]), arguments[3] );
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


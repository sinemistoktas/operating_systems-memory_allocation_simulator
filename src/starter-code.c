#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum { false, true } bool; // for returning bool in isHole and hasEnoughSpace functions since I can't add the library stdbool.h based on assignment restrictions
void printError(char *error); // function prototype for early calls
const char *HOLE_PID = "Unused";

// *************************************** BLOCK ***************************************
// doubly linked list structure for memory -> blocks are nodes
typedef struct Block {
    char PID[10];         // process id name -> "Unused" if unused
    int base;   // start address
    int limit;  // number of bytes 
    struct Block *prev; // pointer to previous block
    struct Block *next;   // pointer to next block
} Block;

// block constructor
Block *createBlock(const char *PID, int base, int limit) {
    Block *newBlock = (Block *) malloc(sizeof(Block)); // allocate memory to hold a Block
    strcpy(newBlock->PID, PID); // set process id name of the new block
    newBlock->base = base; // set base
    newBlock->limit = limit; // set limit
    newBlock->prev = NULL; // set new block's prev pointer to null
    newBlock->next = NULL; // set new block's next pointer to null
    return newBlock; // return pointer to new block
}

// checks if memory block is a hole
bool isHole(Block *b){ return strcmp(b->PID, HOLE_PID) == 0; }

// checks if memory block has enough space
bool hasEnoughSpace (Block *b, int size){ return b->limit >= size; }

// printer for block to use for STAT
void printBlock(Block *b) {
    int start = b->base;
    int end = b->base + b->limit - 1;

    if (isHole(b)) {
        printf("Addresses [%d:%d] Unused\n", start, end); // handles hole case print
    } else {
        printf("Addresses [%d:%d] Process %s\n", start, end, b->PID); // handles non-hole case print
    }
}

// *************************************************************************************



// *************************************** MEMORY **************************************
// linked list structure for memory

typedef struct {
    Block *head;       // pointer to start of memory blocks -> head pointer of linked list
    int total_memory;  // size of the memory 
} Memory;


Memory memory;  // Global memory initiated


// function for checking that there are no processes with the same PID
bool pidExists(const char *PID){
    // used for not allowing duplicate PID names

    Block *current = memory.head;

    while (current != NULL){
        if (!isHole(current) && strcmp(current->PID, PID) == 0){ // pass holes and check if process with given PID already exists
            return true;
        } 
        current = current->next; // update current pointer
    }
    return false;
} 

// *************************************************************************************




// ********************************* helper functions **********************************

// Converts a string to lowercase
void lowercase(char *s) {
    // source for tolower() : https://www.geeksforgeeks.org/tolower-function-in-c/

    for (int i = 0; s[i]; i++) {
        s[i] = tolower(s[i]);
    }
    
}

// *************************************************************************************



void allocate(char* PID, int size, char *type){

/* TODO*/

/*
allocates memory from a hole to a process based on the algorithm chosen.
Type = 'F' or 'f' for first fit, 'B' or 'b' for best fit, 'W' or 'w' for worst fit.
*/
    // argument validation
    if (pidExists(PID)){ // check if there is already a process with given PID
        printError("ERROR: Given PID already exists!");
        return; // we can't allow PID duplicates because it would corrupt allocation logic
    }

    if (size <= 0) {
        printError("ERROR: Invalid memory request size.");
        return;
    }

    char flag = tolower(type[0]); // make strategy flag case-insensitive
    if (flag != 'f' && flag != 'b' && flag != 'w') {
        printError("ERROR: Invalid allocation strategy, must be F, B, or W.");
        return;
    }
    

    Block *current = memory.head; // current pointer to head

    if (flag == 'f') {
        // First-fit logic

        while (current != NULL) { // loop through linked list
            if (isHole(current) && hasEnoughSpace(current, size) ){
                // this block is allocatable
                if (current->limit == size){ // size is equal, no fragmentation 
                    strcpy(current->PID, PID); // just change name of current block
                }
                else{ // current->limit > size
                    Block *newProcess = createBlock(PID, current->base, size); // create memory block for new process

                    // insert new process block before current block
                    newProcess->prev = current->prev;
                    newProcess->next = current;
                    if (current->prev == NULL) {
                        memory.head = newProcess; // inserting at head
                    } else {
                        current->prev->next = newProcess; // inserting in middle
                    }
                    current->prev = newProcess;

                    // update current block's base and limit 
                    current->base += size;
                    current->limit -= size;
                }
                
                return;
            }
            
            // update current pointer after each loop
            current = current->next;
        }

        printError("ERROR: Insufficient memory to allocate to the request.");
        return;

    } else { // Best-fit & Worst-fit logic

        Block *bestFit = NULL; // for best-fit logic
        Block *worstFit = NULL; // for worst-fit logic
        int bestFitSize = memory.total_memory; // possible max size
        int worstFitSize = -1;

        while (current != NULL) { // loop through linked list
            if (isHole(current) && hasEnoughSpace(current, size) ){
                // this block is allocatable
                if (current->limit < bestFitSize){ // check if current is the smallest hole we have found -> for best-fit logic
                    bestFit = current; // save current as best fit block
                    bestFitSize = current->limit; // update bestFitSize
                }
                if (current->limit > worstFitSize){ // check if current is the biggest hole we have found -> for worst-fit logic
                    worstFit = current; // save current as worst fit block
                    worstFitSize = current->limit; // update worstFitSize
                }
            }

            // update current pointer after each loop
            current = current->next;
        }

        // after looping through whole memory is finished, we insert new block
        Block *target = NULL;
        if (flag == 'b') target = bestFit; // for best-fit logic
        else if (flag == 'w') target = worstFit; // for worst-fit logic

        if (target == NULL) {
            printError("ERROR: Insufficient memory to allocate to the request.");
            return;
        }

        if (target->limit == size){ // limit = size, no fragmentation 
            strcpy(target->PID, PID); // just change name of target block
        }
        else{ // limit > size
            Block *newProcess = createBlock(PID, target->base, size); // create memory block for new process

            // insert new process block before target block
            newProcess->prev = target->prev;
            newProcess->next = target;
            if (target->prev == NULL) {
                memory.head = newProcess; // inserting at head
            } else {
                target->prev->next = newProcess; // inserting in middle
            }
            target->prev = newProcess;

            // update target block's base and limit 
            target->base += size;
            target->limit -= size;
        }

        return;
    }
}


void deallocate(char* PID){

/* TODO*/

/*
Deallocates memory from a process to a hole.
If the hole is adjacent to another hole, the two holes should be merged.
*/
    // argument validation
    if (strcmp(PID, HOLE_PID) == 0) {
        printError("ERROR: Can not deallocate a hole.");
        return;
    }
    

    Block *current = memory.head; // current pointer to head

    while (current != NULL) { // loop through linked list
        // edge case start and end
        if (!isHole(current) && strcmp(current->PID, PID) == 0){ // find the block with given PID
            // deallocate current block by turning it to a hole
            strcpy(current->PID, HOLE_PID);

            // if prev and next are holes, current will be merged into one large block with prev and next

            // check if next block is hole -> for merging
            if (current->next != NULL && isHole(current->next)){ // check if current is not last node AND next block is hole
                Block *next = current->next; // get a pointer to next block
                current->limit += next->limit; // elongate current block's limit with next block's limit to merge
                current->next = next->next; // update merged current block's next pointer to block after next block or NULL if next block was last node
                
                if (next->next != NULL){ // check if next block is not last node
                    next->next->prev = current; // update block after next block's prev pointer to point to merged current block
                }
                free(next); // dealloc memory block next points to -> remove next block since its already merged with current block
            }


            // check if previous block is hole -> for merging
            if (current->prev != NULL && isHole(current->prev)){ // check if current is not head node AND next prev is hole
                Block *previous = current->prev; // get a pointer to previous block
                previous->limit += current->limit; // elongate previous block's limit with current block's limit to merge
                previous->next = current->next; // update merged previous block's next pointer to block after current block
                
                if (current->next != NULL){ // check if current block is not last node
                    current->next->prev = previous; // update next block's prev pointer to point to merged (previous+current) block
                }
                free(current); // dealloc memory at current block -> remove current block since its already merged with previous block
            }

            return; // deallocate done
        
        }

        // update current pointer
        current = current->next;
    }
    // else -> PID was not found
    printError("ERROR: Given PID does not exist in memory.");
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

    Block *current = memory.head;

    while (current != NULL){
        printBlock(current); // custom print function for blocks
        current = current->next; // update current pointer
    }
    
}


void compact(){

/* TODO*/

/*
Compacts the memory allocations by shifting all current allocations to the top and combining all holes into a single hole.
*/
// shift all memory blocks to top and shift all holes to bottom to form one giant hole

    // initalize new variables to form a new shuffled memory list
    int base = 0; // to store base value for the blocks I will create while shuffling 
    int totalHoleSpace = 0; // to store total amount of hole block memory space
    Block *newHead = NULL; // pointer to new head
    Block *lastProcess = NULL; // to store pointer to last (non-hole) process block
    Block *current = memory.head; // current pointer to head

    while (current != NULL) {
        if (!isHole(current)){  // if current block is not a hole
            Block *shuffledBlock = createBlock(current->PID, base, current->limit); // create a new block to shuffle the process block
            base += current->limit; // update base by adding current block's limit

            // set newHead to first process block found
            if (newHead == NULL){ 
                newHead = shuffledBlock;
            } 
            else { // for non-first process blocks 
                shuffledBlock->prev = lastProcess; // set new (shuffled) process block's prev pointer to the last process block
                lastProcess->next = shuffledBlock; // set last process block's next pointer to the new process block
            }

            // update last process block pointer for the next shuffles
            lastProcess = shuffledBlock; 

        } else {  // else, if current block is hole
            totalHoleSpace += current->limit; // update how much hole space we have
        }

        // update current pointer to next by also removing current node
        Block *next = current->next;
        free(current); // remove current node's block
        current = next; // update current pointer
    }

    // create one giant hole at bottom
    if (totalHoleSpace > 0) { // if we had holes
        Block *giantHole = createBlock(HOLE_PID, base, totalHoleSpace); // create one giant hole block

        if (lastProcess != NULL){ // if we had a process block
            // link last memory block to giant hole block
            giantHole->prev = lastProcess; 
            lastProcess->next = giantHole;
        }
        else{ // if we didn't had any process 
            newHead = giantHole; // point new head to giant hole block
        }
    }

    // Lastly, update Memory head to point to new head
    memory.head = newHead;
}


void printError(char *error){

/* TODO*/

/*
Prints an error message to the screen.
*/

    fprintf(stderr, "%s\n", error); // print error to standard error output stream
}




int main(int argc, char *argv[]) {
	/* TODO: fill the line below with your names and ids */
	printf(" Group Name: Hole Lotta Trouble  \n Student(s) Name: Sinemis Toktaş \n Student(s) ID: 0076644\n ");

    
    // initialize first hole
    if(argc == 2) {
		/* TODO: Interactive mode */

        int int_memory_amount = atoi(argv[1]);  // get initial memory amount from first argument

        // initialize memory as linked list
        memory.total_memory = int_memory_amount;
        memory.head = createBlock(HOLE_PID, 0, int_memory_amount); // init main memory as one GIANT hole

		printf("HOLE INITIALIZED AT ADDRESS %d WITH %d BYTES\n", memory.head->base, memory.head->limit);

    } else if(argc == 3) {
		/* TODO: Scripted mode*/

        int int_memory_amount = atoi(argv[1]);  // get initial memory amount from first argument
        char *fileName = argv[2]; // get file name from second argument

        // initialize memory as linked list
        memory.total_memory = int_memory_amount;
        memory.head = createBlock(HOLE_PID, 0, int_memory_amount); // init main memory as one GIANT hole

        // open file with given file name in read mode
        // source: https://www.geeksforgeeks.org/basics-file-handling-c/
        FILE *fptr = fopen(fileName, "r");

        if (fptr == NULL){
            printError("ERROR: Could not open script file.");
            return 1;
        } 
        else{
            // read the file
            char line[50]; // Declare the character array for each line to be read from file

            // Read the data from the file using fgets() method
            while (fgets(line, 50, fptr) != NULL) {
                line[strcspn(line, "\n")] = '\0'; // remove newline from line

                if(line[0] == '\0') { // empty input = do nothing 
                    continue;
                }

                // tokenize inputs at each line like it is done in interactive mode
                char* arguments[4]; // max is 4 for RQ
                char* token = strtok(line, " ");
                int tokenCount = 0;

                while(token != NULL){ // get all arguments from input
                    arguments[tokenCount] = token;
                    token = strtok(NULL, " ");
                    tokenCount++;
                }
                
                // make commands case insensitive, i.e. should accept rq,RQ,rl,RL,stat,STAT,c,C,exit,EXIT
                lowercase(arguments[0]);

                // RQ (Request Memory / allocate): Needs 4 arguments and must check if they are valid arguments
                if(strcmp(arguments[0], "rq") == 0){
                    if(  tokenCount == 4  ){
                        allocate(  arguments[1], atoi(arguments[2]), arguments[3] );
                    }
                }
                // RL (Release Memory / Deallocate): Needs 2 arguments and must check if they are valid arguments
                else if(strcmp(arguments[0], "rl") == 0){
                    if(  tokenCount == 2  ){
                        deallocate(  arguments[1]  );
                    }
                }
                // ignore STAT 

                // C (Compact): Needs 1 argument
                else if(strcmp(arguments[0], "c") == 0){
                    if(  tokenCount == 1  ){
                        /* TODO*/
                        compact();
                    }
                }
                // EXIT: Needs 1 argument
                else if(strcmp(arguments[0], "exit") == 0){
                    if(tokenCount == 1){
                        break;
                    }
                    
                }
                // If command is not recognized, do nothing and continue
                else{
                    continue;
                }

            }

            // Closing the file using fclose()
            fclose(fptr);

            // Final STAT output after file read ends
            status();

            return 0; // prevent going to interactive mode
        }

		
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
        lowercase(arguments[0]);

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
            if(  tokenCount == 2  ){
                deallocate(  arguments[1]  );
            }
            else{
                printError("ERROR Expected expression: RL \"PID\".");
            }
        }
        // STATUS: Needs 1 argument
        else if(strcmp(arguments[0], "stat") == 0){
            if(  tokenCount == 1  ){
                status();
            }
            else{
                printError("ERROR Expected expression: STATUS.");
            }
        }
        // C (Compact): Needs 1 argument
        else if(strcmp(arguments[0], "c") == 0){
            if(  tokenCount == 1  ){
                /* TODO*/
                compact();
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


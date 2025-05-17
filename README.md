# COMP 304 - Project 2: Contiguous Memory Allocation
**Group Name:** Hole Lotta Trouble  
**Student Name:** Sinemis Toktaş  
**Student ID:** 0076644  

## Project Description
This project implements a memory management simulator that supports contiguous memory allocation using three different strategies: First Fit, Best Fit, and Worst Fit. The simulator can operate in both interactive and scripted modes, simulating the behaviors of RQ (Request Memory), RL (Release Memory), C (Compact), STAT (Status Report), and EXIT commands.

---

## Implementation Details
### Data Structures
- **Block (struct):** Represents a memory segment. Each block stores:
  - `PID` (char[10]) – Process ID or "Unused" for holes
  - `base` (int) – Start address
  - `limit` (int) – Size in bytes
  - `prev` and `next` pointers for a doubly linked list

- **Memory (struct):** Implemented as a doubly linked list
  - `head` – pointer to the first Block
  - `total_memory` – total available memory, set by the user

### Key Functions
- `createBlock(PID, base, limit)` – Allocates and initializes a new memory block.
- `allocate(PID, size, strategy)` – Handles memory requests using First, Best, or Worst Fit.
- `deallocate(PID)` – Frees memory and merges adjacent holes.
- `compact()` – Moves all used blocks to the top and creates one large hole at the bottom.
- `status()` – Prints the current memory layout.
- `printBlock()` – Utility to display each block's status.
- `lowercase()` – Converts given string to lowercase, used for supporting case-insensitive flag arguments.
- `printError()` – Outputs formatted error messages to stderr.

---

## Features
- **Doubly linked list memory representation** for easier merging and compaction
- **Scripted mode** using file input (no intermediate prompts)
- **Final STAT output** shown only once at end of script (in scripted mode)
- **Case-insensitive** command parsing (except for process ID)
- **Edge case handling** done by printing error messages (invalid size, duplicate PID, invalid strategy argument, nonexistent PID, unopenable script file)

---

## How to Run
Compile `starter-code.c`:
```bash
gcc starter-code.c -o allocator
```

### Interactive Mode:
Total memory amount is set by the user.
```bash
./allocator <total memory amount>
```
Example: 
```bash
./allocator 100
```

### Scripted Mode:
Total memory amount and name of the scirpt txt file are given by the user.
```bash
./allocator <total memory amount> <script txt file name>
```
Example: 
```bash
./allocator 100 commands-to-exexcute.txt
```


---

## External Resources & AI Assistance

### Web Resources
- [GeeksforGeeks - File handling in C](https://www.geeksforgeeks.org/basics-file-handling-c/)
- [GeeksforGeeks - tolower() function in C](https://www.geeksforgeeks.org/tolower-function-in-c/)

### AI Usage (ChatGPT - OpenAI GPT-4, May 2025)
AI assistance was used during the design, debugging, and testing of this project. Below is a detailed breakdown of the prompts and responses:

&nbsp;

**🔹 Prompt 1: "Which design is best for memory: array, linked list, stack?"**

**AI Response Summary:**

- For dynamic memory allocation simulators, linked lists are preferred over arrays and stacks.

- Arrays are not flexible for fragmentation and merging (fixed size, hard to insert/delete).

- Stacks follow LIFO logic and are too restrictive — only suitable for temporary storage or undo operations.

- Linked lists (especially when representing holes and process blocks) allow easy insertion, deletion, and merging — ideal for representing fragmented memory.

- Therefore, linked lists offer the best flexibility and correctness for simulating memory allocation.  

&nbsp;


**🔹 Prompt 2: "Is doubly linked list a better idea than single linked list?"**

**AI Response Summary:**

A doubly linked list is a better choice for simulating memory allocation compared to a singly linked list, especially when:

You need to efficiently merge adjacent holes during deallocation.

You want to compact memory, which involves rebuilding or traversing the list in both directions.

While a singly linked list can work, it requires managing a separate prev pointer externally, which complicates the code and increases the chance of bugs.

The AI recommended a doubly linked list to simplify implementation and ensure correct and maintainable memory operations. This suggestion was implemented in the final design.

&nbsp;


**🔹 Prompt 3: "Can you give me test cases and edge cases for RQ, RL, and C?"**

**AI Response Summary:**

AI provided a categorized and structured set of test cases (both in the form of script text files and command line scripts) for:

- RQ (Request): basic allocations, strategy comparisons (First Fit, Best Fit, Worst Fit), duplicate PID errors, invalid sizes, and insufficient memory

- RL (Release): standard deallocation, merging with previous/next holes, head/tail position tests, and handling of invalid or nonexistent PIDs

- C (Compact): shifting processes up, merging fragmented holes, and verifying no effect when memory is already compacted

- Edge cases were highlighted, including:

  - Deallocating the head or last node

  - Holes surrounding a process block

  - Entire memory being a process or hole

  - Worst/best-fit tie scenarios

&nbsp;

These test cases were used to validate correctness and memory integrity under different conditions.

---

# Memory Allocation Simulator
**Operating Systems - Project 2**  
**Spring 2025 | Koç University**

## Contributors
- **Sinemis Toktaş**

## Overview
This repository showcases Project 2 from the Operating Systems course completed at Koç University during Spring 2025. The project implements a comprehensive memory management simulator supporting contiguous memory allocation using three fundamental algorithms with both interactive and scripted execution modes.

## Key Features

### **Allocation Algorithms**
- **First Fit (F):** Allocates memory in the first available hole large enough
- **Best Fit (B):** Allocates memory in the smallest suitable hole
- **Worst Fit (W):** Allocates memory in the largest available hole
- **Case-Insensitive:** Supports both uppercase and lowercase strategy flags

### **Core Operations**
- **Request (RQ):** Allocate contiguous memory blocks with specified strategy
- **Release (RL):** Deallocate memory with automatic adjacent hole merging
- **Compact (C):** Consolidate fragmented holes into single contiguous block
- **Status (STAT):** Display comprehensive memory layout and allocation map
- **Exit (X):** Graceful program termination

### **Execution Modes**
- **Interactive Mode:** Real-time command processing with user prompts
- **Scripted Mode:** Batch execution from command files with final status output
- **Error Handling:** Robust validation with detailed error messages

### **Advanced Features**
- **Dynamic Memory Management:** Doubly-linked list for efficient operations
- **Automatic Hole Merging:** Combines adjacent free blocks on deallocation
- **Fragmentation Prevention:** Smart compaction algorithm
- **Memory Safety:** Comprehensive bounds checking and validation

## Usage

### **Interactive Mode**
```bash
# Compile and run
gcc starter-code.c -o allocator
./allocator 1048576

# Example session
allocator> RQ P0 40000 W     # Request 40KB using Worst Fit
allocator> RQ P1 25000 F     # Request 25KB using First Fit  
allocator> STAT              # Display memory status
allocator> RL P0             # Release P0's memory
allocator> C                 # Compact memory holes
allocator> X                 # Exit
```

### **Scripted Mode**
```bash
# Execute batch commands
./allocator 1048576 commands.txt

# Example commands.txt:
# RQ P1 100000 B
# RQ P2 50000 F  
# RL P1
# C
```

## Implementation Highlights

### **Data Structures**
- **Block Structure:** Doubly-linked list nodes with PID, base address, and size
- **Memory Management:** Dynamic hole tracking with efficient insertion/deletion
- **Process Validation:** Duplicate PID prevention and existence checking

### **Key Algorithms**
```c
// Memory allocation with strategy selection
void allocate(char* PID, int size, char *strategy)

// Deallocation with automatic merging
void deallocate(char* PID)  

// Memory compaction with address recalculation
void compact()

// Comprehensive status reporting
void status()
```

### **File Structure**
```
memory-allocator/
├── starter-code.c        # Main implementation
├── allocator            # Compiled executable  
├── Makefile            # Build configuration
└── README.md           # This file
```

## Sample Output

### **Memory Status Display**
```
Addresses [0:315000] Process P1
Addresses [315001:512500] Process P3  
Addresses [512501:625575] Unused
Addresses [625576:725100] Process P6
Addresses [725101:1048575] Unused
```

### **Error Handling**
```
ERROR: Insufficient memory to allocate to the request.
ERROR: Given PID already exists!
ERROR: Given PID does not exist in memory.
```

## Technical Features

### **Memory Management**
- **Efficient Allocation:** O(n) time complexity for all allocation strategies
- **Smart Merging:** Automatic combination of adjacent holes during deallocation
- **Compaction Strategy:** Moves all processes to top, creates single hole at bottom
- **Fragmentation Handling:** Minimizes external fragmentation through compaction

### **Robust Design**
- **Input Validation:** Comprehensive argument checking and error reporting
- **Memory Safety:** Proper allocation/deallocation with leak prevention
- **Edge Case Handling:** Boundary conditions and invalid operations
- **Cross-Platform:** Compatible across different Linux distributions

## Learning Outcomes
This project demonstrates proficiency in:
- **Memory Management:** Practical implementation of allocation algorithms
- **Data Structures:** Dynamic linked list operations and manipulation
- **Algorithm Analysis:** Understanding of different allocation strategy trade-offs
- **System Design:** Modular architecture with clean separation of concerns
- **C Programming:** Advanced pointer manipulation and memory operations

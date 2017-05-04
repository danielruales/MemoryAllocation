//Daniel Ruales
//COP3503 pa.2

#include <map>
#include <algorithm>
#include <math.h>
#include <vector>
#include <iostream>
#include <cstdio>
#include "pa2.h"
using namespace std;

// The T highlights the class template parameter and the function's below template parameter
// Begins templates
//uses ListNode

template <class T>
ListNode<T>::ListNode(T inputData):
data(inputData),

next(NULL) {}

template <class T>
//sets the element
// after setting the element, creates ListNode in order to store elements
// uses getData, and returns data
T ListNode<T>::getData() {
    return data;
}

//uses setData
//returns data
template <class T>

T ListNode<T>::setData(T passed) {
    data = passed;
    
    return data;
}

//uses ListNode and getNext
//returns next
template <class T>
ListNode<T> *ListNode<T>::getNext() {
    return next;
}

//uses ListNode and setNext
//returns next
template <class T>
ListNode<T> *ListNode<T>::setNext(ListNode<T> *node) {
    next = node;
    return next;
}

//uses LinkedList, ListNode, and getHead
//returns head
template <class T>
LinkedList<T>::LinkedList():
head(NULL) {}
template <class T>
ListNode<T> *LinkedList<T>::getHead() {
    return head;
}

//uses ListNode, LinkedList, and setHead
//returns head
template <class T>
ListNode<T> *LinkedList<T>::setHead(ListNode<T> *head) {
    this->head = head;
    return
    this->head;
}

//uses ListNode, LinkedList, and append
//has multiple return statements for head and temp -> getNext
template <class T>
ListNode<T> *LinkedList<T>::append(T data) {
    if (head == NULL) { //if list is currently empty
        head = new ListNode<T>(data);
        return head;
    }
    ListNode<T> *temp = head;
    while (temp->getNext() != NULL) {
        temp = temp -> getNext();
    }
    temp->setNext(new ListNode<T>(data));
    return temp ->  getNext();
}

//Memory Allocator that takes in string pass_alg, and int pages
//First the fit algorithm must be chosen
MemoryAllocator::MemoryAllocator(string passed_alg, int pages) {
    algorithm = passed_alg;
    printf("Using %s fit algorithm\n", algorithm.c_str());
    freeMemory = LinkedList<Chunk>();
    freeMemory.append(Chunk(0, pages - 1));
    usedMemory = LinkedList<UsedMemoryChunk>();
}

typedef pair<ListNode<Chunk>*, int> MyPairType;

// Compares MyPairType 's
struct CompareSecond
{
    bool operator()(const MyPairType& left, const MyPairType& right)
    const
    {
        return left.second < right.second;
    }
};

//Acts as a check for accuracy of name and size with the knowledge of previous nodes. Inserts with the concern on "best" or "worst" fit algorithms, and then the function determines in which position to allocate a new node with a basis on page data. TLDR, adds program with certain checks.
//Memory Allocator
void
MemoryAllocator::add_program(ProgramInfo prog_info) {
    if (prog_info.size <= 0) {
        printf("Error: Size not valid\n");
        return;
    }
    
    //Ensures a unique program name
    ListNode<UsedMemoryChunk> *used_current = usedMemory.getHead();
    while (used_current != NULL) {
        if (used_current->getData().programName == prog_info.name) {
            printf("Error, Program %s is already running. \n", prog_info.name.c_str());
            return;
        }
        else {
            used_current = used_current->getNext();
        }
    }
    
    map<ListNode<Chunk>*,
    int> open_slots;
    
    ListNode<Chunk> *current = freeMemory.getHead();
    
    //Skims over program size and ensures enough space for program that is trying to be applied
    int pages = ceil(prog_info.size / 4.0);
    
    while (current != NULL) {
        int open_size = (current->getData().endPage - current->getData().startPage) + 1;
        if (open_size >= (pages)) {
            open_slots[current] = open_size;
        }
        current = current->getNext();
    }
    
    if (open_slots.size() == 0){
        printf("Error, Not enough memory for Program \n");
        return;
    }
    
    ListNode<UsedMemoryChunk> *new_node = NULL;
    ListNode<Chunk> *usable_node = NULL;
    
    //If statement and action for best algorithm
    if (this->algorithm == "best") {
        pair<ListNode<Chunk>*,
        int> min = *min_element(open_slots.begin(), open_slots.end(), CompareSecond());
        usable_node = min.first;
    }
    else {
        //Else statement and action for worst fit algorithm
        pair<ListNode<Chunk>*, int> max =
        *max_element(open_slots.begin(), open_slots.end(), CompareSecond());
        usable_node = max.first;
    }
    
    
    //Free memory won't be included if the end page is smaller than the initial page and pages.
    //First if statement compares chunk_to_use.startPage + pages to chunk_to_use.endPage in order to ensure which action to pursue.
    //past_node is the latest node identified.
    Chunk chunk_to_use = usable_node-> getData();
    if (chunk_to_use.startPage + pages > chunk_to_use.endPage)
    {
        if (freeMemory.getHead() == usable_node) {
            freeMemory.setHead(usable_node->getNext());
        }
        else {
            bool past_node =
            false;
            current = freeMemory.getHead()->getNext();
            while (current != NULL && !past_node) {
                if (current->getNext() == usable_node) {
                    current->setNext(usable_node->getNext());
                    past_node =
                    true;
                } else {
                    current = current->getNext();
                }
            }
        }
    }
    else {
        usable_node->setData(Chunk(chunk_to_use.startPage + pages, chunk_to_use.endPage));
    }
    
    int chunkInitial = chunk_to_use.startPage, chunkFinal = chunk_to_use.startPage + pages - 1;
    
    new_node = new ListNode<UsedMemoryChunk>(UsedMemoryChunk(chunkInitial, chunkFinal, prog_info.name));
    
    used_current = usedMemory.getHead();
    
    ListNode<UsedMemoryChunk>* used_final = usedMemory.getHead();
    
    if (used_current != NULL && used_current->getData().startPage > new_node->getData().startPage)
    {
        
        new_node->setNext(usedMemory.getHead());
        
        usedMemory.setHead(new_node);
        //Prints the successfull statement if the program is added successfully and the amount of pages used
        printf("Program %s added successfully: %d page(s) used\n\n", prog_info.name.c_str(),pages);
        return;
    }
    
    // This will not stop placing util the point of surpassing the initial page of allocated memory
    while (used_current != NULL) {
        if (used_current->getData().startPage > new_node->getData().startPage) {
            used_final->setNext(new_node);
            new_node->setNext(used_current);
            printf("Program %s added successfully: %d page(s) used\n\n", prog_info.name.c_str(), pages);
            return;
        } else {
            used_final = used_current;
            used_current = used_current->getNext();
        }
    }
    
    // Safety measure for an append
    usedMemory.append(new_node->getData());
    printf("Program %s added successfully: %d page(s) used\n\n",
           prog_info.name.c_str(), pages);
}

void MemoryAllocator::defragment() {
    ListNode<Chunk> *current = freeMemory.getHead(),
    *final = NULL;
    while (current != NULL) {
        if (final == NULL) {
            final = current;
            current = current->getNext();
        }
        else {
            int final_end = final->getData().endPage,
            current_start = current->getData().startPage;
            if (final_end + 1 == current_start) {
                Chunk expanded(final->getData().startPage,
                               current->getData().endPage);
                final->setData(expanded);
                final->setNext(current->getNext());
                return;
            } else {
                final = current;
                current = current->getNext();
            }
        }
    }
}


// Either the program name does not exist and exit or delete the node
// from used and appropriately add a new one to new.

//MemoryAllocator
void
MemoryAllocator::kill_program(string programName) {
    int freed_start_page = 0, freed_end_page = 0;
    bool program_found = false;
    ListNode<UsedMemoryChunk> *used_current = usedMemory.getHead();
    ListNode<UsedMemoryChunk> *used_final = NULL;
    while (used_current != NULL && !program_found) {
        UsedMemoryChunk current_chunk = used_current->getData();
        if (programName.compare(current_chunk.programName) == 0) {
            freed_start_page = current_chunk.startPage;
            freed_end_page = current_chunk.endPage;
            if (used_final != NULL) {
                used_final->setNext(used_current->getNext());
            }
            else {
                usedMemory.setHead(used_current->getNext());
            }
            program_found =  true;
        }
        else {
            used_final = used_current;
            used_current = used_current->getNext();
        }
    }
    
    //If statement for not being able to find a program the user types in
    if (!program_found) {
        
        printf("Could not find program with name %s\n", programName.c_str());
        
        return;
        
    }
    
    Chunk freed(used_current->getData().startPage, used_current->getData().endPage);
    ListNode<Chunk> *new_node = new ListNode<Chunk>(freed);
    
    if (freeMemory.getHead() == NULL) {
        freeMemory.setHead(new_node);
        printf("%s successfully killed, %d page(s) reclaimed\n\n", programName.c_str(),freed_end_page - freed_start_page +1);
        return;
    }
    
    /* Go through free memory until passing start page of freed memory, then create new node and point to first node */
    //Runs through free memory until initial page of freed memory
    //Then new node is built and point to the first node
    ListNode<Chunk> *free_current = freeMemory.getHead();
    ListNode<Chunk> *free_final = NULL;
    
    while (free_current != NULL) {
        int current_start_page = free_current->getData().startPage;
        
        if (current_start_page > freed.startPage) {
            new_node->setNext(free_current);
            if (free_final == NULL) {
                freeMemory.setHead(new_node);
            } else {
                free_final->setNext(new_node);
            }
            defragment();
            printf("Program %s successfully killed, %d page(s) reclaimed.\n\n", programName.c_str(), freed_end_page - freed_start_page + 1);
            return;
        } else {
            free_final = free_current;
            free_current = free_current->getNext();
        }
    }
}

//Memory Allocator
//print_fragmentation in order to print the number of fragments
//amount_frag is number of fragmentations
void
MemoryAllocator::print_fragmentation() {
    int amount_frag = 0;
    ListNode<Chunk> *current = freeMemory.getHead();
    while (current != NULL) {
        amount_frag++;
        current = current->getNext();
    }
    printf("There are %d fragment(s). \n\n", amount_frag);
}

//Memory Allocator
//print_memory for printing the memory
void
MemoryAllocator::print_memory() {
    
    map<int, string> used_pages;
    ListNode<UsedMemoryChunk> *current = usedMemory.getHead();

    while (current != NULL) {
        UsedMemoryChunk info = current->getData();
        for (int i = info.startPage; i <= info.endPage; i++) {
            used_pages[i] = info.programName;
        }
        current = current->getNext();
    }
    //r is row of memory
    //c is column of memory
    //thus, prints the memory from each row and column accurately, including free memory
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 8; c++) {
            int curr_page = r * 8 + c;
            if (used_pages.find(curr_page) != used_pages.end())
            {
                printf("%s ", used_pages[curr_page].c_str());
            }
            else {
                printf("Free ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

//Certain printing instructions are for the menu
void
menu_print() {
    cout << "1. Add program\n"
    << "2. Kill program\n"
    << "3. Fragmentation\n"
    << "4. Print memory\n"
    << "5. Exit\n"
    << endl;
}

//asks for the users choice/input
int getChoice(){
    string choice;
    printf("choice - ");
    cin >> choice;
    if(isdigit(choice[0])){
        int c = stoi(choice);
        return c;
    }
    printf("\n");
    return -1;
}

//uses get_programName to call and print the program name by asking the user to input the name
string get_programName() {
    string name;
    printf("Program name - ");
    cin >> name;
    return name;
}

//uses get_programInfo to call and print the program size in kilobytes by asking for the input
ProgramInfo
get_programInfo() {
    string name = get_programName();
    int size;
    printf("Program size (KB) -");
    cin >> size;
    return ProgramInfo(name, size);
}

//runs the users choice for the input on the menu
//Memory Allocator
int choice_menu(string algorithm) {
    MemoryAllocator mem_alloc(algorithm, 32);
    menu_print();
     int choice = -1;
    while (choice != 5) {
        choice = getChoice();
        switch (choice) {
            case 1: mem_alloc.add_program(get_programInfo());
                break;
            case 2: mem_alloc.kill_program(get_programName());
                break;
            case 3: mem_alloc.print_fragmentation();
                break;
            case 4: mem_alloc.print_memory();
                break;
            case 5: break;
            default: printf("Error, invalid option.\n");
                choice = -1;
        }
    }
    return 0;
}

//main method
//Asks for the users first input by asking for the "best" or "worst" algorithm
//After user decides the user is brought to the menu to decide next
//Checks for bad input
int main (){
    string x;
    string best = "best";
    string worst = "worst";
    cout << ("> .a.out ");
    cin >> x;
    if (x.compare(best) == 0 || x.compare(worst) == 0){
        choice_menu(x);
    }
    else{
        cout << ("Bad input");
        return 1;
    }
}

//  Daniel Ruales
//  pa2.h

#ifndef pa2_h
#define pa2_h
#include <string>
using namespace std;

//For template
//Class ListNode
//Private data fields
template <class T> class ListNode {
private:
    T data;
    ListNode <T> * next;

    //Public date fields
public:
    ListNode(T data);
    
    T setData(T data);
    ListNode<T> *setNext(ListNode<T> *node);
    T getData();
    ListNode<T> * getNext();
};

//For template
//Class LinkedList
//Private data field
template <class T> class LinkedList{
private:
    ListNode<T>*head;
    
    //Public data fields
public:
    LinkedList();
    ListNode<T> *setHead(ListNode<T>*head);
    ListNode<T> *getHead();
    ListNode<T> *append(T data);
};

struct ProgramInfo {
    string name;
    int size;
    
    ProgramInfo(string name, int size) {
        this->name = name;
        this -> size = size;
    }
};

struct Chunk {
    int startPage, endPage;
    
    Chunk( int start, int end) {
        startPage = start;
        endPage = end;
    }
};

struct UsedMemoryChunk: Chunk{
    string programName;
    
    UsedMemoryChunk(int start, int end, string name):
    Chunk (startPage, endPage){
        startPage = start;
        endPage = end;
        programName = name;
    }
};

//Memory Allocator
//user inputs for "best" or "worst" algorithms then runs through more use
//header and data fields for most of Memory Allocator
class MemoryAllocator{
    //Private data fields
private:
    string algorithm;
    LinkedList<UsedMemoryChunk> usedMemory;
    LinkedList<Chunk> freeMemory;
    //Public data fields
public:
    MemoryAllocator(string algorithm, int num_pages);
    void
    add_program(ProgramInfo prog_info);
    void
    kill_program(string programName);
    void
    defragment();
    void
    print_fragmentation();
    void
    print_memory();
};

#endif 

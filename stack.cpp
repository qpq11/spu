#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "clearBuffer.h"
#define LOG(...) fprintf(__VA_ARGS__)

FILE * logStream = stdout;

struct stack
{
    size_t capacity = 0;
    size_t size = 0;
    int* data = nullptr;     
};

const int POISON = (int) 0xDEADDEAD;
const int StackMultiplier = 2;
const int InitCapacity = 1000;
/*struct stack
{
    size_t maxsize;
    size_t top;
    int* items;     
};**/

/*int main()
{
	//struct stack* pt = newStack(78);
	struct stack* pt = (struct stack*)calloc(sizeof(struct stack), 1);
    push(pt, 1);
    push(pt, 2);
    push(pt, 3);

	char s1[] = "";
	char s2[] = "";
	scanf("%s %s\n", s1, s2);
    printf("The top element is %d\n", top(pt));
    printf("The stack size is %d\n", pt->size);
	printf("The top element is %d\n", top(pt));
    printf("The stack size is %d\n", pt->size);

    pop(pt);
    pop(pt);
    pop(pt);

    if (isEmpty(pt)) {
        printf("The stack is empty");
    }
    else {
        printf("The stack is not empty");
    }
}*/

/*struct stack* newStack(int maxsize)
{
    struct stack* pt = (struct stack*)calloc(sizeof(struct stack), 1);

    pt->capacity = maxsize;
    pt->top = -1;
    pt->items = (int*)calloc(sizeof(int), capacity);

    return pt;
}*/

void stackDtor (struct stack* pt)
{
	assert(pt != nullptr);
	pt->capacity = 0;
	//pt->top = 0;
	pt->data = nullptr;
	pt->size = 0;
}

//doesn't return the stack pointer, unlike newStack
void stackCtor (struct stack* pt)
{
	assert(pt != nullptr);
	pt->capacity = InitCapacity;
	pt->size = 0;
	pt->data = (int*) calloc(InitCapacity, sizeof(int));
}

/*int size(struct stack* pt) {
    return pt->top + 1;
}*/

int isEmpty(struct stack* pt) 
{
    return pt->size == 0;
}

int isFull(struct stack* pt) 
{
    return pt->size == pt->capacity;
}

int notHalfFull(struct stack* pt)
{
	return (pt->capacity / StackMultiplier >= pt->size);
}

void push(struct stack* pt, int x)
{
    if (isFull(pt))
    {
        LOG(logStream, "Stack overflow detected\nWill try increasing stack's capacity\n");
		stackIncrease(pt);
        //exit(EXIT_FAILURE);
    }

    LOG(logStream, "Inserting %d\n", x);

    pt->data[pt->size++] = x;
}

int top(struct stack* pt)
{
    assert(pt != nullptr);
    if (!isEmpty(pt)) {
        return pt->data[pt->size - 1];
    }
    else {
        exit(EXIT_FAILURE);
    }
}

int pop(struct stack* pt, int* x)
{
	//assert(pt != nullptr);
	assert(!isEmpty(pt));

    LOG(logStream, "Removing %d\n", top(pt));
	
    int popElem = pt->data[pt->size-1]; //e.g.: data = [1,2,3] => will make size=2 and print data[2] = 3
	pt->size--;
	pt->data[pt->size] = POISON;
	return popElem;
}

void stackDump (struct stack* pt)
{
	if (pt->data == nullptr) 
	{
		LOG (logStream, "Dump started.\n");
		LOG (logStream, " Empty stack: %17p\n", pt);
		LOG (logStream, " Size:     %10lld\n", pt->size);
		LOG (logStream, " Capacity: %10llu\n", pt->capacity);
		LOG (logStream, " Address start: nullptr\n\n");
	}
	
	else
	{
		LOG (logStream, "Dump started.\n");
		LOG (logStream, " Empty stack: %17p\n", pt);
		LOG (logStream, " Size:     %10lld\n", pt->size);
		LOG (logStream, " Capacity: %10llu\n", pt->capacity);
		LOG (logStream, " Address start: %p\n", pt->data);
		LOG (logStream, " Address   end: %p\n", pt->data + sizeof (int) * pt->capacity);
	}
	
	for (size_t i = 0; i < pt->capacity; i++)
	{
		if (pt->data[i] == POISON)
			LOG (logStream, "| stack[%7d] = NULL VALUE |\n", i);
		else
			LOG (logStream, "| stack[%7d] = %18d |\n", i, pt->data[i]);
	}
	
	LOG(logStream, "End of dump.\n");
}

int* recallocStack (struct stack* pt, const size_t capacity)
{
    assert(pt != nullptr);

    int* data = pt->data;

    if (data == nullptr) 
    {
        LOG(logStream, "RECALLOC FAILED\n");
        return nullptr;
    }

    nullValueSet (data + pt->size, capacity - pt->size);

    return data;
}

void nullValueSet (int* data, size_t size)
{   
    for (size_t i = 0; i < size; i++)
    {
        data[i] = POISON;
    }
}

void stackIncrease (struct stack* pt)
{
    if (isFull (pt))
    {
        size_t newCapacity = pt->capacity;
        newCapacity = newCapacity * StackMultiplier;
        
        int* data = recallocStack (pt, newCapacity);

        assert (data != nullptr);

        pt->data = (int*) data;
        pt->capacity = newCapacity;
    }
}

void stackDecrease(struct stack* pt)
{
	if (notHalfFull(pt))
	{
		size_t newCapacity = pt->capacity / StackMultiplier;
		int* items = recallocStack(pt, newCapacity);
		
		assert(items != nullptr);
		
		pt->data = items;
		pt->capacity = newCapacity;
	}
}
#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include "clearBuffer.h"

struct stack
{
    size_t maxsize;    // Максимальную емкость stack
    int top;        // Положение вершины stack 
    int* items;     // Указатель на массив
};

/**int main()
{
    struct stack* pt = newStack(78);

    push(pt, 1);
    push(pt, 2);
    push(pt, 3);

	char s1[] = "";
	char s2[] = "";
	scanf("%s %s\n", s1, s2);
    printf("The top element is %d\n", peek(pt));
    printf("The stack size is %d\n", size(pt));
	add(pt);
	printf("The top element is %d\n", peek(pt));
    printf("The stack size is %d\n", size(pt));

    pop(pt);
    pop(pt);
    pop(pt);

    if (isEmpty(pt)) {
        printf("The stack is empty");
    }
    else {
        printf("The stack is not empty");
    }
} */

struct stack* newStack(int capacity)
{
    struct stack* pt = (struct stack*)calloc(sizeof(struct stack), 1);

    pt->maxsize = capacity;
    pt->top = -1;
    pt->items = (int*)calloc(sizeof(int), capacity);

    return pt;
}

int size(struct stack* pt) {
    return pt->top + 1;
}

int isEmpty(struct stack* pt) {
    return size(pt) == 0;
}

int isFull(struct stack* pt) {
    return size(pt) == pt->maxsize;
}

void push(struct stack* pt, int x)
{
    if (isFull(pt))
    {
        printf("STack overflow\nProgram killed\n");
        exit(EXIT_FAILURE);
    }

    printf("Inserting %d\n", x);

    pt->items[++pt->top] = x;
}

int peek(struct stack* pt)
{
    
    if (!isEmpty(pt)) {
        return pt->items[pt->top];
    }
    else {
        exit(EXIT_FAILURE);
    }
}

int pop(struct stack* pt)
{
	assert(!isEmpty(pt));

    printf("Removing %d\n", peek(pt));

    return pt->items[pt->top--];
}

void add(struct stack* pt)
{
	int a = pop(pt);
	int b = pop(pt);
	push(pt, a+b);
	return;
}

void di(struct stack* pt)
{
	int a = pop(pt);
	int b = pop(pt);
	push(pt, a/b);
	return;
}

void sub(struct stack* pt)
{
	int a = pop(pt);
	int b = pop(pt);
	push(pt, b-a);
	return;
}

void mul(struct stack* pt)
{
	int a = pop(pt);
	int b = pop(pt);
	push(pt, a*b);
	return;
}

void in(struct stack* pt)
{
	int a = 0;
	scanf("%d", &a);
	push(pt, a);
	clearBuffer();
	return;
}
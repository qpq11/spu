#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct stack* newStack(int capacity);

void stackDtor (struct stack* pt);

void stackDump (struct stack* pt);

int* recallocStack (struct stack* const pt, const size_t capacity);

int isEmpty(struct stack* pt);

int isFull(struct stack* pt);

void push(struct stack* pt, int x);

int top(struct stack* pt);

int pop(struct stack* pt, int* x = nullptr);

int notHalfFull(struct stack* pt);

void nullValueSet (int* data, size_t size);

void stackIncrease (struct stack* pt);

void stackDecrease(struct stack* pt);

#endif
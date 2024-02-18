#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

char* MyFgets (char* str, size_t n, FILE * stream);

struct stack* newStack(int capacity);

void stackDtor (struct stack* pt);

void stackDump (struct stack* pt);

struct spu* newSpu(int capacity);

void SpuDtor(struct spu* pt);

int interpreter(int* j, struct spu* pt, int* reads);

int size(struct stack* pt);

int isEmpty(struct stack* pt);

int isFull(struct stack* pt);

void jNullify(struct stack* pt);

void push(struct stack* pt, int x);

int peek(struct stack* pt);

int pop(struct stack* pt);

void add(struct stack* pt);

void di(struct stack* pt);

void sub(struct stack* pt);

void mul(struct stack* pt);

void in(struct stack* pt);

void rpush(struct spu* pt, int regn);

void rpop(struct spu* pt, int regn);
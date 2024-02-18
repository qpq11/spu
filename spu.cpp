#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include <assert.h>

struct stack
{
    size_t maxsize;
    int top;
    int* items;
};

struct spu
{
	struct stack* stk;
	struct stack* retStk;
	int rax;
	int rbx;
	int rcx;
	int rdx;
	int jmx; //variable keeping jump with condition
};

const int minVal = -6;

const int maxVal = 66;

const int maxLen = 16; //the biggest amount of digits possible in a valid command; if surpassed, program will read the first maxLen digits as one command and then go on to the new one

const int capacity = 1000;

const int ExitCond = 1; //the "beginner" condition for a recursion; if this or lesser value appears in stack, we return to main body

enum Commands
{
	chlt = 0,
	cpush,
	cpop,
	cadd = minVal,
	csub,
	cdiv,
	cmul,
	sout,
	sin,
	regpush = 16,
	regpop,
	jmp = 33,
	jnl,
	jnm,
	pret = 65,
	pcall
};

Commands codes[] = {chlt, cpush, cpop, cadd, csub, cdiv, cmul, sout, sin, regpush, regpop, jmp, jnl, jnm, pret, pcall};

const char* texts[] = {"hlt", "push", "pop", "add", "sub", "div", "mul", "out", "in", "rpush", "rpop", "jump", "jnl", "jnm", "ret", "call"};


int main(int argc, char** argv)
{
	FILE * fp;
	if (argc>1)
		fp = fopen(argv[1], "r");
	else
		fp = fopen("asm.txt", "r");
	
	assert(fp != nullptr);
	
	struct spu* proc = newSpu(capacity);
	
	assert(proc != nullptr);
	
	int* reads;
	reads = (int*) calloc(sizeof(int), capacity);
	
	char *s;
	s = (char*) calloc(sizeof(char), maxLen);
	
	int i = 0;

	while((MyFgets(s, maxLen, fp)) != NULL)
	{
		int n = atoi(s);
		//printf("%d\n", n);
		*(reads + i) = n;
		i++;
	}
	
	int ip = 0;
	
	while(ip < i)
	{
		//printf("%d %d\n", ip, i);
		ip+= (1 + interpreter(&ip, proc, reads));
		
		//printf("ip changed to %d\n", ip);
	}
	//stackDump(proc->stk);
	SpuDtor(proc);
	return 0;
}

struct spu* newSpu(int capacity)
{
    struct spu* pt = (struct spu*)calloc(sizeof(struct spu), 1);

    pt->stk = newStack(capacity);
	pt->retStk = newStack(capacity);
    pt->rax = 0;
	pt->rbx = 0;
	pt->rcx = 0;
	pt->rdx = 0;
	pt->jmx = 0;
    return pt;
}

int interpreter(int* ip, struct spu* pt, int* reads)
{
	//printf("%d element in reads is %d\n", *(ip), reads[*(ip)]);
	assert(pt != nullptr);
	switch ((Commands) reads[*(ip)])
	{
		case chlt: 
		{
			//printf("stumbled onto halt; program terminated. position %d\n", *ip);
			break;
		}
		case cpush: 
		{
			//printf("push launched, gotta push %d\n", *(ip)+1);
			push(pt->stk, reads[*(ip)+1]);
			return 1;
		}
		case cpop: 
		{
			pop(pt->stk);
			return 0;
		}
		case cadd: 
		{
			add(pt->stk);
			return 0;
		}
		case csub: 
		{
			sub(pt->stk);
			return 0;
		}
		case cdiv: 
		{
			di(pt->stk);
			return 0;
		}
		case sout: 
		{
			printf("Printing out %d\n", peek(pt->stk));
			//pop(pt->stk);
			return 0;
		}
		case cmul:
		{
			mul(pt->stk);
			return 0;
		}
		case sin:
		{
			in(pt->stk);
			return 0;
		}
		case regpush:
		{
			//printf("rpush launched, boutta push %d into stack\n", pt->rax);
			rpush(pt, reads[*(ip)+1]);
			return 1;
		}
		case regpop:
		{
			rpop(pt, reads[*(ip)+1]);
			return 1;
		}
		case jmp:
		{
			//puts("received jump");
			*ip = reads[*(ip)+1];
			//printf("Jumping to %d\n", *ip);
			return -1;
		}
		case jnl:
		{
			if ((pt->jmx) >= -reads[*(ip)+2])
			{
				*ip = reads[*(ip)+1];
				(pt->jmx)--;
				printf("j now equals %d\n", pt->jmx);
				return 0;
			}
			return 1;
		}
		case jnm:
		{
			if ((pt->jmx) <= reads[*(ip)+2])
			{
				*ip = reads[*(ip)+1];
				(pt->jmx)++;
				printf("j now equals %d\n", pt->jmx);
				return 0;
			}
			return 1;
		}
		case pret:
		{
			//printf("ret launched! register a is now %d, goin back if it's less or equals %d\n", pt->rax, ExitCond);
			if (pt->rax > ExitCond)
			{
				*ip = peek(pt->retStk);
				//pop(pt->retStk);
				return -1;
			}
			//printf("going back to the main body\n");
			*ip = peek(pt->retStk) + 2;
			return -1;
			
		}
		case pcall:
		{
			push(pt->retStk, *ip); //return to the command right after "call x"
			*ip = reads[*(ip)+1];
			return 1;
		}
		default:
		{
			printf("unknown command: %d\n", reads[*(ip)]);
		}
	}
}

void jNullify(struct spu* pt)
{
	pt->jmx = 0;
	//printf("jump counter nullified.\n");
}

char* MyFgets (char* str, size_t n, FILE * stream)
{
	int i = 0;
	int ch;
	
	while (((ch = fgetc(stream)) != '\n') && (ch != EOF) && (ch) && (i < n) && (ch != ' '))
	{
		*(str + i++) = ch;
	}
	
	/*if (i < n)
	{
		*(str + i++) = '\n';
	}*/
	
	*(str + i) = '\0';
	
	if (ch == EOF)
		return NULL;
	
	return str;
	
}

void rpush(struct spu* pt, int regn)
{
	assert((regn > 0) && (regn < 5));
	switch(regn)
	{
		case 1:
		{
			push(pt->stk, pt->rax);
			//pt->rax = 0;
			return;
		}
		case 2:
		{
			push(pt->stk, pt->rbx);
			//pt->rbx = 0;
			return;
		}
		case 3:
		{
			push(pt->stk, pt->rcx);
			//pt->rcx = 0;
			return;
		}
		case 4:
		{
			push(pt->stk, pt->rdx);
			//pt->rdx = 0;
			return;
		}
		default:
		{
			printf("incorrect register number, try again\n");
			rpop(pt, regn);
		}
	}
}

void rpop(struct spu* pt, int regn)
{
	assert(pt != nullptr);
	assert(regn > 0);
	//printf("rpop launched, going to pop %d into register %d\n", peek(pt->stk), regn);
	switch(regn)
	{
		case 1:
		{
			pt->rax = pop(pt->stk);
			//printf("register has %d\n", pt->rax);
			return;
		}
		case 2:
		{		
			pt->rbx = pop(pt->stk);
			return;
		}
		case 3: 
		{
			pt->rcx = pop(pt->stk);
			return;
		}
		case 4: 
		{
			pt->rdx = pop(pt->stk);
			return;
		}
		default:
		{
			printf("incorrect register number, try again\n");
			rpop(pt, regn);
		}
	}
}

void SpuDtor(struct spu* pt)
{
	assert(pt != nullptr);
	stackDtor(pt->stk);
	stackDtor(pt->retStk);
	pt->rax = 0;
	pt->rbx = 0;
	pt->rcx = 0;
	pt->rdx = 0;
	pt->jmx = 0;
	free(pt);
}
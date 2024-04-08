#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include "header.h"
#include "spu.h"
#include <assert.h>
//#include "clearBuffer.h"
#include "stack.h"

struct spu
{
	struct stack* stk;
	struct stack* retStk;
	elem_t rax;
	elem_t rbx;
	elem_t rcx;
	elem_t rdx;
	//int jmx; //to operate jump with condition
	//int cmx; //flag variable
};

enum Commands
{
	chlt    =  0,
	cpush   =  1,
	cpop    = -9,
	cneg    = -8,
	csqrt   = -7,
	sadd    = -6,
	ssub    = -5,
	sdiv    = -4,
	smul    = -3,
	stout   = -2,
	stin    = -1,
	regpush = 16,
	regpop  = 17,
	jmp     = 33,
	jnl     = 34,
	jnm     = 35,
	jl      = 36,
	jm      = 37,
	je      = 38,
	jne     = 39,
	jz      = 40,
	pret    = 65,
	pcall   = 66
};

//Commands codes[] = {chlt, cpush, cpop, sadd, ssub, sdiv, smul, stout, stin, regpush, regpop, jmp, jnl, jnm, pret, pcall};

//const char* texts[] = {"hlt", "push", "pop", "add", "sub", "div", "mul", "out", "in", "rpush", "rpop", "jump", "jnl", "jnm", "ret", "call"};


int main(int argc, char** argv)
{
	FILE * fp = nullptr;
	if (argc>1)
		fp = fopen(argv[1], "r");
	else
		fp = fopen("asm.txt", "r");
	
	assert(fp != nullptr);
	
	//struct spu* proc = newSpu(InitCapacity);
	struct spu* proc = (struct spu*) calloc(sizeof(struct spu), 1);
	proc->stk    = (struct stack*) calloc(InitCapacity, 1);
	proc->retStk = (struct stack*) calloc(InitCapacity, 1);
	
	SpuCtor(proc);
	
	assert(proc != nullptr);
	
	int* reads = nullptr;
	reads = (int*) calloc(sizeof(int), InitCapacity);
	
	char *s = nullptr;
	s = (char*) calloc(sizeof(char), maxLen);
	
	int i = 0;

	while((MyFgets(s, maxLen, fp)) != NULL)
	{
		if (s[0] == '.')           //?
		{
			int n = atoi(s + 2) - 100* (maxInt + 1); //numbers of 9 digits are NOT expected to be assembled correctly (maxWord = 8), so a number being of 9 digits pretty obviously means that this isn't a NUMBER but a LABEL spotted by spu
			*(reads + i) = n;
			//printf("%d\n", n);
			i++;
			continue;
		}
		if(!strcmp("", s))
		{
			continue;
		}
		int n = atoi(s);
		//printf("%d\n", n);
		*(reads + i) = n;
		i++;
	}
	//printf("%d \n", i);
	DeLabel(reads, i-1);
	//printf("%d \n", i);
	
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

void DeLabel(int *reads, int i)
{
	while(i>=0)
	{
		int n = i;
		int lbl = *(reads + i);
		//printf("$%d %d\n", i, lbl);
		if (lbl < minInt)
		{
			*(reads + i) = i;
			//printf("$%d \n", i);
			while(n>=0)
			{
				if (lbl == *(reads + n))
					*(reads + n) = i;
				n--;
			}
		}
		i--;
	}
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
		case cneg:
		{
			neg(pt->stk);
			return 0;
		}
		case csqrt:
		{
			root(pt->stk);
			return 0;
		}
		case sadd: 
		{
			add(pt->stk);
			return 0;
		}
		case ssub: 
		{
			sub(pt->stk);
			return 0;
		}
		case sdiv: 
		{
			ddiv(pt->stk);
			return 0;
		}
		case stout: 
		{
			printf("PRINTING OUT %d\n", top(pt->stk));
			//pop(pt->stk);
			return 0;
		}
		case smul:
		{
			mul(pt->stk);
			return 0;
		}
		case stin:
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
			///puts("received jump");
			
			*ip = reads[*(ip)+1];
			///printf("Jumping to %d\n", *ip);
			return 0;
		}
		case jnl:					//[rsp] <= [rsp + 4]
		{
			//puts("JNL");
			elem_t a1 = pop(pt->stk);
			elem_t a2 = pop(pt->stk);
			//printf("%d %d\n", a1, a2);
			push(pt->stk, a2);
			push(pt->stk, a1);
			if (a1 >= a2)
			{
				*ip = reads[*(ip)+1];
				return 0;
			}
			return 1;
		}
		case jnm:
		{
			elem_t a1 = pop(pt->stk);
			elem_t a2 = pop(pt->stk);
			push(pt->stk, a2);
			push(pt->stk, a1);
			if (a1 <= a2)
			{
				*ip = reads[*(ip)+1];
				return 0;
			}
			return 1;
		}
		case jl:
		{
			elem_t a1 = pop(pt->stk);
			elem_t a2 = pop(pt->stk);
			push(pt->stk, a2);
			push(pt->stk, a1);
			if (a1 > a2)
			{
				*ip = reads[*(ip)+1];
				return 0;
			}
			return 1;
		}
		case jm:
		{
			elem_t a1 = pop(pt->stk);
			elem_t a2 = pop(pt->stk);
			push(pt->stk, a2);
			push(pt->stk, a1);
			if (a1 < a2)
			{
				*ip = reads[*(ip)+1];
				return 0;
			}
			return 1;
		}
		case je:
		{
			elem_t a1 = pop(pt->stk);
			elem_t a2 = pop(pt->stk);
			push(pt->stk, a2);
			push(pt->stk, a1);
			if (a1 == a2)
			{
				*ip = reads[*(ip)+1];
				return 0;
			}
			return 1;
		}
		case jne:
		{
			elem_t a1 = pop(pt->stk);
			elem_t a2 = pop(pt->stk);
			push(pt->stk, a2);
			push(pt->stk, a1);
			if (a1 != a2)
			{
				*ip = reads[*(ip)+1];
				return 0;
			}
			return 1;
		}
		case jz:
		{
			elem_t a1 = pop(pt->stk);
			push(pt->stk, a1);
			if (!a1)
			{
				*ip = reads[*(ip)+1];
				return 0;
			}
			return 1;
		}
		case pret:
		{
			//printf("ret launched! register a is now %d, goin back if it's less or equals %d\n", pt->rax, ExitRec);
			if (pt->rax > ExitRec)
			{
				*ip = top(pt->retStk);
				//pop(pt->retStk);
				return -1;
			}
			//printf("going back to the main body\n");
			*ip = top(pt->retStk) + 2;
			return -1;
			
		}
		case pcall:
		{
			//printf("called pcall, will save %d to retStk\n", *ip);
			push(pt->retStk, *ip); //return to the command right after "call x"
			*ip = reads[*(ip)+1];
			//printf("$$$$%d \n", *ip);
			return 0;
		}
		default:
		{
			printf("unknown command: %d\n", reads[*(ip)]);
		}
	}
}

// void jNullify(struct spu* pt)
// {
	// pt->jmx = 0;
	// //printf("jump counter nullified.\n");
// }

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
			printf("%d is an incorrect register number\n", regn);
			//rpop(pt, regn-1);
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
			printf("%d is an incorrect register number, cannot rpop there\n", regn);
			//rpop(pt, regn);
		}
	}
}

void SpuCtor(struct spu* pt)
{
	//pt->stk    = (int*) calloc(InitCapacity, sizeof(int));
	stackCtor(pt->stk);
	stackCtor(pt->retStk);
	pt->rax    = 0;
	pt->rbx    = 0;
	pt->rcx    = 0;
	pt->rdx    = 0;
	//pt->jmx    = 0;
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
	//pt->jmx = 0;
	free(pt);
}

void add(struct stack* pt)
{
	elem_t a = pop(pt);
	elem_t b = pop(pt);
	push(pt, a+b);
	return;
}

void ddiv(struct stack* pt)
{
	elem_t a = pop(pt);
	elem_t b = pop(pt);
	if (!b)
		fprintf(stdout, "Attempted division by zero. Consequences are unpredictable\n");
	push(pt, a/b);
	return;
}

void sub(struct stack* pt)
{
	elem_t a = pop(pt);
	elem_t b = pop(pt);
	push(pt, b-a);
	return;
}

void mul(struct stack* pt)
{
	elem_t a = pop(pt);
	elem_t b = pop(pt);
	push(pt, a*b);
	return;
}

void in(struct stack* pt)
{
	elem_t a = 0;
	scanf("%d", &a);
	push(pt, a);
	clearBuffer();
	return;
}

void root(struct stack* pt)
{
	elem_t a = pop(pt);
	if (a<0)
	{
		printf("CAN'T TAKE SQUARE ROOT OFF NEGATIVE NUMBERS\n");
		assert(a >= 0);
	}
	//puts("sqrt done successfully");
	push(pt, (elem_t) sqrt(a));
	return;
}

void neg(struct stack* pt)
{
	elem_t a = pop(pt);
	push(pt, -a);
	//puts("negation done");
	return;
}
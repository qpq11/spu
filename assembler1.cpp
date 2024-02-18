//БЕЗ Enter В КОНЦЕ ТЕКСТОВОГО ФАЙЛА hlt ЗАТИРАЕТСЯ ПРИ АССЕМБЛИРОВАНИИ?

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int minVal = -6;

const int maxVal = 66;

char* MyFgets (char* str, size_t n, FILE * stream);

const int CmdCnt = 16;

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

const char* regs[] = {"ax", "bx", "cx", "dx"};

int MyAssembler (char* s);

//const int maxLen = 3;

int main(int argc, char** argv)
{
	//char *w = "bx"; //98
	//printf("%d\n", w[0]); 
	FILE * fp;
	FILE * res;
	if (argc>1)
		fp = fopen(argv[1], "r");
	else
		fp = fopen("normaltext.txt", "r");
	
	char *s;
	
	//for (int j = 0; j<= (maxVal - minVal); j++)
		//printf("%d", (int) codes[j]);
	
	s = (char*) calloc(sizeof(char), 16);
	
	int cond = 0; //condition not to use the '\n' symbol
	
	res = fopen("asm.txt", "w");
	
	int wordsCount = 0; //counts the amount of array cells taken by commands or nil strings
	
	while((MyFgets(s, 8, fp)) != NULL)
	{
		printf("got %s\n", s);
		if (!strcmp(s, "org"))
		{
			int beginOrg = wordsCount;
			int endOrg = atoi(MyFgets(s, 8, fp));
			for(int i = wordsCount; i < endOrg; i++)
			{
				fprintf(res, "%d\n", 0);
				printf("now on pos %d\n", i);
			}
			wordsCount = endOrg;
			continue;
		}

		if (strlen(s) == 2)
		//Checking whether or not inputed word is a name of register
		{
			int isreg = 1;
			switch((int) s[0])
			{
				case 97:
				{
					fprintf(res, "%d\n", 1);
					cond = 0;
					break;
				}
				case 98:
				{
					fprintf(res, "%d\n", 2);
					cond = 0;
					break;
				}
				case 99:
				{
					fprintf(res, "%d\n", 3);
					cond = 0;
					break;
				}
				case 100:
				{
					fprintf(res, "%d\n", 4);
					cond = 0;
					break;
				}
				default: 
				{
					isreg = 0;
					break;
				}
			}
			wordsCount++;
			printf("assembled %d commands rn\n", wordsCount);
			if (isreg)
				continue;
		}
		int as = MyAssembler(s);
		wordsCount++;
		//printf("turned into %d\n", as);
		if ((as>0) && (!cond))
		{
			fprintf(res, "%d ", as);
			cond = 1;
		}
		else
		{
			fprintf(res, "%d\n", as);
			cond = 0;
		}
		
		printf("%d ", MyAssembler(s));
		//printf("from %s\n", s);
		printf("cond = %d\n", cond);
		printf("assembled %d commands rn\n", wordsCount);
	}
	free(s);
	fclose(fp);
	int i = 0;
	return 0;
}

int MyAssembler(char* s)
{
	for (int i = 0; i < CmdCnt; i++)
	{	
		//printf("i= %d %s %s %d w %d \n", i, texts[i], s, codes[i], strcmp(texts[i], s));
		//printf("lens %d %d \n", sizeof(texts[i]), sizeof(s));
		if (!strcmp(texts[i], s))
		{
			//printf("n %d\n", (int) codes[i]);
			return codes[i];
		}
	}
	//printf("not a command\n");
	return atoi(s);
}

/**
* Works like fgets() but also stops on each space symbol
*/
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
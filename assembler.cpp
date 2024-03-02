//БЕЗ Enter В КОНЦЕ ТЕКСТОВОГО ФАЙЛА hlt ЗАТИРАЕТСЯ ПРИ АССЕМБЛИРОВАНИИ!!! фикс asap

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

const int minVal = -6;

const int maxVal = 66;

char* MyFgets (char* str, size_t n, FILE * stream);

const int CmdCnt = 16;

const int maxWord = 8; //max amount of symbols taken by MyFgets

enum Commands
{
	chlt    =  0,
	cpush   =  1,
	cpop    =  2,
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
	pret    = 65,
	pcall   = 66
};

struct Codewords
{
	const char text[maxWord];   //will work, but is not entirely correct as the max length of a command is less than maxLen
	Commands code;
};

//Commands codes[] = {chlt, cpush, cpop, sadd, ssub, sdiv, smul, stout, stin, regpush, regpop, jmp, jnl, jnm, pret, pcall};

//const char* texts[] = {"hlt", "push", "pop", "add", "sub", "div", "mul", "out", "in", "rpush", "rpop", "jump", "jnl", "jnm", "ret", "call"};

const struct Codewords CodeArray[] = {  //CodeArray[CmdCnt]
	{"hlt",  chlt    },
	{"push", cpush   },
	{"pop",  cpop    },
	{"add",  sadd    },
	{"sub",  ssub    },
	{"div",  sdiv    },
	{"mul",  smul    },
	{"out",  stout   },
	{"in",   stin    },
	{"rpush", regpush},
	{"rpop", regpop  },
	{"jump", jmp     },
	{"jnl",  jnl     },
	{"jnm",  jnm     },
	{"ret",  pret    },
	{"call", pcall   }
};
	

const char* regs[] = {"ax", "bx", "cx", "dx"}; //registers for common use

int MyOrg(int wordsCount, int endOrg, FILE * res);

int IsReg(const char* s, FILE * res);

int MyAssembler (char* s);

void FileEndOnSlashN (FILE * fp);

const int maxLen = 18; //size of a buffer; is made much bigger than maxWord in order to avoid overflows

int main(int argc, char** argv)
{
	//char *w = "bx"; //98
	//printf("%d\n", w[0]); 
	FILE * fp = nullptr;
	FILE * res = nullptr;
	if (argc>1)
		fp = fopen(argv[1], "a+");
	else
		fp = fopen("normaltext.txt", "a+");
	assert(fp != nullptr);
	
	FileEndOnSlashN(fp);
	
	fclose(fp);
	
	if (argc>1)
		fp = fopen(argv[1], "r");
	else
		fp = fopen("normaltext.txt", "r");
	assert(fp != nullptr);
	
	char *s;
	
	//for (int j = 0; j<= (maxVal - minVal); j++)
		//printf("%d", (int) codes[j]);
	
	s = (char*) calloc(sizeof(char), maxLen);
	
	int cond = 0; //condition not to use the '\n' symbol
	
	res = fopen("asm.txt", "w");
	
	assert(fp != nullptr);
	
	int wordsCount = 0; //counts the amount of array cells taken by commands or nil strings
	
	while((MyFgets(s, 8, fp)) != NULL)
	{
		printf("got %s\n", s);
		if (!strcmp(s, "org"))
		{
			/*int beginOrg = wordsCount;
			int endOrg = atoi(MyFgets(s, 8, fp));
			for(int i = wordsCount; i < endOrg; i++)
			{
				fprintf(res, "%d\n", 0);
				printf("now on pos %d\n", i);
			}
			wordsCount = endOrg;
			continue;*/
			int endOrg = atoi(MyFgets(s, 8, fp));
			wordsCount = MyOrg(wordsCount, endOrg, res);
		}

		if (strlen(s) == 2)
		{
			int isregister = IsReg(s, res); //Checking whether or not inputed word is a name of register
			wordsCount++;
			printf("assembled %d commands rn\n", wordsCount);
			if (isregister)
			{
				cond = 0;
				continue;
			}
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

void FileEndOnSlashN (FILE * fp)
{
	char lastChar = '\0';
	
	fseek(fp, 0, SEEK_END);
	
	if (ftell(fp))
	{
		fseek(fp, -1, SEEK_END);
		lastChar = fgetc(fp);
		printf("last char is %d\n", lastChar);
		fseek(fp, 0, SEEK_END);
	}
	
	if (lastChar != '\n')
	{
		fprintf(fp, "\n");
		printf("added \\n to end of file \n");
	}
	else
		printf("already ended on \\n \n");
	return;
}

int MyOrg (int wordsCount, int endOrg, FILE * res)
{
	int beginOrg = wordsCount;
	//int endOrg = atoi(MyFgets(s, 8, fp));
	for(int i = beginOrg; i < endOrg; i++)
	{
		fprintf(res, "%d\n", 0);
		printf("now on pos %d\n", i);
	}
	return endOrg;
}

int IsReg(const char* s, FILE * res)
{
	int flag = 1;
	/*switch((int) s[0])
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
				flag = 0;
				break;
			}
	}*/
	if ((int) s[0] >= 97 && (int) s[0] <=100)
		fprintf(res, "%d\n", ((int) s[0]) - 96);
	else
		flag = 0;
	return flag;
}

int MyAssembler(char* s)
{
	for (int i = 0; i < CmdCnt; i++)
	{	
		//printf("i= %d %s %s %d w %d \n", i, texts[i], s, codes[i], strcmp(texts[i], s));
		//printf("lens %d %d \n", sizeof(texts[i]), sizeof(s));
		/*if (!strcmp(texts[i], s))
		{
			//printf("n %d\n", (int) codes[i]);
			return codes[i];
		}*/
		if (!strcmp(CodeArray[i].text, s))
		{
			//printf("n %d\n", (int) CodeArray[i]->code);
			return CodeArray[i].code;
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
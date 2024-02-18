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

const int MaxLen = 12;

Commands codes[] = {chlt, cpush, cpop, cadd, csub, cdiv, cmul, sout, sin, regpush, regpop, jmp, jnl, jnm, pret, pcall};

const char* texts[] = {"hlt", "push", "pop", "add", "sub", "div", "mul", "out", "in", "rpush", "rpop", "jump", "jnl", "jnm", "ret", "call"};

const char* MyDisassembler (int n, int cond);

int main(int argc, char** argv)
{
	FILE * fp;
	FILE * res;
	fp = fopen("asm.txt", "r");
	
	int cond = 0; //condition not to use the '\n' symbol
	
	res = fopen("disasm.txt", "w");
	
	char *s;
	s = (char*) calloc(sizeof(char), MaxLen);
	
	const char* dis;
	
	while((MyFgets(s, MaxLen, fp)) != NULL)
	{
		int n = atoi(s);
		printf("%d ", n);
		//char *buf;
		//buf = (char*) calloc(sizeof(char), 16);
		dis = MyDisassembler(n, cond);
		//free(buf);
		printf("%s\n", dis);
		if ((n>0) && (!cond))
		{
			fprintf(res, "%s ", dis);
			cond = 1;
		}
		else if (cond)
		{
			fprintf(res, "%d\n", n);
			cond = 0;
		}
		else
		{
			fprintf(res, "%s\n", dis);
			cond = 0;
		}
	}
	fclose(fp);
	free(s);
	//int i = 0;
}

const char* MyDisassembler(int n, int cond)
{
	if (cond)
	{
		return "";
	}
	else
	{
		switch((Commands) n)
		{
			case chlt: return "hlt";
			case cpush: return "push";
			case cpop: return "pop";
			case cadd: return "add";
			case csub: return "sub";
			case cmul: return "mul";
			case cdiv: return "di";
			case sout: return "out";
			case sin: return "in";
			case regpush: return "rpush";
			case regpop: return "rpop";
			case jmp: return "jump";
			case jnl: return "jnl";
			case jnm: return "jnm";
			case pret: return "ret";
			case pcall: return "call";
			default: puts("boi what the hell boi");
		}
	}
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
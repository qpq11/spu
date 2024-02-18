/*clearBuffer.h- clears the input buffer; works like fflush() but for input stream*/
void clearBuffer(void);

void clearBuffer(void)
{
	int ch;
	while(((ch=getchar()) != '\n') && (ch != EOF)){
		continue;
	}
}
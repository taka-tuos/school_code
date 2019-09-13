#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *gen;
int tn = 0;
char *pg;

void putcode(const char *s)
{
	for (int i = 0; i < tn; i++) fputc('\t', gen);
	fprintf(gen, "%s\n", s);
}

void optimize(int ch, char *opt, char *norm)
{
	if(pg[1] == ch) {
		int n = 0;
		while(*pg == ch) pg++, n++;
		pg--;
		char s[32];
		sprintf(s,opt,n);
		putcode(s);
	} else {
		putcode(norm);
	}
}

int main(int argc, char *argv[])
{
	if(argc < 3) {
		puts("too few argments");
		return 2;
	}
	
	FILE *fp = fopen(argv[1], "rb");

	size_t siz;

	fseek(fp, 0, SEEK_END);
	siz = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	pg = (char *)malloc(siz);

	fread(pg, 1, siz, fp);

	gen = fopen(argv[2], "wt");

	putcode("#include <stdio.h>");
	putcode("#include <stdlib.h>");
	putcode("#include <string.h>");
	putcode("int main(void) {");

	tn++;

	putcode("long *bs = (long *)malloc(131072 * sizeof(long));");
	putcode("long *bp = bs;");
	putcode("memset(bs, 0, 130172 * sizeof(long));");

	while (*pg) {
		char c = *pg;
		if (c == '[') {
			putcode("while(*bp) {");
			tn++;
		}
		else if (c == ']') {
			tn--;
			putcode("}");
		}
		else if (c == '>') {
			optimize('>', "bp+=%d;", "bp++;");
		}
		else if (c == '<') {
			optimize('<', "bp-=%d;", "bp--;");
		}
		else if (c == '+') {
			optimize('+', "(*bp)+=%d;", "(*bp)++;");
		}
		else if (c == '-') {
			optimize('-', "(*bp)-=%d;", "(*bp)--;");
		}
		else if (c == '.') {
			putcode("putchar(*bp);");
		}
		else if (c == ',') {
			putcode("*bp = getchar();");
		}

		pg++;
	}

	putcode("return 0;");
	tn--;

	putcode("}");

	fclose(gen);

}

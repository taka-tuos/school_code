#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define optimize(ch, opt, norm) \
{ \
	if(pg[1] == ch) { \
		int n = 0; \
		while(*pg == ch) pg++, n++; \
		pg--; \
		opt n; \
	} else { \
		norm; \
	} \
} \

int main(int argc, char *argv[])
{
	if(argc < 2) {
		puts("too few argments");
		return 2;
	}
	
	long *bp;
	long *bs = (long *)malloc(131072 * sizeof(long));

	memset(bs, 0, 131072 * sizeof(long));

	FILE *fp = fopen(argv[1], "rb");

	size_t siz;

	fseek(fp, 0, SEEK_END);
	siz = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char *pg = (char *)malloc(siz);
	char *opg = pg;

	fread(pg, 1, siz, fp);

	bp = bs;

	while((size_t)(pg-opg) < siz) {
		char c = *pg;
		if (c == '[') {
			if (*bp == 0) {
				int bracket = 1;
				do {
					pg++;
					if (*pg == '[') bracket++;
					if (*pg == ']') bracket--;
				} while (bracket != 0);
			}
		}
		else if (c == ']') {
			if (*bp != 0) {
				int bracket = 0;
				do {
					if (*pg == '[') bracket++;
					if (*pg == ']') bracket--;
					pg--;
				} while (bracket != 0);
			}
		}
		else if (c == '>') {
			//bp++;
			//optimize('>',bp+=,bp++);
			if(pg[1] == '>') {
				int n = 0;
				while(*pg == '>') pg++, n++;
				pg--;
				bp+=n;
			} else {
				bp++;
			}
		}
		else if (c == '<') {
			//bp--;
			//optimize('<',bp-=,bp--);
			if(pg[1] == '<') {
				int n = 0;
				while(*pg == '<') pg++, n++;
				pg--;
				bp-=n;
			} else {
				bp--;
			}
		}
		else if (c == '+') {
			//(*bp)++;
			//optimize('+',(*bp)+=,bp++);
			if(pg[1] == '+') {
				int n = 0;
				while(*pg == '+') pg++, n++;
				pg--;
				(*bp)+=n;
			} else {
				(*bp)++;
			}
		}
		else if (c == '-') {
			//(*bp)--;
			//optimize('-',(*bp)-=,bp--);
			if(pg[1] == '-') {
				int n = 0;
				while(*pg == '-') pg++, n++;
				pg--;
				(*bp)-=n;
			} else {
				(*bp)--;
			}
		}
		else if (c == '.') {
			putchar(*bp);
		}
		else if (c == ',') {
			*bp = getchar();
		}

		pg++;
	}
}

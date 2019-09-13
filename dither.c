#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include <stdint.h>

int bayerpattern[16] = {
	  0,  8,  2, 10,
	 12,  4, 14,  6,
	  3, 11,  1,  9,
	 15,  7, 13,  5
};

int bayerpattern8[64] = {
	 1, 33,  9, 41,  3, 35, 11, 43,
	49, 17, 57, 25, 51, 19, 59, 27,
	13, 45,  5, 37, 15, 47,  7, 39,
	61, 29, 53, 21, 63, 31, 55, 23,
	 4, 36, 12, 44,  2, 34, 10, 42,
	52, 20, 60, 28, 50, 18, 58, 26,
	16, 48,  8, 40, 14, 46,  6, 38,
	64, 32, 56, 24, 62, 30, 54, 22,
};

uint8_t colorpalette[16*3] = {
	0x00, 0x00, 0x00,
	0xAA, 0x00, 0x00,
	0x00, 0xAA, 0x00,
	0xAA, 0xAA, 0x00,
	0x00, 0x00, 0xAA,
	0xAA, 0x00, 0xAA,
	0x00, 0xAA, 0xAA,
	0xAA, 0xAA, 0xAA,
	0x55, 0x55, 0x55,
	0xFF, 0x55, 0x55,
	0x55, 0xFF, 0x55,
	0xFF, 0xFF, 0x55,
	0x55, 0x55, 0xFF,
	0xFF, 0x55, 0xFF,
	0x55, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF,
};

int *nearcolor(int a, int b, int c)
{
	int minl, mini = 0;
	
	minl = 16777216;
	
	for(int i = 0; i < 16; i++) {
		int pa = colorpalette[i*3+0];
		int pb = colorpalette[i*3+1];
		int pc = colorpalette[i*3+2];
		
		int al = pa - a;
		int bl = pb - b;
		int cl = pc - c;
		
		int len = al*al + bl*bl + cl*cl;
		
		if(len < minl) {
			minl = len;
			mini = i;
		}
	}
	
	//printf("%d,%d\n",mini,minl);
	
	return colorpalette + mini*3;
}

int main(void)
{
	int w,h,bpp;
	stbi_uc *in = stbi_load("base.png",&w,&h,&bpp,4);
	
	printf("%dx%d %dbpp\n",w,h,bpp*8);
	
	stbi_uc *in2 = (stbi_uc *)malloc(w*(h/2)*4);
	stbi_uc *out = (stbi_uc *)malloc(w*h*4);
	
	for(int x = 0; x < w; x++) {
		for(int y = 0; y < h; y+=2) {
			uint32_t *p = ((uint32_t *) in) + (y*w+x);
			uint32_t *q = ((uint32_t *)in2) + ((y/2)*w+x);
			
			int a1 = *p & 0xff;
			int b1 = ((*p) >> 8) & 0xff;
			int c1 = ((*p) >> 16) & 0xff;
			
			int a2 = p[w] & 0xff;
			int b2 = ((p[w]) >> 8) & 0xff;
			int c2 = ((p[w]) >> 16) & 0xff;
			
			*q = ((a1+a2)/2) | (((b1+b2)/2) << 8) | (((c1+c2)/2) << 16) | 0xff000000;
		}
	}
	
	for(int x = 0; x < w; x++) {
		for(int y = 0; y < h/2; y++) {
			uint32_t *p = ((uint32_t *)in2) + (y*w+x);
			uint32_t *q = ((uint32_t *)out) + ((2*y)*w+x);
			
			*q = 0xff000000;
			
			int a = *p & 0xff;
			int b = ((*p) >> 8) & 0xff;
			int c = ((*p) >> 16) & 0xff;
			
			//int bias = bayerpattern[(y%4)*4 + (x%4)]*4;
			int bias = bayerpattern8[(y%8)*8 + (x%8)];
			//int bias = 0;
			
			int *r = nearcolor(a+bias,b+bias,c+bias);
			
			*q |= *r;
			
			q[w] = *q;
		}
	}
	
	stbi_write_png("out.png",w,h,4,out,0);
	
	return 0;
}

#ifndef _UTILS_H
#define _UTILS_H
void initPalette(void);
void setPalette(int start, int end, unsigned char *rgb);
void initBackground(char *vram, int x, int y);
void fillBox(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);

void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(char *vram, int vxsize, int pxsize,
                 int pysize, int px0, int py0, char *buf, int bxsize);
unsigned int getStringSize(char *c);
int String2Int(char *str);
#endif
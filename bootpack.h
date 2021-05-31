//basic
#define true 1
#define false 0

//color
#define COL8_000000 0
#define COL8_FF0000 1
#define COL8_00FF00 2
#define COL8_FFFF00 3
#define COL8_0000FF 4
#define COL8_FF00FF 5
#define COL8_00FFFF 6
#define COL8_FFFFFF 7
#define COL8_C6C6C6 8
#define COL8_840000 9
#define COL8_008400 10
#define COL8_848400 11
#define COL8_000084 12
#define COL8_840084 13
#define COL8_008484 14
#define COL8_848484 15
#define COL_TRANSPARENT 50

//boot
struct BootInfo
{
    char cyls, leds, vmode, reserve;
    short screenX, screenY;
    char *vram;
};

/* naskfunc.nas */
void io_hlt(void);
void io_cli(void);
void io_sti(void);
void io_stihlt(void);
int io_in8(int port);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);
int load_cr0(void);
void store_cr0(int cr0);
void asm_inthandler21(void);
void asm_inthandler27(void);
void asm_inthandler2c(void);
unsigned int memtest_sub(unsigned int start, unsigned int end);

void initPalette(void);
void setPalette(int start, int end, unsigned char *rgb);
void initBackground(char *vram, int x, int y);
void fillBox(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);

void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(char *vram, int vxsize, int pxsize,
                 int pysize, int px0, int py0, char *buf, int bxsize);

struct BootInfo *getBootInfo();
struct MemoryManager *getMemoryManager();
void logger(char *c, unsigned int y);

//sheet.c

#define SHEET_MAX 512

#define FIX_UNSET 0
#define FIX_TOP 1
#define FIX_BOTTOM 2

#define TRANSPARENT_INDEX -1

struct Sheet
{
    //自身基本属性
    short width, height;
    short x, y;
    char status; //高1位fixTop 高2位fixBottom
    char *vram;

    //对于兄弟图层
    struct Sheet *fatherSheet;
    struct Sheet *nextSheet;     //nextSheet下一层
    struct Sheet *previousSheet; //previousSheet上一层
    unsigned int index;

    //对于子图层
    struct Sheet *topSheet;
    struct Sheet *bottomSheet;
    int *indexMap;
    char *updateMap;

    //子图层内存管理
    struct Sheet *sheetStore[SHEET_MAX];

    //备注
    unsigned int series;

    // struct SheetManager *subSheetManager;
};

// typedef struct SheetManager
// {
//     struct Sheet *topSheet;
//     struct Sheet *bottomSheet;
//     struct Sheet sheets[SHEET_MAX];
//     struct Sheet *fatherSheet;
// };

// void initRootSheetManager(struct SheetManager **sheetManager);
// void createSubSheetManager(struct Sheet **fatherSheet);
// struct Sheet *getUnusedSheetFromSheetManager(struct SheetManager *sheetManager);
// struct Sheet *createSheetToTop(struct SheetManager **sheetManager, short x1, short y1, short width, short height);
// void releaseSheet(struct SheetManager *sheetManager, struct Sheet *sheet);
struct Sheet *initRootSheet();
struct Sheet *createSubsheetToTop(struct Sheet *fatherSheet, short x, short y, short width, short height);
struct Sheet *createSubsheetToTopWithVram(struct Sheet *fatherSheet, short x, short y, short width, short height, char *vram);
void updateSheet(struct Sheet *sheet);
void fillVram(struct Sheet *sheet, unsigned char c);
void setBitInUpdateMap(struct Sheet *sheet, unsigned int index, unsigned int value);
void forceUpdateSheet(struct Sheet *sheet);
char getFixedTop(struct Sheet *sheet);
char getFixedBottom(struct Sheet *sheet);
void setFixedTop(struct Sheet *sheet);
void setFixedBottom(struct Sheet *sheet);
void printInSheet(struct Sheet *sheet, short x, short y, char *c, unsigned int colorConstant);
void fillInSheet(struct Sheet *sheet, short x, short y, short width, short height, unsigned int colorConstant);
void updateAllSubsheetWithFather(struct Sheet *sheet, struct Sheet *fatherSheet);
void updateAllSubsheet(struct Sheet *sheet);
void updateSingleSheetIndexAndVramInFatherSheet(struct Sheet *sheet);
void updateSheetIndexMap(struct Sheet *sheet);
void fillVramByIndexMap(struct Sheet *sheet);

// void createSubSheetManager(struct Sheet **fatherSheet);

//memory.c
#define MEMMAN_ADDR 0x003c0000
struct MemoryBlock
{
    unsigned int addrFrom;
    unsigned int addrTo;
    struct MemoryBlock *nextBlock;
    unsigned int blockType;

    unsigned int requestSize;
    unsigned int taskId;
    unsigned int isUsing;
};

struct MemoryManager
{
    struct MemoryBlock *firstBlock;
    struct MemoryBlock blocks[2048];
};

void initMemoryManage(struct MemoryManager *memoryManager);
void initMemoryManager(struct MemoryManager *memoryManager, unsigned int start, unsigned int end);
unsigned int allocaMemory(struct MemoryManager *memoryManager, unsigned int size);
struct MemoryBlock *getUnusedBlock(struct MemoryManager *memoryManager);
void combineMemoryBlock(struct MemoryManager *memoryManager);
unsigned int getPreferBlockType(struct MemoryManager *memoryManager, unsigned int size);
unsigned int getUnusedMemoryTotal(struct MemoryManager *memoryManager);
int releaseBlock(struct MemoryManager *memoryManager, unsigned int addr);
struct MemoryBlock *cutBlock(struct MemoryBlock **lastReturnBlock, unsigned int fromAddr, int toAddr);
unsigned int getMaxBlockTypeInMemory(struct MemoryManager *memoryManager, unsigned int size);
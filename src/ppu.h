#ifndef _FCEU_PPU_H
#define _FCEU_PPU_H

#ifndef __cplusplus
#if defined(_MSC_VER) && _MSC_VER < 1800 && !defined(SN_TARGET_PS3)
/* Hack applied for MSVC when compiling in C89 mode
 * as it isn't C99-compliant. */
#define bool unsigned char
#define true 1
#define false 0
#else
#include <stdbool.h>
#endif
#endif

void FCEUPPU_Init(void);
void FCEUPPU_Reset(void);
void FCEUPPU_Power(void);
int FCEUPPU_Loop(int skip);

void FCEUPPU_LineUpdate();
void FCEUPPU_SetVideoSystem(int w);

extern void (*GameHBIRQHook)(void), (*GameHBIRQHook2)(void);
extern void FP_FASTAPASS(1) (*PPU_hook)(uint32 A);

/* For cart.c and banksw.h, mostly */
extern uint8 NTARAM[0x800], *vnapage[4];
extern uint8 PPUNTARAM;
extern uint8 PPUCHRRAM;
extern uint8 PALRAM[0x20];

void FCEUPPU_SaveState(void);
void FCEUPPU_LoadState(int version);

extern int scanline;
extern uint8 PPU[4];

#define WIDTH 33
#define HEIGHT 31
#define SPRITE_COUNT 64

#pragma pack( push, 1 )
typedef struct
{
    int64_t low, high; //data
    short x, y; // real position
    int32_t index; // position in grid
    bool flipX, flipY;
    bool hidden;
    uint8_t palette; // palete index
    bool isBg;
} TileData;
#pragma pack( pop )

bool empty(TileData* data);
#define SIZE (WIDTH * HEIGHT * 1 + SPRITE_COUNT * 2)
extern TileData tile_buffer[WIDTH * HEIGHT + SPRITE_COUNT * 2];
extern int32_t tile_count;

#endif

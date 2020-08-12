#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <icore.h>

#include "../../fceu.h"
#include "../../fceu-endian.h"
#include "../../input.h"
#include "../../state.h"
#include "../../ppu.h"
#include "../../cart.h"
#include "../../x6502.h"
#include "../../git.h"
#include "../../palette.h"
#include "../../sound.h"
#include "../../file.h"
#include "../../cheat.h"
#include "../../ines.h"
#include "../../unif.h"
#include "../../fds.h"
#include "../../vsuni.h"
#include "../../video.h"

#define MAX_PORTS 2 

typedef struct
{
   unsigned nes3d;
   unsigned fceux;
} input_map_t;

static const input_map_t input_map[] = {
   { A_3DNes, JOY_A },
   { B_3DNes, JOY_B },
   { A_3DNes, JOY_SELECT },
   { Start_3DNes, JOY_START },
   { Up_3DNes, JOY_UP },
   { Down_3DNes, JOY_DOWN },
   { Left_3DNes, JOY_LEFT },
   { Right_3DNes, JOY_RIGHT },
};

/* Some timing-related variables. */
extern unsigned sndsamplerate;
extern unsigned sndquality;
extern unsigned sndvolume;
extern unsigned swapDuty;

extern int32_t tile_count;

static GetJoypadStateCallback get_input_cb = NULL;
static RenderVideoCallback render_video_cb = NULL;
static int32_t *_sound = 0;
static uint32_t input = 0;
static int32_t frame;
#define BUFFER_SIZE 1000000
uint8_t *buffer;

void core_get_video_info(VideoInfo* info)
{
	info->width = WIDTH;
	info->height = HEIGHT;
	info->spCount = SPRITE_COUNT * 2;
}

void core_set_input_callback(GetJoypadStateCallback t)
{
	get_input_cb = t;
}

void core_set_video_callback(RenderVideoCallback t)
{
	render_video_cb = t;
}

void core_init(void)
{
	frame = 0;
	buffer = (uint8_t*)malloc(BUFFER_SIZE);
}

void core_de_init(void)
{	
	FCEUI_CloseGame();
	FCEUI_Sound(0);
	FCEUI_Kill();
	free(buffer);	
}

void core_set_speed(float speed)
{
   FCEUI_Sound((int)sndsamplerate / speed);
}

int32_t core_get_frame_counter(void)
{
	return frame;
}

void core_set_frame_counter(int new_frame)
{
	frame = new_frame;
}
 
int32_t core_read_audio_data(float* data, int32_t size)
{
}

void core_reset()
{
	ResetNES();
	frame = 0;
}

void power()
{
	PowerNES();
	frame = 0;
}

void core_run_frame(int inputs, int32_t** pSound, int32_t* sound_size, TileData ** pTiles, int32_t* tileCount, int8_t** palette)
{
   unsigned i;
   uint8_t *gfx;
   int32_t ssize = 0;
   
	int button = (inputs >> 16) & 0xFF;
	if (button == 1)
		core_reset();
	else if (button == 2)
		power();
   frame++;  
   input = inputs & 0xFFFF;
	tile_count = 0;
   FCEUI_Emulate(&gfx, pSound, sound_size, 0);
	*tileCount = tile_count;
	*pTiles = tile_buffer;
	*palette = (int8_t*)PALRAM;
}

bool core_load_rom(uint8_t* data, int32_t size)
{
   unsigned i, j;
	
   power();

   FCEUI_Initialize();
   FCEUI_SetSoundVolume(sndvolume);
   FCEUI_Sound(sndsamplerate);
	
	frame = 0;

   GameInfo = (FCEUGI*)FCEUI_LoadGame("", data, size);

   if (!GameInfo)
   {
      struct retro_message msg;
      char msg_local[256];

      sprintf(msg_local, "ROM loading failed...");
      return false;
   }

   for (i = 0; i < MAX_PORTS; i++)
		FCEUI_SetInput(i, SI_GAMEPAD, &input, 0);

   FCEUI_DisableFourScore(1);

   return true;
}

void core_close_rom(void)
{
}

uint8_t core_peek_memory(unsigned short addr)
{
	return RAM[addr & 0x7FF];
}

void core_write_memory(unsigned short addr, uint8_t value)
{
	RAM[addr & 0x7FF] = value;
}

int32_t core_save_state(uint8_t** data)
{
	*(int*)buffer = frame;
   memstream_set_buffer((uint8_t*) buffer + sizeof(int), BUFFER_SIZE - 4);
   FCEUSS_Save_Mem();	
	*data = buffer;
	return memstream_get_last_size() + sizeof(int);
}

int32_t core_load_state(uint8_t* data, int32_t size, int index)
{
	int s = sizeof(int);
	if (size < retro_serialize_size() + s)
		return -1;
	frame = *(int*)(data + index);
   memstream_set_buffer((uint8_t*)(data + index + s), retro_serialize_size());
   FCEUSS_Load_Mem();
	return retro_serialize_size() + s;
}

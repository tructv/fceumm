#ifndef ICORE_H__
#define ICORE_H__

#include <libretro.h>
#include "../../ppu.h"

#ifdef __cplusplus
extern "C" {
#endif

enum NesButton
{
        A_3DNes = 1,
        B_3DNes = 2,
        Select_3DNes = 4,
        Start_3DNes = 8,
        Up_3DNes = 16,
        Down_3DNes = 32,
        Left_3DNes = 64,
        Right_3DNes = 128,
        ZoomIn_3DNes = 256,
        ZoomOut_3DNes = 512,
        RotLeft_3DNes = 1024,
        RotRight_3DNes = 2048,
        RotUp_3DNes = 4096,
        RotDown_3DNes = 8192,
        Menu_3DNes = 16384
 };
 
typedef struct 
{
    int32_t width; // background tile per line
    int32_t height; // background tile count per row
    int32_t spCount; // sprite count per frame
} VideoInfo;

typedef int32_t (RETRO_CALLCONV *GetJoypadStateCallback) (int32_t index);

typedef void (RETRO_CALLCONV *RenderVideoCallback) (TileData* data, int64_t count, int32_t* palette, int64_t maskX, int64_t maskY);

RETRO_API void core_set_input_callback(GetJoypadStateCallback t);

RETRO_API void core_set_video_callback(RenderVideoCallback t);

RETRO_API void core_get_video_info(VideoInfo* info);

RETRO_API void core_init(void);

RETRO_API void core_de_init(void);

RETRO_API void core_set_speed(float speed); // how fast emulator run compared to default speed

RETRO_API void core_run_frame(int inputs, int32_t** pSound, int32_t* sound_size, TileData ** pTiles, int32_t* tileCount, int8_t** palette);

RETRO_API int32_t core_get_frame_counter(void);

RETRO_API void core_set_frame_counter(int new_frame);

RETRO_API int32_t core_read_audio_data(float* data, int32_t size);

RETRO_API void core_reset(void);

RETRO_API bool core_load_rom(uint8_t* data, int32_t size);

RETRO_API void core_close_rom(void);

RETRO_API uint8_t core_peek_memory(unsigned short add);

RETRO_API void core_write_memory(unsigned short add, uint8_t value);

RETRO_API int32_t core_save_state(uint8_t** data);

RETRO_API int32_t core_load_state(uint8_t* data, int32_t size, int index);

#ifdef __cplusplus
}
#endif

#endif
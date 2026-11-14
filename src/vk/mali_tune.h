/*
 * SPDX-FileCopyrightText: 2026 ARMSX2 and bmdhacks
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <stdint.h>

/*
 * Switches for measuring what an optimisation is worth on a game, and
 * diagnostics: the comma-separated words of MALISX2_TUNE or, on Android,
 * the property debug.malisx2.tune, read when a device is created (one
 * device a process, like mali_crc_options). Unset, every optimisation is
 * on and every diagnostic off. The words that change compiled shaders are
 * part of the shaders' cache keys.
 *   noubopush    no uniform-buffer words pushed into FAU (v7 lower_ubo_push)
 *   noloadalign  v7: 4-byte aligned vec4 global loads split into four
 *                32-bit loads, as the compiler's alignment rule says
 *   nocrc        no transaction elimination (mali_crc_options.disable)
 * and ones that are off unless named:
 *   passlog      v7 diagnostic: every 20 s, the passes of the next ~150 ms
 *                with their draws' shaders, blending and vertex counts, and
 *                every 2 s the shader compiles, in logcat
 *   deplog       diagnostic: every 2 s, how many vtc atoms had to wait for
 *                fragment work still running, and why (mali_jm_queue.c),
 *                and the barriers that made them (mali_jm_cmd_buffer.c)
 *   shaderstats  diagnostic: each shader read back from the pipeline cache
 *                logged with its code size, registers, thread storage
 *                (spills) and interface counts
 *   sbslots      v7: Bifrost scoreboard slots by kind (bi_scoreboard.c):
 *                a warp's loads, textures and varyings no longer wait for
 *                each other unless they must; unset, Mesa's scheme (every
 *                varying load, memory load, store and atomic one at a time
 *                on slot 0). Sonic's attract sequence: no measured change
 *                (2026-11-14)
 *   idvs         v7: application pipelines also get their vertex shader
 *                split for index-driven vertex shading, and a direct draw
 *                of at least MALI_V7_IDVS_MIN_RANGE vertices runs it as an
 *                Indexed Vertex job (mali_jm_cmd_draw_v7.c); smaller,
 *                indirect and rasterizer-discard draws keep the plain one
 */
enum mali_tune_off {
   MALI_TUNE_NO_UBO_PUSH = 1u << 0,
   MALI_TUNE_NO_LOAD_ALIGN = 1u << 1,
   MALI_TUNE_NO_CRC = 1u << 2,
   MALI_TUNE_PASSLOG = 1u << 3,
   MALI_TUNE_DEPLOG = 1u << 4,
   MALI_TUNE_SHADERSTATS = 1u << 5,
   MALI_TUNE_IDVS = 1u << 6,
   MALI_TUNE_SB_SLOTS = 1u << 7,
};
extern uint32_t mali_tune_off __attribute__((visibility("hidden")));
/* Read the words and set mali_tune_off and mali_crc_options.disable
 * (mali_image.c). */
void mali_tune_init(void);

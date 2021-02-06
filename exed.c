#include <SDL2/SDL.h>
#include <stdio.h>

/* 
Copyright (c) 2020 Devine Lu Linvega

Permission to use, copy, modify, and distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE.
*/

#define HOR 32
#define VER 16
#define PAD 2
#define SZ 512 * 16

typedef unsigned char Uint8;

typedef struct {
	int unsaved;
	char name[256];
	Uint8 data[SZ];
} Document;

typedef struct Brush {
	int x, y, i, swap, view;
	Uint8 clip;
} Cursor;

int WIDTH = 8 * HOR + 8 * PAD * 2;
int HEIGHT = 8 * (VER + 2) + 8 * PAD * 2;
int FPS = 30, GUIDES = 1, ZOOM = 2;

Document doc;
Cursor cursor;

Uint32 theme[] = {
	0x000000,
	0xFFFFFF,
	0x72DEC2,
	0x666666,
	0x222222};

Uint8 icons[][8] = {
	{0x38, 0x44, 0x82, 0x82, 0x82, 0x44, 0x38, 0x00}, /* color:blank */
	{0x38, 0x7c, 0xfe, 0xfe, 0xfe, 0x7c, 0x38, 0x00}, /* color:full */
	{0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00}, /* blank */
	{0x7c, 0x82, 0x92, 0xba, 0x92, 0x82, 0x7c, 0x00}, /* incr */
	{0x7c, 0x82, 0x82, 0xba, 0x82, 0x82, 0x7c, 0x00}, /* decr */
	{0x1c, 0x22, 0x42, 0x84, 0x42, 0x22, 0x1c, 0x00}, /* ror */
	{0x70, 0x88, 0x84, 0x42, 0x84, 0x88, 0x70, 0x00}, /* rol */
	{0xaa, 0x00, 0xaa, 0x00, 0xaa, 0x00, 0xaa, 0x00}, /* view:grid */
	{0xee, 0x92, 0x82, 0x54, 0x82, 0x92, 0xee, 0x00}, /* view:bigpixels */
	{0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xe0, 0x10, 0x00}, /* clip:blank */
	{0x82, 0xc5, 0xe2, 0xf0, 0xf8, 0xe0, 0x10, 0x00}, /* clip:active */
	{0x00, 0x00, 0x00, 0x82, 0x44, 0x38, 0x00, 0x00}, /* eye open */
	{0x00, 0x38, 0x44, 0x92, 0x28, 0x10, 0x00, 0x00}, /* eye closed */
	{0x10, 0x54, 0x28, 0xc6, 0x28, 0x54, 0x10, 0x00}  /* unsaved */
};

Uint8 font[][8] = {
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x24, 0x00, 0x7e, 0x3c, 0x00, 0x00},
	{0x00, 0x00, 0x24, 0x00, 0x3c, 0x42, 0x00, 0x00},
	{0x00, 0x00, 0x6c, 0x7c, 0x7c, 0x38, 0x10, 0x00},
	{0x00, 0x10, 0x38, 0x7c, 0x7c, 0x38, 0x10, 0x00},
	{0x00, 0x38, 0x38, 0x7c, 0x6c, 0x10, 0x38, 0x00},
	{0x00, 0x10, 0x38, 0x7c, 0x7c, 0x10, 0x38, 0x00},
	{0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00},
	{0x00, 0x7e, 0x42, 0x42, 0x42, 0x42, 0x7e, 0x00},
	{0x00, 0x00, 0x18, 0x24, 0x24, 0x18, 0x00, 0x00},
	{0x00, 0x3c, 0x66, 0x5a, 0x5a, 0x66, 0x3c, 0x00},
	{0x00, 0x1e, 0x06, 0x3a, 0x4a, 0x48, 0x30, 0x00},
	{0x00, 0x38, 0x44, 0x6c, 0x10, 0x7c, 0x10, 0x00},
	{0x00, 0x0e, 0x08, 0x08, 0x08, 0x38, 0x38, 0x00},
	{0x00, 0x3e, 0x3e, 0x22, 0x22, 0x66, 0x66, 0x00},
	{0x00, 0x00, 0x10, 0x44, 0x00, 0x44, 0x10, 0x00},
	{0x00, 0x00, 0x10, 0x18, 0x1c, 0x18, 0x10, 0x00},
	{0x00, 0x00, 0x08, 0x18, 0x38, 0x18, 0x08, 0x00},
	{0x00, 0x08, 0x1c, 0x00, 0x00, 0x1c, 0x08, 0x00},
	{0x00, 0x28, 0x28, 0x28, 0x28, 0x00, 0x28, 0x00},
	{0x00, 0x3e, 0x4a, 0x4a, 0x3a, 0x0a, 0x0a, 0x00},
	{0x00, 0x0c, 0x30, 0x46, 0x62, 0x0c, 0x30, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff},
	{0x00, 0x10, 0x38, 0x00, 0x38, 0x10, 0x00, 0x38},
	{0x00, 0x10, 0x38, 0x54, 0x10, 0x10, 0x10, 0x00},
	{0x00, 0x10, 0x10, 0x10, 0x54, 0x38, 0x10, 0x00},
	{0x00, 0x00, 0x08, 0x04, 0x7e, 0x04, 0x08, 0x00},
	{0x00, 0x00, 0x10, 0x20, 0x7e, 0x20, 0x10, 0x00},
	{0x00, 0x00, 0x40, 0x40, 0x7e, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x24, 0x66, 0x24, 0x00, 0x00},
	{0x00, 0x00, 0x10, 0x38, 0x7c, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x7c, 0x38, 0x10, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x10, 0x10, 0x10, 0x10, 0x00, 0x10, 0x00},
	{0x00, 0x24, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x24, 0x7e, 0x24, 0x24, 0x7e, 0x24, 0x00},
	{0x00, 0x08, 0x1e, 0x28, 0x1c, 0x0a, 0x3c, 0x08},
	{0x00, 0x62, 0x64, 0x08, 0x10, 0x26, 0x46, 0x00},
	{0x00, 0x30, 0x48, 0x32, 0x4c, 0x44, 0x3a, 0x00},
	{0x00, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x04, 0x08, 0x08, 0x08, 0x08, 0x04, 0x00},
	{0x00, 0x20, 0x10, 0x10, 0x10, 0x10, 0x20, 0x00},
	{0x00, 0x00, 0x14, 0x08, 0x3e, 0x08, 0x14, 0x00},
	{0x00, 0x00, 0x08, 0x08, 0x3e, 0x08, 0x08, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x10},
	{0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00},
	{0x00, 0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x00},
	{0x00, 0x3c, 0x46, 0x4a, 0x52, 0x62, 0x3c, 0x00},
	{0x00, 0x18, 0x28, 0x08, 0x08, 0x08, 0x3e, 0x00},
	{0x00, 0x3c, 0x42, 0x02, 0x3c, 0x40, 0x7e, 0x00},
	{0x00, 0x3c, 0x42, 0x1c, 0x02, 0x42, 0x3c, 0x00},
	{0x00, 0x08, 0x18, 0x28, 0x48, 0x7e, 0x08, 0x00},
	{0x00, 0x7e, 0x40, 0x7c, 0x02, 0x42, 0x3c, 0x00},
	{0x00, 0x3c, 0x40, 0x7c, 0x42, 0x42, 0x3c, 0x00},
	{0x00, 0x7e, 0x02, 0x04, 0x08, 0x10, 0x10, 0x00},
	{0x00, 0x3c, 0x42, 0x3c, 0x42, 0x42, 0x3c, 0x00},
	{0x00, 0x3c, 0x42, 0x42, 0x3e, 0x02, 0x3c, 0x00},
	{0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x10, 0x00},
	{0x00, 0x00, 0x10, 0x00, 0x00, 0x10, 0x10, 0x20},
	{0x00, 0x00, 0x08, 0x10, 0x20, 0x10, 0x08, 0x00},
	{0x00, 0x00, 0x00, 0x3e, 0x00, 0x3e, 0x00, 0x00},
	{0x00, 0x00, 0x10, 0x08, 0x04, 0x08, 0x10, 0x00},
	{0x00, 0x3c, 0x42, 0x0c, 0x10, 0x00, 0x10, 0x00},
	{0x00, 0x3c, 0x42, 0x52, 0x5e, 0x40, 0x3c, 0x00},
	{0x00, 0x3c, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x00},
	{0x00, 0x7c, 0x42, 0x7c, 0x42, 0x42, 0x7c, 0x00},
	{0x00, 0x3c, 0x42, 0x40, 0x40, 0x42, 0x3c, 0x00},
	{0x00, 0x78, 0x44, 0x42, 0x42, 0x44, 0x78, 0x00},
	{0x00, 0x7e, 0x40, 0x7c, 0x40, 0x40, 0x7e, 0x00},
	{0x00, 0x7e, 0x40, 0x40, 0x7c, 0x40, 0x40, 0x00},
	{0x00, 0x3c, 0x42, 0x40, 0x4e, 0x42, 0x3c, 0x00},
	{0x00, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x42, 0x00},
	{0x00, 0x3e, 0x08, 0x08, 0x08, 0x08, 0x3e, 0x00},
	{0x00, 0x02, 0x02, 0x02, 0x42, 0x42, 0x3c, 0x00},
	{0x00, 0x44, 0x48, 0x70, 0x48, 0x44, 0x42, 0x00},
	{0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x7e, 0x00},
	{0x00, 0x42, 0x66, 0x5a, 0x42, 0x42, 0x42, 0x00},
	{0x00, 0x42, 0x62, 0x52, 0x4a, 0x46, 0x42, 0x00},
	{0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00},
	{0x00, 0x7c, 0x42, 0x42, 0x7c, 0x40, 0x40, 0x00},
	{0x00, 0x3c, 0x42, 0x42, 0x52, 0x4a, 0x3c, 0x00},
	{0x00, 0x7c, 0x42, 0x42, 0x7c, 0x44, 0x42, 0x00},
	{0x00, 0x3c, 0x40, 0x3c, 0x02, 0x42, 0x3c, 0x00},
	{0x00, 0xfe, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},
	{0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00},
	{0x00, 0x42, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00},
	{0x00, 0x42, 0x42, 0x42, 0x5a, 0x66, 0x42, 0x00},
	{0x00, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x00},
	{0x00, 0x82, 0x44, 0x28, 0x10, 0x10, 0x10, 0x00},
	{0x00, 0x7e, 0x04, 0x08, 0x10, 0x20, 0x7e, 0x00},
	{0x00, 0x0e, 0x08, 0x08, 0x08, 0x08, 0x0e, 0x00},
	{0x00, 0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x00},
	{0x00, 0x70, 0x10, 0x10, 0x10, 0x10, 0x70, 0x00},
	{0x00, 0x08, 0x14, 0x22, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff},
	{0x00, 0x10, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x38, 0x04, 0x3c, 0x44, 0x3c, 0x00},
	{0x00, 0x20, 0x20, 0x3c, 0x22, 0x22, 0x3c, 0x00},
	{0x00, 0x00, 0x1c, 0x20, 0x20, 0x20, 0x1c, 0x00},
	{0x00, 0x04, 0x04, 0x3c, 0x44, 0x44, 0x3c, 0x00},
	{0x00, 0x00, 0x38, 0x44, 0x78, 0x40, 0x3c, 0x00},
	{0x00, 0x0c, 0x10, 0x1c, 0x10, 0x10, 0x10, 0x00},
	{0x00, 0x00, 0x38, 0x44, 0x44, 0x3c, 0x04, 0x38},
	{0x00, 0x40, 0x40, 0x58, 0x64, 0x44, 0x44, 0x00},
	{0x00, 0x10, 0x00, 0x30, 0x10, 0x10, 0x0c, 0x00},
	{0x00, 0x08, 0x00, 0x08, 0x08, 0x08, 0x48, 0x30},
	{0x00, 0x20, 0x20, 0x28, 0x30, 0x28, 0x24, 0x00},
	{0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x0c, 0x00},
	{0x00, 0x00, 0x68, 0x54, 0x54, 0x54, 0x54, 0x00},
	{0x00, 0x00, 0x58, 0x64, 0x44, 0x44, 0x44, 0x00},
	{0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x38, 0x00},
	{0x00, 0x00, 0x78, 0x44, 0x44, 0x78, 0x40, 0x40},
	{0x00, 0x00, 0x3c, 0x44, 0x44, 0x3c, 0x04, 0x06},
	{0x00, 0x00, 0x2c, 0x30, 0x20, 0x20, 0x20, 0x00},
	{0x00, 0x00, 0x38, 0x40, 0x38, 0x04, 0x78, 0x00},
	{0x00, 0x10, 0x10, 0x3c, 0x10, 0x10, 0x0c, 0x00},
	{0x00, 0x00, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00},
	{0x00, 0x00, 0x44, 0x44, 0x28, 0x28, 0x10, 0x00},
	{0x00, 0x00, 0x44, 0x54, 0x54, 0x54, 0x28, 0x00},
	{0x00, 0x00, 0x44, 0x28, 0x10, 0x28, 0x44, 0x00},
	{0x00, 0x00, 0x44, 0x44, 0x44, 0x3c, 0x04, 0x38},
	{0x00, 0x00, 0x7c, 0x08, 0x10, 0x20, 0x7c, 0x00},
	{0x00, 0x0e, 0x08, 0x30, 0x08, 0x08, 0x0e, 0x00},
	{0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00},
	{0x00, 0x70, 0x10, 0x0c, 0x10, 0x10, 0x70, 0x00},
	{0x00, 0x32, 0x4c, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x3c, 0x42, 0x99, 0xa1, 0xa1, 0x99, 0x42, 0x3c},
	{0x00, 0x3c, 0x42, 0x40, 0x42, 0x3c, 0x10, 0x70},
	{0x00, 0x44, 0x00, 0x44, 0x44, 0x44, 0x38, 0x00},
	{0x08, 0x10, 0x38, 0x44, 0x78, 0x40, 0x3c, 0x00},
	{0x10, 0x28, 0x00, 0x3c, 0x44, 0x4c, 0x34, 0x00},
	{0x00, 0x24, 0x00, 0x3c, 0x44, 0x4c, 0x34, 0x00},
	{0x00, 0x20, 0x10, 0x3c, 0x44, 0x4c, 0x34, 0x00},
	{0x10, 0x28, 0x10, 0x3c, 0x44, 0x4c, 0x34, 0x00},
	{0x00, 0x00, 0x1c, 0x20, 0x20, 0x1c, 0x08, 0x18},
	{0x10, 0x28, 0x38, 0x44, 0x78, 0x40, 0x3c, 0x00},
	{0x28, 0x00, 0x38, 0x44, 0x78, 0x40, 0x3c, 0x00},
	{0x20, 0x10, 0x3c, 0x44, 0x78, 0x40, 0x3c, 0x00},
	{0x00, 0x28, 0x00, 0x30, 0x10, 0x10, 0x0c, 0x00},
	{0x10, 0x28, 0x00, 0x30, 0x10, 0x10, 0x0c, 0x00},
	{0x20, 0x10, 0x00, 0x30, 0x10, 0x10, 0x0c, 0x00},
	{0x42, 0x3c, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x00},
	{0x08, 0x14, 0x3c, 0x42, 0x7e, 0x42, 0x42, 0x00},
	{0x08, 0x10, 0x7e, 0x40, 0x7c, 0x40, 0x7e, 0x00},
	{0x00, 0x00, 0x6c, 0x12, 0x3c, 0x50, 0x6e, 0x00},
	{0x00, 0x3e, 0x50, 0x7c, 0x50, 0x50, 0x5e, 0x00},
	{0x10, 0x28, 0x00, 0x38, 0x44, 0x44, 0x38, 0x00},
	{0x28, 0x00, 0x38, 0x44, 0x44, 0x44, 0x38, 0x00},
	{0x20, 0x10, 0x38, 0x44, 0x44, 0x44, 0x38, 0x00},
	{0x10, 0x28, 0x00, 0x44, 0x44, 0x44, 0x38, 0x00},
	{0x20, 0x10, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00},
	{0x44, 0x00, 0x44, 0x44, 0x44, 0x3c, 0x04, 0x38},
	{0x42, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00},
	{0x42, 0x00, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00},
	{0x00, 0x10, 0x38, 0x40, 0x40, 0x38, 0x10, 0x00},
	{0x00, 0x1c, 0x22, 0x78, 0x20, 0x20, 0x7e, 0x00},
	{0x00, 0x44, 0x28, 0x7c, 0x10, 0x7c, 0x10, 0x00},
	{0x00, 0x7c, 0x42, 0x52, 0x7c, 0x50, 0x4c, 0x00},
	{0x00, 0x0c, 0x10, 0x7c, 0x10, 0x10, 0x60, 0x00},
	{0x08, 0x10, 0x00, 0x3c, 0x44, 0x4c, 0x34, 0x00},
	{0x08, 0x10, 0x00, 0x30, 0x10, 0x10, 0x0c, 0x00},
	{0x08, 0x10, 0x38, 0x44, 0x44, 0x44, 0x38, 0x00},
	{0x08, 0x10, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00},
	{0x32, 0x4c, 0x00, 0x5c, 0x62, 0x42, 0x42, 0x00},
	{0x32, 0x4c, 0x00, 0x62, 0x52, 0x4a, 0x46, 0x00},
	{0x38, 0x48, 0x58, 0x28, 0x00, 0x78, 0x00, 0x00},
	{0x30, 0x48, 0x48, 0x30, 0x00, 0x78, 0x00, 0x00},
	{0x00, 0x08, 0x00, 0x08, 0x30, 0x42, 0x3c, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x7c, 0x40, 0x40, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x7c, 0x04, 0x04, 0x00},
	{0x44, 0x48, 0x50, 0x2c, 0x42, 0x04, 0x0e, 0x00},
	{0x44, 0x48, 0x50, 0x24, 0x54, 0x1e, 0x04, 0x00},
	{0x00, 0x10, 0x00, 0x10, 0x10, 0x10, 0x10, 0x00},
	{0x00, 0x00, 0x12, 0x24, 0x48, 0x24, 0x12, 0x00},
	{0x00, 0x00, 0x48, 0x24, 0x12, 0x24, 0x48, 0x00},
	{0x88, 0x44, 0x22, 0x11, 0x88, 0x44, 0x22, 0x11},
	{0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55},
	{0x44, 0x11, 0x44, 0x11, 0x44, 0x11, 0x44, 0x11},
	{0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10},
	{0x10, 0x10, 0x10, 0x10, 0xf0, 0x10, 0x10, 0x10},
	{0x10, 0x10, 0x10, 0xf0, 0x10, 0xf0, 0x10, 0x10},
	{0x28, 0x28, 0x28, 0x28, 0xe8, 0x28, 0x28, 0x28},
	{0x00, 0x00, 0x00, 0x00, 0xf8, 0x28, 0x28, 0x28},
	{0x00, 0x00, 0x00, 0xf0, 0x10, 0xf0, 0x10, 0x10},
	{0x28, 0x28, 0x28, 0xe8, 0x08, 0xe8, 0x28, 0x28},
	{0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28},
	{0x00, 0x00, 0x00, 0xf8, 0x08, 0xe8, 0x28, 0x28},
	{0x28, 0x28, 0x28, 0xe8, 0x08, 0xf8, 0x00, 0x00},
	{0x28, 0x28, 0x28, 0x28, 0xf8, 0x00, 0x00, 0x00},
	{0x10, 0x10, 0x10, 0xf0, 0x10, 0xf0, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0xf0, 0x10, 0x10, 0x10},
	{0x10, 0x10, 0x10, 0x10, 0x1f, 0x00, 0x00, 0x00},
	{0x10, 0x10, 0x10, 0x10, 0xff, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0xff, 0x10, 0x10, 0x10},
	{0x10, 0x10, 0x10, 0x10, 0x1f, 0x10, 0x10, 0x10},
	{0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00},
	{0x10, 0x10, 0x10, 0x10, 0xff, 0x10, 0x10, 0x10},
	{0x10, 0x10, 0x10, 0x1f, 0x10, 0x1f, 0x10, 0x10},
	{0x28, 0x28, 0x28, 0x28, 0x2f, 0x28, 0x28, 0x28},
	{0x28, 0x28, 0x28, 0x2f, 0x20, 0x3f, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x3f, 0x20, 0x2f, 0x28, 0x28},
	{0x28, 0x28, 0x28, 0xef, 0x00, 0xff, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0xff, 0x00, 0xef, 0x28, 0x28},
	{0x28, 0x28, 0x28, 0x2f, 0x20, 0x2f, 0x28, 0x28},
	{0x00, 0x00, 0x00, 0xff, 0x00, 0xff, 0x00, 0x00},
	{0x28, 0x28, 0x28, 0xef, 0x00, 0xef, 0x28, 0x28},
	{0x10, 0x10, 0x10, 0xff, 0x00, 0xff, 0x00, 0x00},
	{0x28, 0x28, 0x28, 0x28, 0xff, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0xff, 0x00, 0xff, 0x10, 0x10},
	{0x00, 0x00, 0x00, 0x00, 0xff, 0x28, 0x28, 0x28},
	{0x28, 0x28, 0x28, 0x28, 0x3f, 0x00, 0x00, 0x00},
	{0x10, 0x10, 0x10, 0x1f, 0x10, 0x1f, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x1f, 0x10, 0x1f, 0x10, 0x10},
	{0x00, 0x00, 0x00, 0x00, 0x3f, 0x28, 0x28, 0x28},
	{0x28, 0x28, 0x28, 0x28, 0xef, 0x28, 0x28, 0x28},
	{0x10, 0x10, 0x10, 0xff, 0x00, 0xff, 0x10, 0x10},
	{0x10, 0x10, 0x10, 0x10, 0xf0, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x1f, 0x10, 0x10, 0x10},
	{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
	{0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff},
	{0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0},
	{0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f},
	{0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x32, 0x4a, 0x44, 0x44, 0x3a, 0x00},
	{0x00, 0x38, 0x44, 0x5c, 0x42, 0x62, 0x5c, 0x40},
	{0x00, 0x7e, 0x42, 0x40, 0x40, 0x40, 0x40, 0x00},
	{0x00, 0x00, 0x7c, 0x28, 0x28, 0x28, 0x24, 0x00},
	{0x00, 0x7e, 0x22, 0x10, 0x10, 0x22, 0x7e, 0x00},
	{0x00, 0x00, 0x3e, 0x44, 0x44, 0x44, 0x38, 0x00},
	{0x00, 0x00, 0x44, 0x44, 0x44, 0x44, 0x7a, 0x40},
	{0x00, 0x00, 0x7c, 0x10, 0x10, 0x10, 0x08, 0x00},
	{0x00, 0x10, 0x38, 0x54, 0x54, 0x38, 0x10, 0x00},
	{0x00, 0x3c, 0x42, 0x42, 0x7e, 0x42, 0x3c, 0x00},
	{0x00, 0x3c, 0x42, 0x42, 0x42, 0x24, 0x66, 0x00},
	{0x78, 0x20, 0x18, 0x44, 0x44, 0x44, 0x38, 0x00},
	{0x00, 0x00, 0x6c, 0x92, 0x92, 0x6c, 0x00, 0x00},
	{0x00, 0x00, 0x18, 0x54, 0x54, 0x38, 0x10, 0x00},
	{0x00, 0x3e, 0x40, 0x7e, 0x40, 0x40, 0x3e, 0x00},
	{0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x42, 0x00},
	{0x00, 0x00, 0x7e, 0x00, 0x7e, 0x00, 0x7e, 0x00},
	{0x00, 0x08, 0x08, 0x3e, 0x08, 0x00, 0x3e, 0x00},
	{0x00, 0x10, 0x08, 0x04, 0x08, 0x10, 0x3c, 0x00},
	{0x00, 0x08, 0x10, 0x20, 0x10, 0x08, 0x3c, 0x00},
	{0x00, 0x00, 0x08, 0x14, 0x10, 0x10, 0x10, 0x10},
	{0x10, 0x10, 0x10, 0x10, 0x50, 0x20, 0x00, 0x00},
	{0x00, 0x00, 0x08, 0x00, 0x3e, 0x00, 0x08, 0x00},
	{0x00, 0x00, 0x32, 0x4c, 0x00, 0x32, 0x4c, 0x00},
	{0x00, 0x18, 0x24, 0x24, 0x18, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00},
	{0x00, 0x0e, 0x08, 0x08, 0x48, 0x28, 0x18, 0x00},
	{0x00, 0x50, 0x68, 0x48, 0x48, 0x00, 0x00, 0x00},
	{0x00, 0x60, 0x10, 0x20, 0x70, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x3c, 0x3c, 0x3c, 0x3c, 0x00, 0x00},
	{0x00, 0x00, 0x3c, 0x3c, 0x3c, 0x3c, 0x00, 0x00}};

SDL_Window *gWindow;
SDL_Renderer *gRenderer;
SDL_Texture *gTexture;
Uint32 *pixels;

#pragma mark - HELPERS

int
clamp(int val, int min, int max)
{
	return (val >= min) ? (val <= max) ? val : max : min;
}

char *
scpy(char *src, char *dst, int len)
{
	int i = 0;
	while((dst[i] = src[i]) && i < len - 2)
		i++;
	dst[i + 1] = '\0';
	return dst;
}

#pragma mark - DRAW

void
clear(Uint32 *dst)
{
	int v, h;
	for(v = 0; v < HEIGHT; v++)
		for(h = 0; h < WIDTH; h++)
			dst[v * WIDTH + h] = theme[0];
}

void
putpixel(Uint32 *dst, int x, int y, int color)
{
	if(x >= 0 && x < WIDTH - 8 && y >= 0 && y < HEIGHT - 8)
		dst[(y + PAD * 8) * WIDTH + (x + PAD * 8)] = theme[color];
}

void
drawchr(Uint32 *dst, int x, int y, Uint8 *icon)
{
	int v, h;
	for(v = 0; v < 8; v++)
		for(h = 0; h < 8; h++) {
			int ch1 = ((icon[v] >> h) & 0x1);
			int ch2 = (((icon[v + 8] >> h) & 0x1) << 1);
			putpixel(dst, x + 7 - h, y + v, ch1 + ch2);
		}
}

void
drawicon(Uint32 *dst, int x, int y, Uint8 *icon, int fg, int bg)
{
	int v, h;
	for(v = 0; v < 8; v++)
		for(h = 0; h < 8; h++) {
			int ch1 = (icon[v] >> (7 - h)) & 0x1;
			putpixel(dst, x + h, y + v, ch1 == 1 ? fg : bg);
		}
}

int
gethexfont(int v)
{
	if(v >= 0 && v <= 9)
		return 48 + v;
	if(v >= 10 && v <= 15)
		return 55 + v;
	return 46;
}

void
drawui(Uint32 *dst)
{
	int bottom = VER * 8 + 8;
	drawicon(dst, 0 * 8, bottom, icons[3], 2, 0);
	drawicon(dst, 1 * 8, bottom, icons[4], 2, 0);
	drawicon(dst, 2 * 8, bottom, icons[5], 2, 0);
	drawicon(dst, 3 * 8, bottom, icons[6], 2, 0);
	drawicon(dst, 5 * 8, bottom, icons[GUIDES ? 12 : 11], GUIDES ? 1 : 2, 0);
	drawicon(dst, (HOR - 1) * 8, bottom, icons[13], doc.unsaved ? 2 : 3, 0);
	drawicon(dst, 15 * 8, bottom, font[gethexfont((cursor.i >> 12) & 0xf)], 3, 0);
	drawicon(dst, 16 * 8, bottom, font[gethexfont((cursor.i >> 8) & 0xf)], 3, 0);
	drawicon(dst, 17 * 8, bottom, font[gethexfont((cursor.i >> 4) & 0xf)], 3, 0);
	drawicon(dst, 18 * 8, bottom, font[gethexfont(cursor.i & 0xf)], 3, 0);
}

void
drawline(Uint32 *dst, int y, int id)
{
	int b = 0;
	if(id >= SZ)
		return;
	for(b = 0; b < 8; ++b) {
		int x = b * 2 + ((b % 8) / 2);
		int k = id + b;
		int b0 = gethexfont(doc.data[k] >> 4 & 0xf);
		int b1 = gethexfont(doc.data[k] & 0xf);
		int bc = doc.data[k];
		int sel = cursor.i == k;
		int linesel = GUIDES && cursor.y == id / 8;
		int pagesel = id % 256 == 0;
		drawicon(dst, x * 8, y, font[b0], sel ? 0 : linesel + pagesel + 1, sel ? 2 : 0);
		drawicon(dst, x * 8 + 8, y, font[b1], sel ? 0 : linesel + pagesel + 1, sel ? 2 : 0);
		drawicon(dst, (19 + (k % 8)) * 8 + 8, y, bc ? font[bc] : icons[2], bc ? 1 : 3, 0);
	}
	drawicon(dst, 29 * 8, y, &doc.data[id], 1, 0);
	if(id % 16 == 0)
		drawchr(dst, 31 * 8, y, &doc.data[id]);
}

void
redraw(Uint32 *dst)
{
	int y;
	clear(dst);
	for(y = 0; y < VER; ++y)
		drawline(dst, y * 8, (cursor.view + y) * 8);
	drawui(dst);
	SDL_UpdateTexture(gTexture, NULL, dst, WIDTH * sizeof(Uint32));
	SDL_RenderClear(gRenderer);
	SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
	SDL_RenderPresent(gRenderer);
}

#pragma mark - OPTIONS

int
error(char *msg, const char *err)
{
	printf("Error %s: %s\n", msg, err);
	return 0;
}

void
savemode(int *i, int v)
{
	*i = v;
	redraw(pixels);
}

void
makedoc(Document *d, char *name)
{
	int i;
	for(i = 0; i < SZ; ++i)
		d->data[i] = 0x00;
	d->unsaved = 0;
	scpy(name, d->name, 256);
	printf("Made: %s\n", d->name);
	redraw(pixels);
}

int
savedoc(Document *d, char *name)
{
	FILE *f = fopen(name, "w");
	if(!fwrite(d->data, sizeof(d->data), 1, f))
		return error("Save", "Failure");
	d->unsaved = 0;
	scpy(name, d->name, 256);
	fclose(f);
	printf("Saved: %s\n", d->name);
	redraw(pixels);
	return 1;
}

int
opendoc(Document *d, char *name)
{
	FILE *f = fopen(name, "r");
	if(!f)
		return error("Load", "Invalid input file");
	fread(doc.data, sizeof(doc.data), 1, f);
	d->unsaved = 0;
	scpy(name, doc.name, 256);
	fclose(f);
	printf("Loaded: %s\n", doc.name);
	redraw(pixels);
	return 1;
}

int
printdoc(Document *d)
{
	int i;
	FILE *f = fopen("exed-output.c", "w");
	fprintf(f, "{");
	for(i = 0; i < SZ; ++i) {
		if(i > 0 && i % 16 == 0)
			fprintf(f, "},\n{");
		fprintf(f, "0x%02x", doc.data[i]);
		if(i % 16 < 15)
			fprintf(f, ", ");
	}
	fprintf(f, "}");
	fclose(f);
	puts("Saved exed-output.c");
	return 1;
}

int
printline(Document *d, int id)
{
	int i;
	FILE *f = fopen("exed-output-selection.c", "w");
	fprintf(f, "{");
	for(i = 0; i < 8; ++i) {
		fprintf(f, "0x%02x", doc.data[id + i]);
		if(i % 8 < 7)
			fprintf(f, ", ");
	}
	fprintf(f, "}");
	fclose(f);
	puts("Saved exed-output-selection.c");
	return 1;
}

void
copyclip(int i)
{
	cursor.clip = doc.data[i];
}

void
pasteclip(int i)
{
	doc.data[i] = cursor.clip;
	redraw(pixels);
}

void
cutclip(int i)
{
	copyclip(i);
	doc.data[i] = 0x00;
	redraw(pixels);
}

void
select(int x, int y)
{
	cursor.x = clamp(x, 0, 7);
	cursor.y = clamp(y, 0, SZ / 8 - 1);
	cursor.i = cursor.x % 8 + (cursor.y * 8);
	if(cursor.y >= cursor.view + VER)
		cursor.view = clamp(cursor.y - VER + 1, 0, SZ - 4);
	if(cursor.y <= cursor.view)
		cursor.view = clamp(cursor.y, 0, SZ - 4);
	cursor.swap = 0;
	redraw(pixels);
}

void
insert(int v)
{
	if(!cursor.swap)
		doc.data[cursor.i] = v * 16 + (doc.data[cursor.i] & 0xf);
	else
		doc.data[cursor.i] = (doc.data[cursor.i] / 16 * 16) + v;
	cursor.swap = !cursor.swap;
	redraw(pixels);
}

char
doincr(char c)
{
	return c + 1;
}

char
dodecr(char c)
{
	return c - 1;
}

void
transform(int key, char (*fn)(char))
{
	doc.data[key] = fn(doc.data[key]);
	doc.unsaved = 1;
	redraw(pixels);
}

void
selectoption(int option)
{
	switch(option) {
	case 0: transform(cursor.i, doincr); break;
	case 1: transform(cursor.i, dodecr); break;
	case 2:
		doc.data[cursor.i] = doc.data[cursor.i] << 2;
		redraw(pixels);
		break;
	case 3:
		doc.data[cursor.i] = doc.data[cursor.i] >> 2;
		redraw(pixels);
		break;
	case 5: savemode(&GUIDES, !GUIDES); break;
	case HOR - 1: savedoc(&doc, doc.name); break;
	}
}

void
selectcell(int x, int y)
{
	if(GUIDES) {
		if(x == 4 || x == 9 || x == 14)
			return;
		if(x > 4)
			x--;
		if(x > 9)
			x--;
		if(x > 13)
			x--;
		x /= 2;
	} else
		x /= 3;
	select(x, y + cursor.view);
}

void
quit(void)
{
	free(pixels);
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	SDL_Quit();
	exit(0);
}

#pragma mark - TRIGGERS

void
domouse(SDL_Event *event)
{
	switch(event->type) {
	case SDL_MOUSEBUTTONUP:
		break;
	case SDL_MOUSEBUTTONDOWN:
		if(event->motion.y / ZOOM / 8 - PAD == VER + 1) {
			selectoption(event->motion.x / ZOOM / 8 - PAD);
			return;
		}
		selectcell(
			event->motion.x / ZOOM / 8 - PAD,
			event->motion.y / ZOOM / 8 - PAD);
		break;
	case SDL_MOUSEMOTION:

		break;
	}
}

void
dokey(SDL_Event *event)
{
	int shift = SDL_GetModState() & KMOD_LSHIFT || SDL_GetModState() & KMOD_RSHIFT;
	int ctrl = SDL_GetModState() & KMOD_LCTRL || SDL_GetModState() & KMOD_RCTRL;
	if(ctrl) {
		switch(event->key.keysym.sym) {
		/* Generic */
		case SDLK_n: makedoc(&doc, "untitled.chr"); break;
		case SDLK_r: opendoc(&doc, doc.name); break;
		case SDLK_s: savedoc(&doc, doc.name); break;
		case SDLK_x: cutclip(cursor.i); break;
		case SDLK_c: copyclip(cursor.i); break;
		case SDLK_v: pasteclip(cursor.i); break;
		case SDLK_p: shift ? printline(&doc, cursor.y * 8) : printdoc(&doc); break;
		case SDLK_h: savemode(&GUIDES, !GUIDES); break;
		case SDLK_UP: transform(cursor.i, doincr); break;
		case SDLK_DOWN: transform(cursor.i, dodecr); break;
		}
	} else {
		switch(event->key.keysym.sym) {
		case SDLK_UP: select(cursor.x, cursor.y - 1); break;
		case SDLK_DOWN: select(cursor.x, cursor.y + 1); break;
		case SDLK_LEFT: select(cursor.x - 1, cursor.y); break;
		case SDLK_RIGHT: select(cursor.x + 1, cursor.y); break;
		case SDLK_PAGEUP: select(cursor.x, cursor.y - 16); break;
		case SDLK_PAGEDOWN: select(cursor.x, cursor.y + 16); break;
		case SDLK_BACKSPACE: insert(0); break;
		}
	}
}

void
dotext(SDL_Event *event)
{
	int i;
	if(SDL_GetModState() & KMOD_LCTRL || SDL_GetModState() & KMOD_RCTRL)
		return;
	for(i = 0; i < SDL_TEXTINPUTEVENT_TEXT_SIZE; ++i) {
		char c = event->text.text[i];
		if(c < ' ' || c > '~')
			break;
		if(c < '0' || c > 'f')
			break;
		if(c >= '0' && c <= '9')
			insert(c - '0');
		if(c >= 'a' && c <= 'f')
			insert(c - 'a' + 10);
	}
}

void
dowheel(SDL_Event *event)
{
	select(cursor.x, cursor.y - event->wheel.y);
}

int
init(void)
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		return error("Init", SDL_GetError());
	gWindow = SDL_CreateWindow("Exed",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WIDTH * ZOOM,
		HEIGHT * ZOOM,
		SDL_WINDOW_SHOWN);
	if(gWindow == NULL)
		return error("Window", SDL_GetError());
	gRenderer = SDL_CreateRenderer(gWindow, -1, 0);
	if(gRenderer == NULL)
		return error("Renderer", SDL_GetError());
	gTexture = SDL_CreateTexture(gRenderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STATIC,
		WIDTH,
		HEIGHT);
	if(gTexture == NULL)
		return error("Texture", SDL_GetError());
	pixels = (Uint32 *)malloc(WIDTH * HEIGHT * sizeof(Uint32));
	if(pixels == NULL)
		return error("Pixels", "Failed to allocate memory");
	clear(pixels);
	return 1;
}

int
main(int argc, char **argv)
{
	int ticknext = 0;
	if(!init())
		return error("Init", "Failure");
	if(argc > 1) {
		if(!opendoc(&doc, argv[1]))
			makedoc(&doc, argv[1]);
	} else
		makedoc(&doc, "untitled.chr");
	select(0, 0);
	while(1) {
		int tick = SDL_GetTicks();
		SDL_Event event;
		if(tick < ticknext)
			SDL_Delay(ticknext - tick);
		ticknext = tick + (1000 / FPS);
		while(SDL_PollEvent(&event) != 0) {
			switch(event.type) {
			case SDL_QUIT: quit(); break;
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEMOTION: domouse(&event); break;
			case SDL_MOUSEWHEEL: dowheel(&event); break;
			case SDL_TEXTINPUT: dotext(&event); break;
			case SDL_KEYDOWN: dokey(&event); break;
			case SDL_WINDOWEVENT:
				if(event.window.event == SDL_WINDOWEVENT_EXPOSED)
					redraw(pixels);
				break;
			}
		}
	}
	quit();
	return 0;
}

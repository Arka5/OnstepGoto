﻿#include "_EEPROM_ext.h"
#include "SmartController.h"
#include "LX200.h"


static char* BreakRC[4] = { ":Qn#" ,":Qs#" ,":Qe#" ,":Qw#" };
static char* RC[4] = { ":Mn#" , ":Ms#" ,":Me#" ,":Mw#" };


#define MY_BORDER_SIZE 1
#define icon_width 16
#define icon_height 16

#define teenastro_width 128
#define teenastro_height 68



static unsigned char wifi_bits[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x20, 0x40, 0x4e, 0x00, 0x11,
  0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0xfe, 0x7f, 0x02, 0x40, 0xda, 0x5f,
  0xda, 0x5f, 0x02, 0x40, 0xfe, 0x7f, 0x00, 0x00 };


static unsigned char align1_bits[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x00, 0x00,
  0xc0, 0x03, 0xc0, 0x00, 0x40, 0x01, 0x50, 0x02, 0x18, 0x04, 0x10, 0x08,
  0x10, 0x10, 0x10, 0x20, 0x00, 0x00, 0x00, 0x00 };

static unsigned char align2_bits[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x00, 0x00,
  0xc0, 0x03, 0xc0, 0x00, 0x40, 0x01, 0x48, 0x02, 0x14, 0x04, 0x10, 0x08,
  0x08, 0x10, 0x04, 0x20, 0x1c, 0x00, 0x00, 0x00 };

static unsigned char align3_bits[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x00, 0x00,
  0xc0, 0x03, 0xc0, 0x00, 0x40, 0x01, 0x40, 0x02, 0x1c, 0x04, 0x10, 0x08,
  0x18, 0x10, 0x10, 0x20, 0x1c, 0x00, 0x00, 0x00 };

static unsigned char home_bits[] U8X8_PROGMEM = {
  0x00, 0x02, 0x00, 0x07, 0x80, 0x0f, 0xc0, 0x1f, 0x80, 0x3f, 0x00, 0x7f,
  0x00, 0x7e, 0x00, 0x7f, 0x80, 0xfb, 0xc0, 0xc1, 0xe0, 0x01, 0xbc, 0x49,
  0x9e, 0x49, 0x9e, 0x79, 0x8c, 0x49, 0x80, 0x49 };

static unsigned char parked_bits[] U8X8_PROGMEM = {
  0x00, 0x00, 0xfe, 0x7f, 0x02, 0x40, 0x02, 0x40, 0xe2, 0x43, 0x62, 0x46,
  0x62, 0x46, 0x62, 0x46, 0xe2, 0x43, 0x62, 0x40, 0x62, 0x40, 0x62, 0x40,
  0x62, 0x40, 0x02, 0x40, 0xfe, 0x7f, 0x00, 0x00 };

static unsigned char parking_bits[] U8X8_PROGMEM = {
  0xff, 0xff, 0x01, 0x80, 0x01, 0x80, 0xf9, 0x80, 0x99, 0x81, 0x99, 0x81,
  0x99, 0x81, 0xf9, 0x80, 0x19, 0x80, 0x99, 0x84, 0x99, 0x8d, 0x99, 0x9f,
  0x81, 0x8d, 0x81, 0x84, 0x01, 0x80, 0xff, 0xff };

static unsigned char parkingFailed_bits[] U8X8_PROGMEM = {
  0xff, 0xff, 0x01, 0x80, 0x01, 0x80, 0xf9, 0x90, 0x99, 0x91, 0x99, 0x91,
  0x99, 0x91, 0xf9, 0x90, 0x19, 0x90, 0xd9, 0x93, 0x59, 0x90, 0xd9, 0x91,
  0x41, 0x80, 0x41, 0x90, 0x01, 0x80, 0xff, 0xff };

static unsigned char guiding_bits[] U8X8_PROGMEM = {
  0x00, 0x00, 0x80, 0x01, 0x80, 0x01, 0xc0, 0x03, 0x20, 0x04, 0x10, 0x08,
  0x08, 0x10, 0x8e, 0x71, 0x8e, 0x71, 0x08, 0x10, 0x10, 0x08, 0x20, 0x04,
  0xc0, 0x03, 0x80, 0x01, 0x80, 0x01, 0x00, 0x00 };

static unsigned char no_tracking_bits[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x38, 0x1c, 0x7c, 0x3e, 0x7c, 0x3e, 0x7c, 0x3e,
  0x7c, 0x3e, 0x7c, 0x3e, 0x7c, 0x3e, 0x7c, 0x3e, 0x7c, 0x3e, 0x7c, 0x3e,
  0x7c, 0x3e, 0x7c, 0x3e, 0x38, 0x1c, 0x00, 0x00 };

static unsigned char tracking_bits[] U8X8_PROGMEM = {
  0x00, 0x00, 0x02, 0x00, 0x06, 0x00, 0x0e, 0x00, 0x1e, 0x00, 0x3e, 0x00,
  0x7e, 0x00, 0xfe, 0x38, 0xfe, 0x44, 0x7e, 0x44, 0x3e, 0x20, 0x1e, 0x10,
  0x0e, 0x10, 0x06, 0x00, 0x02, 0x10, 0x00, 0x00 };

static unsigned char tracking_S_bits[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x06, 0x00, 0x0e, 0x00, 0x1e, 0x00,
  0x3e, 0x00, 0x7e, 0x00, 0xfe, 0x38, 0x7e, 0x04, 0x3e, 0x04, 0x1e, 0x18,
  0x0e, 0x20, 0x06, 0x20, 0x02, 0x1c, 0x00, 0x00 };

static unsigned char sleewing_bits[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x06, 0x03, 0x0e, 0x07, 0x1e, 0x0f,
  0x3e, 0x1f, 0x7e, 0x3f, 0xfe, 0x7f, 0x7e, 0x3f, 0x3e, 0x1f, 0x1e, 0x0f,
  0x0e, 0x07, 0x06, 0x03, 0x02, 0x01, 0x00, 0x00 };

static unsigned char W_bits[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0xe0, 0x03, 0x10, 0x04, 0x08, 0x08, 0x24, 0x12,
  0x22, 0x22, 0x22, 0x22, 0xa2, 0x22, 0xa2, 0x22, 0x42, 0x21, 0x44, 0x11,
  0x08, 0x08, 0x10, 0x04, 0xe0, 0x03, 0x00, 0x00 };

static unsigned char E_bits[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0xe0, 0x03, 0x10, 0x04, 0x08, 0x08, 0xe4, 0x13,
  0x22, 0x20, 0x22, 0x20, 0xe2, 0x21, 0x22, 0x20, 0x22, 0x20, 0xe4, 0x13,
  0x08, 0x08, 0x10, 0x04, 0xe0, 0x03, 0x00, 0x00 };

static unsigned char ErrDe_bits[] U8X8_PROGMEM = {
  0xff, 0xff, 0x00, 0x80, 0x0e, 0xb0, 0x02, 0xb0, 0x66, 0xb3, 0x22, 0xb1,
  0x2e, 0xb1, 0x00, 0xb0, 0x1e, 0xb0, 0x22, 0xb0, 0xa2, 0xb3, 0xa2, 0xb2,
  0xa2, 0x83, 0xa2, 0xb0, 0x9e, 0xb3, 0x00, 0x80 };

static unsigned char ErrHo_bits[] U8X8_PROGMEM = {
  0xff, 0xff, 0x00, 0x80, 0x0e, 0xb0, 0x02, 0xb0, 0x66, 0xb3, 0x22, 0xb1,
  0x2e, 0xb1, 0x00, 0xb0, 0x22, 0xb0, 0x22, 0xb0, 0x22, 0xb3, 0xbe, 0xb4,
  0xa2, 0x84, 0xa2, 0xb4, 0x22, 0xb3, 0x00, 0x80 };

static unsigned char ErrMe_bits[] U8X8_PROGMEM = {
  0xff, 0xff, 0x00, 0x80, 0x0e, 0xb0, 0x02, 0xb0, 0x66, 0xb3, 0x22, 0xb1,
  0x2e, 0xb1, 0x00, 0xb0, 0x22, 0xb0, 0x36, 0xb0, 0xaa, 0xb3, 0xa2, 0xb2,
  0xa2, 0x83, 0xa2, 0xb0, 0xa2, 0xb3, 0x00, 0x80 };

static unsigned char ErrMf_bits[] U8X8_PROGMEM = {
  0xff, 0xff, 0x00, 0x80, 0x0e, 0xb0, 0x02, 0xb0, 0x66, 0xb3, 0x22, 0xb1,
  0x2e, 0xb1, 0x00, 0xb0, 0x22, 0xb0, 0x36, 0xb0, 0xaa, 0xb3, 0xa2, 0xb0,
  0xa2, 0x81, 0xa2, 0xb0, 0xa2, 0xb0, 0x00, 0x80 };

static unsigned char ErrUp_bits[] U8X8_PROGMEM = {
  0xff, 0xff, 0x00, 0x80, 0x0e, 0xb0, 0x02, 0xb0, 0x66, 0xb3, 0x22, 0xb1,
  0x2e, 0xb1, 0x00, 0xb0, 0x22, 0xb0, 0x22, 0xb0, 0xa2, 0xb3, 0xa2, 0xb2,
  0xa2, 0x83, 0xa2, 0xb0, 0x9c, 0xb0, 0x00, 0x80 };

static const unsigned char teenastro_bits[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x03,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x80, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00,
  0x00, 0x00, 0x00, 0x10, 0x00, 0x80, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x0f, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
  0x00, 0xe0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x01, 0x00, 0xfc, 0x01, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xfe, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfe, 0x0f, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
  0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x3f, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xfc, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1f, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0x03, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x7e, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x7f, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x80, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x03, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x0f,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xf8, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9c, 0x7f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xfe,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x06, 0xf8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x0f, 0xe0, 0x07, 0x00, 0x00, 0x00,
  0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1f, 0xc0,
  0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xc0, 0x1e, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x1e, 0x00, 0xfc, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x3c, 0x00,
  0xf0, 0x81, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x60, 0x2c, 0x00, 0x80, 0xc7, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0x00, 0x00, 0x00, 0x20, 0x68, 0x00, 0x00, 0xce, 0x3f, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x48, 0x00,
  0x00, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x10, 0xd8, 0x00, 0x00, 0xf0, 0x7f, 0x00, 0x00, 0x20, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xd8, 0x00, 0x00, 0xf0, 0x7f, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x98, 0x01,
  0x00, 0xf0, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x0c, 0x90, 0x01, 0x00, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x30, 0x01, 0x00, 0xf0, 0x3f, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x30, 0x03,
  0x00, 0xc0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x02, 0x30, 0x07, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x30, 0x06, 0x00, 0x80, 0x7f, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x20, 0x04,
  0x00, 0x80, 0xff, 0x0f, 0xfe, 0x01, 0x00, 0x00, 0x1c, 0x00, 0x02, 0x00,
  0x80, 0x01, 0x20, 0x0c, 0x00, 0xc0, 0xff, 0x3f, 0xfe, 0x01, 0x00, 0x00,
  0x1c, 0x00, 0x03, 0x00, 0x80, 0x00, 0x20, 0x08, 0x00, 0xe0, 0xff, 0xff,
  0x30, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x03, 0x00, 0xc0, 0x00, 0x20, 0x18,
  0x00, 0xf0, 0xff, 0xff, 0x30, 0x38, 0x1c, 0x1b, 0x3e, 0x9c, 0xb7, 0x18,
  0x40, 0x00, 0x60, 0x30, 0x00, 0xfc, 0xff, 0xff, 0x30, 0x6c, 0x36, 0x3f,
  0x36, 0xbe, 0xf7, 0x3c, 0x60, 0x00, 0x40, 0x30, 0x80, 0xff, 0xff, 0xff,
  0x30, 0x6c, 0x36, 0x33, 0x36, 0x26, 0x33, 0x66, 0x20, 0x00, 0x40, 0x20,
  0xc0, 0x8f, 0xff, 0xff, 0x30, 0x7c, 0x3e, 0x33, 0x63, 0x1e, 0x33, 0x66,
  0x20, 0x00, 0x40, 0x60, 0xc0, 0x07, 0xff, 0xff, 0x30, 0x0c, 0x06, 0x33,
  0x7f, 0x38, 0x33, 0x66, 0x30, 0x00, 0x40, 0x40, 0xc0, 0x1f, 0xff, 0xff,
  0x30, 0x6c, 0x36, 0x33, 0x7f, 0x32, 0x33, 0x66, 0x10, 0x00, 0xc0, 0xc0,
  0xc0, 0x7f, 0xff, 0xff, 0x30, 0x6c, 0x36, 0xb3, 0xc1, 0x3e, 0x37, 0x3c,
  0x18, 0x00, 0xc0, 0xc0, 0xc0, 0xff, 0xff, 0xff, 0x30, 0x38, 0x1c, 0xb3,
  0xc1, 0x1c, 0x36, 0x1c, 0x0c, 0x00, 0xc0, 0x80, 0xc1, 0xff, 0xff, 0xff,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0xc0, 0x81,
  0xc1, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x0e, 0x00, 0xc0, 0x81, 0xf3, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x80, 0xc3, 0xf3, 0xff, 0xff, 0xff,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x80, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x07, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

void gethms(const long& v, uint8_t& v1, uint8_t& v2, uint8_t& v3)
{
  v3 = v % 60;
  v2 = (v / 60) % 60;
  v1 = v / 3600;
}

void getdms(const long& v, short& v1, uint8_t& v2, uint8_t& v3)
{
  v3 = abs(v) % 60;
  v2 = (abs(v) / 60) % 60;
  v1 = v / 3600;
}

void longRa2Ra(long Ra, int& h, int& m, int& s)
{
  h = Ra / 30;
  m = (Ra - h * 30) / 60;
  s = (Ra / 30) % 60;
}

void longDec2Dec(long Dec, int& deg, int& min)
{
  deg = Dec / 60;
  min = Dec % 60;
}


void SmartHandController::setup(const int pin[7],const bool active[7], const int SerialBaud, const OLED model)
{
  telInfo.lastState = 0;
  buttonPad.setup( pin, active);
  if (model == OLED_SH1106)
    display = new U8G2_EXT_SH1106_128X64_NONAME_1_HW_I2C(U8G2_R0);
  else if (model == OLED_SSD1306)
    display = new U8G2_EXT_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0);
  display->begin();
  drawIntro();
  tickButtons();

  Ser.begin(SerialBaud);
  for (int i = 0; i < 3; i++)
  {
    Ser.print(":#");
    delay(500);
    Ser.flush();
    delay(500);
  }
}
void SmartHandController::tickButtons()
{
  buttonPad.tickButtons();
}

bool SmartHandController::buttonPressed()
{
  return buttonPad.buttonPressed();
}
void SmartHandController::update()
{
  tickButtons();
  unsigned long top = millis();
  if (buttonPressed())
  {
    if (sleepDisplay)
    {
      display->setContrast(maxContrast);
      display->sleepOff();
      sleepDisplay = false;
      lowContrast = false;
      time_last_action = millis();
    }
    if (lowContrast)
    {
      display->setContrast(maxContrast);
      lowContrast = false;
      time_last_action = top;
    }
  }
  else if (sleepDisplay)
  {
    return;
  }
  else if (top - time_last_action > 120000)
  {
    display->sleepOn();
    sleepDisplay = true;
    return;
  }
  else if (top - time_last_action > 30000 && !lowContrast)
  {
    display->setContrast(0);
    lowContrast = true;
    return;
  }
  if (powerCylceRequired)
  {
    display->setFont(u8g2_font_helvR12_tr);
    DisplayMessage("REBOOT", "DEVICE", 1000);
    return;
  }
  if (telInfo.align == Telescope::ALI_SELECT_STAR_1 || telInfo.align == Telescope::ALI_SELECT_STAR_2 || telInfo.align == Telescope::ALI_SELECT_STAR_3)
  {
    if (telInfo.align == Telescope::ALI_SELECT_STAR_1)
      DisplayLongMessage("Select a Star", "near the Meridian", "& the Celestial Equ.", "in the Western Sky", -1);
    else if (telInfo.align == Telescope::ALI_SELECT_STAR_2)
      DisplayLongMessage("Select a Star", "near the Meridian", "& the Celestial Equ.", "in the Eastern Sky", -1);
    else if (telInfo.align == Telescope::ALI_SELECT_STAR_3)
      DisplayLongMessage("Select a Star", "HA = -3 hour", "Dec = +- 45 degree", "in the Eastern Sky", -1);
    if (!SelectStarAlign())
    {
      DisplayMessage("Alignment", "Aborted", -1);
      telInfo.align = Telescope::ALI_OFF;
      return;
    }
    telInfo.align = static_cast<Telescope::AlignState>(telInfo.align + 1);
  }
  else if (top - lastpageupdate > 100)
  {
    updateMainDisplay( page);
  }
  if (telInfo.connected == false)
  {
    DisplayMessage("Hand controler", "not connected", -1);
  }
  if (telInfo.connected && (telInfo.getTrackingState() == Telescope::TRK_SLEWING || telInfo.getParkState() == Telescope::PRK_PARKING))
  {
    bool stop = (eventbuttons[0] == E_LONGPRESS || eventbuttons[0] == E_LONGPRESSTART || eventbuttons[0] == E_DOUBLECLICK) ? true : false;
    int it = 1;
    while (!stop && it < 5)
    {
      stop = (eventbuttons[it] == E_LONGPRESS || eventbuttons[it] == E_CLICK || eventbuttons[it] == E_LONGPRESSTART);
      it++;
    }
    if (stop)
    {
      Ser.print(":Q#");
      Ser.flush();
      time_last_action = millis();
      display->sleepOff();
      if (telInfo.align != Telescope::ALI_OFF)
      {
        telInfo.align = static_cast<Telescope::AlignState>(telInfo.align - 1);
      }
      return;
    }

  }
  else
  {
    buttonCommand = false;
    for (int k = 1; k < 5; k++)
    {
      if (Move[k - 1] && (eventbuttons[k] == E_LONGPRESSSTOP || eventbuttons[k] == E_NONE))
      {
        buttonCommand = true;
        Move[k - 1] = false;
        Ser.print(BreakRC[k - 1]);
        Ser.flush();
        continue;
      }
      else if (!Move[k - 1] && (eventbuttons[k] == E_LONGPRESS || eventbuttons[k] == E_CLICK || eventbuttons[k] == E_LONGPRESSTART))
      {
        buttonCommand = true;
        Move[k - 1] = true;
        Ser.print(RC[k - 1]);
        Ser.flush();
        continue;
      }
    }
    if (buttonCommand)
    {
      time_last_action = millis();
      return;
    }

  }
  if (eventbuttons[0] == E_DOUBLECLICK /*|| eventbuttons[0] == E_CLICK)  && eventbuttons[1] != E_NONE*/)
  {
    menuSpeedRate();
    time_last_action = millis();
  }
  else if (eventbuttons[0] == E_CLICK && telInfo.align == Telescope::ALI_OFF)
  {
    page++;
    if (page > 2) page = 0;
    time_last_action = millis();
  }
  else if (eventbuttons[0] == E_LONGPRESS && telInfo.align == Telescope::ALI_OFF)
  {
    menuMain();
    time_last_action = millis();
  }
  else if (eventbuttons[0] == E_CLICK && (telInfo.align == Telescope::ALI_RECENTER_1 || telInfo.align == Telescope::ALI_RECENTER_2 || telInfo.align == Telescope::ALI_RECENTER_3))
  {
    telInfo.addStar();
  }

}

void SmartHandController::updateMainDisplay( u8g2_uint_t page)
{
  u8g2_t *u8g2 = display->getU8g2();
  display->setFont(u8g2_font_helvR12_te);
  u8g2_uint_t line_height = u8g2_GetAscent(u8g2) - u8g2_GetDescent(u8g2) + MY_BORDER_SIZE;
  u8g2_uint_t step1 = u8g2_GetUTF8Width(u8g2, "44");
  u8g2_uint_t step2 = u8g2_GetUTF8Width(u8g2, "4") + 1;
  telInfo.connected = true;
  telInfo.updateTel();
  if (telInfo.connected == false)
  {
    return;
  }
  if (telInfo.hasTelStatus && telInfo.align != Telescope::ALI_OFF)
  {
    Telescope::TrackState curT = telInfo.getTrackingState();
    if (curT != Telescope::TRK_SLEWING && (telInfo.align == Telescope::ALI_SLEW_STAR_1 || telInfo.align == Telescope::ALI_SLEW_STAR_2 || telInfo.align == Telescope::ALI_SLEW_STAR_3))
    {
      telInfo.align = static_cast<Telescope::AlignState>(telInfo.align + 1);
    }
    page = 3;
  }
  else if (page == 0)
  {
    telInfo.updateRaDec();
  }
  else if (page == 1)
  {
    telInfo.updateAzAlt();
  }
  else
  {
    telInfo.updateTime();
  }
  u8g2_FirstPage(u8g2);

  do
  {
    u8g2_uint_t x = u8g2_GetDisplayWidth(u8g2);
    int k = 0;
    if (wifiOn)
      display->drawXBMP(0, 0, icon_width, icon_height, wifi_bits);

    if (telInfo.hasTelStatus)
    {
      Telescope::ParkState curP = telInfo.getParkState();
      Telescope::TrackState curT = telInfo.getTrackingState();
      if (curP == Telescope::PRK_PARKED)
      {
        display->drawXBMP(x - icon_width, 0, icon_width, icon_height, parked_bits);
        x -= icon_width + 1;
      }
      else if (curP == Telescope::PRK_PARKING)
      {
        display->drawXBMP(x - icon_width, 0, icon_width, icon_height, parking_bits);
        x -= icon_width + 1;
      }
      else if (telInfo.atHome())
      {
        display->drawXBMP(x - icon_width, 0, icon_width, icon_height, home_bits);
        x -= icon_width + 1;
      }
      else
      {
        if (curT == Telescope::TRK_SLEWING)
        {
          display->drawXBMP(x - icon_width, 0, icon_width, icon_height, sleewing_bits);
          x -= icon_width + 1;
        }
        else if (curT == Telescope::TRK_ON)
        {
          display->drawXBMP(x - icon_width, 0, icon_width, icon_height, tracking_S_bits);
          x -= icon_width + 1;
        }
        else if (curT == Telescope::TRK_OFF)
        {
          display->drawXBMP(x - icon_width, 0, icon_width, icon_height, no_tracking_bits);
          x -= icon_width + 1;
        }

        if (curP == Telescope::PRK_FAILED)
        {
          display->drawXBMP(x - icon_width, 0, icon_width, icon_height, parkingFailed_bits);
          x -= icon_width + 1;
        }
        if (telInfo.hasPierInfo)
        {
          Telescope::PierState CurP = telInfo.getPierState();
          if (CurP == Telescope::PIER_E)
          {
            display->drawXBMP(x - icon_width, 0, icon_width, icon_height, E_bits);
            x -= icon_width + 1;
          }
          else if (CurP == Telescope::PIER_W)
          {
            display->drawXBMP(x - icon_width, 0, icon_width, icon_height, W_bits);
            x -= icon_width + 1;
          }

        }
        if (telInfo.align != Telescope::ALI_OFF)
        {
          if (telInfo.aliMode == Telescope::ALIM_ONE)
            display->drawXBMP(x - icon_width, 0, icon_width, icon_height, align1_bits);
          else if (telInfo.aliMode == Telescope::ALIM_TWO)
            display->drawXBMP(x - icon_width, 0, icon_width, icon_height, align2_bits);
          else if (telInfo.aliMode == Telescope::ALIM_THREE)
            display->drawXBMP(x - icon_width, 0, icon_width, icon_height, align3_bits);
          x -= icon_width + 1;
        }

        if (telInfo.isGuiding())
        {
          display->drawXBMP(x - icon_width, 0, icon_width, icon_height, guiding_bits);
          x -= icon_width + 1;
        }

      }
      switch (telInfo.getError())
      {
      case Telescope::ERR_MOTOR_FAULT:
        display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrMf_bits);
        x -= icon_width + 1;
        break;
      case  Telescope::ERR_ALT:
        display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrHo_bits);
        x -= icon_width + 1;
        break;
      case Telescope::ERR_DEC:
        display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrDe_bits);
        x -= icon_width + 1;
        break;
      case Telescope::ERR_UNDER_POLE:
        display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrUp_bits);
        x -= icon_width + 1;
        break;
      case Telescope::ERR_MERIDIAN:
        display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrMe_bits);
        x -= icon_width + 1;
        break;
      default:
        break;
      }

    }

    if (page == 0)
    {

      if (telInfo.hasInfoRa && telInfo.hasInfoDec)
      {
        char Rah[3];
        char Ram[3];
        char Ras[3];
        char decsign[2];
        char decdeg[3];
        char decmin[3];
        char decsec[3];
        memcpy(Rah, telInfo.TempRa, 2);
        Rah[2] = '\0';
        memcpy(Ram, &telInfo.TempRa[3], 2);
        Ram[2] = '\0';
        memcpy(Ras, &telInfo.TempRa[6], 2);
        Ras[2] = '\0';
        memcpy(decsign, telInfo.TempDec, 1);
        decsign[1] = '\0';
        memcpy(decdeg, &telInfo.TempDec[1], 2);
        decdeg[2] = '\0';
        memcpy(decmin, &telInfo.TempDec[4], 2);
        decmin[2] = '\0';
        memcpy(decsec, &telInfo.TempDec[7], 2);
        decsec[2] = '\0';

        u8g2_uint_t y = 36;
        x = u8g2_GetDisplayWidth(u8g2);

        display->drawRA( x, y, Rah, Ram, Ras);
        u8g2_DrawUTF8(u8g2, 0, y, "RA");

        y += line_height + 4;
        u8g2_DrawUTF8(u8g2, 0, y, "Dec");
        display->drawDec( x, y, decsign, decdeg, decmin, decsec);

      }
    }
    else if (page == 1)
    {

      if (telInfo.hasInfoAz && telInfo.hasInfoAlt)
      {
        char Azdeg[4];
        char Azm[3];
        char Azs[3];
        char Altsign[2];
        char Altdeg[3];
        char Altmin[3];
        char Altsec[3];
        memcpy(Azdeg, telInfo.TempAz, 3);
        Azdeg[3] = '\0';
        memcpy(Azm, &telInfo.TempAz[4], 2);
        Azm[2] = '\0';
        memcpy(Azs, &telInfo.TempAz[7], 2);
        Azs[2] = '\0';
        memcpy(Altsign, telInfo.TempAlt, 1);
        Altsign[1] = '\0';
        memcpy(Altdeg, &telInfo.TempAlt[1], 2);
        Altdeg[2] = '\0';
        memcpy(Altmin, &telInfo.TempAlt[4], 2);
        Altmin[2] = '\0';
        memcpy(Altsec, &telInfo.TempAlt[7], 2);
        Altsec[2] = '\0';

        u8g2_uint_t y = 36;
        u8g2_uint_t startpos = u8g2_GetUTF8Width(u8g2, "123456");
        x = startpos;
        x = u8g2_GetDisplayWidth(u8g2);
        u8g2_DrawUTF8(u8g2, 0, y, "Az.");
        display->drawAz(x, y, Azdeg, Azm, Azs);

        y += line_height + 4;
        x = startpos;
        x = u8g2_GetDisplayWidth(u8g2);

        display->drawDec( x, y, Altsign, Altdeg, Altmin, Altsec);
        u8g2_DrawUTF8(u8g2, 0, y, "Alt.");


      }
    }
    else if (page == 2)
    {

      if (telInfo.hasInfoUTC && telInfo.hasInfoSideral)
      {
        char Rah[3];
        char Ram[3];
        char Ras[3];
        u8g2_uint_t y = 36;

        x = u8g2_GetDisplayWidth(u8g2);
        memcpy(Rah, telInfo.TempUTC, 2);
        Rah[2] = '\0';
        memcpy(Ram, &telInfo.TempUTC[3], 2);
        Ram[2] = '\0';
        memcpy(Ras, &telInfo.TempUTC[6], 2);
        Ras[2] = '\0';
        u8g2_DrawUTF8(u8g2, 0, y, "UTC");
        display->drawRA( x, y, Rah, Ram, Ras);

        y += line_height + 4;
        memcpy(Rah, telInfo.TempSideral, 2);
        Rah[2] = '\0';
        memcpy(Ram, &telInfo.TempSideral[3], 2);
        Ram[2] = '\0';
        memcpy(Ras, &telInfo.TempSideral[6], 2);
        Ras[2] = '\0';
        u8g2_DrawUTF8(u8g2, 0, y, "Sideral");
        display->drawRA(x, y, Rah, Ram, Ras);
      }
    }
    else if (page == 3)
    {
      int idx = telInfo.alignSelectedStar - 1;
      const byte* cat_letter = NULL;
      const byte*  cat_const = NULL;
      cat_letter = &Star_letter[idx];
      cat_const = &Star_constellation[idx];
      u8g2_uint_t y = 36;
      char txt[20];

      if ((telInfo.align - 1) % 3 == 1)
      {
        sprintf(txt, "Slew to Star %u", (telInfo.align - 1) / 3 + 1);
      }
      else if ((telInfo.align - 1) % 3 == 2)
      {
        sprintf(txt, "Recenter Star %u", (telInfo.align - 1) / 3 + 1);
      }
      u8g2_DrawUTF8(u8g2, 0, y, txt);
      y += line_height + 4;
      const uint8_t* myfont = u8g2->font;
      u8g2_SetFont(u8g2, u8g2_font_unifont_t_greek);
      u8g2_DrawGlyph(u8g2, 0, y, 944 + *cat_letter);
      u8g2_SetFont(u8g2, myfont);
      u8g2_DrawUTF8(u8g2, 16, y, constellation_txt[*cat_const - 1]);
    }

  } while (u8g2_NextPage(u8g2));
  lastpageupdate = millis();
}

void SmartHandController::drawIntro()
{
  display->firstPage();
  do {
    display->drawXBMP(0, 0, teenastro_width, teenastro_height, teenastro_bits);
  } while (display->nextPage());
  delay(2000);
}

void SmartHandController::drawLoad()
{
  display->firstPage();
  uint8_t x = 0;
  do {
    display->setFont(u8g2_font_helvR14_tr);
    x = (display->getDisplayWidth() - display->getStrWidth("Loading")) / 2;
    display->drawStr(x, display->getDisplayHeight()/2. - 14, "Loading");
    x = (display->getDisplayWidth() - display->getStrWidth("Version 0.0")) / 2;
    display->drawStr(x, display->getDisplayHeight() / 2. + 14, "Version 0.0");
  } while (display->nextPage());
}

void SmartHandController::drawReady()
{
  display->firstPage();
  uint8_t x = 0;
  do {
    x = (display->getDisplayWidth() - display->getStrWidth("Ready!")) / 2;
    display->drawStr(x, display->getDisplayHeight() / 2, "Ready!");
  } while (display->nextPage());
  delay(500);
}

bool SmartHandController::menuSetStepperGearBox(uint8_t &axis, unsigned short &worm)
{
  char text[20];
  float stepperGearBox = 10;
  sprintf(text, "Gear box M%u", axis);
  if (display->UserInterfaceInputValueFloat(&buttonPad,text, "Ratio", &stepperGearBox, 1, 100, 5, 0, ""))
  {
    return DisplayMessageLX200(writeTotGearLX200(axis, stepperGearBox * worm),false);
  }
}

bool SmartHandController::menuSetReverse(uint8_t &axis)
{
  bool reverse;
  if (!DisplayMessageLX200(readReverseLX200(axis, reverse)))
    return false;
  char text[20];
  char * string_list_micro = "Direct\nReversed";
  sprintf(text, "Rotation M%u", axis);
  uint8_t choice = display->UserInterfaceSelectionList(&buttonPad, text, (uint8_t)reverse + 1, string_list_micro);
  if (choice)
  {
    reverse = (bool)(choice - 1);
    return DisplayMessageLX200(writeReverseLX200(axis, reverse),false);
  }
  return true;
}
bool SmartHandController::menuSetBacklash(uint8_t &axis)
{
  float backlash;
  if (!DisplayMessageLX200(readBacklashLX200(axis, backlash)))
    return false;
  char text[20];
  sprintf(text, "Backlash M%u", axis);
  if (display->UserInterfaceInputValueFloat(&buttonPad, text, "", &backlash, 0, 1000, 4, 0, " in seconds"))
  {
    return DisplayMessageLX200(writeBacklashLX200(axis, backlash),false);
  }
  return true;
}
bool SmartHandController::menuSetTotGear(uint8_t &axis)
{
  float totGear;
  if (!DisplayMessageLX200(readTotGearLX200(axis, totGear)))
    return false;
  char text[20];
  sprintf(text, "Gear M%u", axis);
  if (display->UserInterfaceInputValueFloat(&buttonPad, text, "Ratio", &totGear, 1, 10000, 5, 0, ""))
  {
    return DisplayMessageLX200(writeTotGearLX200(axis, totGear), false);
  }
  return true;
}
bool SmartHandController::menuSetStepPerRot(uint8_t &axis)
{
  float stepPerRot;
  if (!DisplayMessageLX200(readStepPerRotLX200(axis, stepPerRot)))
    return false;
  char text[20];
  sprintf(text, "Stepper M%u", axis);
  if (display->UserInterfaceInputValueFloat(&buttonPad, text, "", &stepPerRot, 1, 400, 3, 0, " Steps"))
  {
    return DisplayMessageLX200(writeStepPerRotLX200(axis, stepPerRot),false);
  }
  return true;
}
bool SmartHandController::menuSetMicro(uint8_t &axis)
{
  uint8_t microStep;
    if (!DisplayMessageLX200(readMicroLX200(axis, microStep)))
      return false;
  char text[20];
  char * string_list_micro = "16 (~256)\n32\n64\n128\n256";
  sprintf(text, "Stepper M%u", axis);
  uint8_t choice = microStep - 4 + 1;
  choice = display->UserInterfaceSelectionList(&buttonPad, text, choice, string_list_micro);
  if (choice)
  {
    microStep = choice - 1 + 4;
    return DisplayMessageLX200(writeMicroLX200(axis, microStep),false);
  }
  return true;
}
bool SmartHandController::menuSetLowCurrent(uint8_t &axis)
{
  uint8_t lowCurr;
  if (!DisplayMessageLX200(readLowCurrLX200(axis, lowCurr)))
  {
    return false;
  }
  char text[20];
  sprintf(text, "Low Curr. M%u", axis);
  if (display->UserInterfaceInputValueInteger(&buttonPad, text, "", &lowCurr, 10, 200, 3, "0 mA"))
  {
    return DisplayMessageLX200(writeLowCurrLX200(axis, lowCurr),false);
  }
  return true;
}

bool SmartHandController::menuSetHighCurrent(uint8_t &axis)
{
  uint8_t highCurr;
  if (!DisplayMessageLX200(readHighCurrLX200(axis, highCurr)))
  {
    return false;
  }
  char text[20];
  sprintf(text, "High Curr. M%u", axis);
  if (display->UserInterfaceInputValueInteger(&buttonPad, text, "", &highCurr, 10, 200, 3, "0 mA"))
  {
    return DisplayMessageLX200(writeHighCurrLX200(axis, highCurr));
  }
  return true;
}

void SmartHandController::DisplayMotorSettings(uint8_t &axis)
{
  char line1[32]="";
  char line2[32]="";
  char line3[32]="";
  char line4[32]="";
  bool reverse;
  float backlash,totGear,stepPerRot;
  uint8_t microStep,lowCurr,highCurr;
  sprintf(line1, "Motor %u Settings", axis);
  if (DisplayMessageLX200(readReverseLX200(axis, reverse)))
  {
    reverse ? sprintf(line3, "Reversed Rotation") : sprintf(line3, "Direct Rotation");
  }
  if (DisplayMessageLX200(readTotGearLX200(axis, totGear)))
  {
    sprintf(line4, "Ratio: %u", (unsigned int)totGear);
  }
  DisplayLongMessage(line1, NULL, line3, line4, -1);

  line2[0] = 0;
  line3[0] = 0;
  line4[0] = 0;
  if (DisplayMessageLX200(readStepPerRotLX200(axis, stepPerRot)))
  {
    sprintf(line2, "%u Steps per Rot.", (unsigned int)stepPerRot);
  }
  if (DisplayMessageLX200(readMicroLX200(axis, microStep)))
  {
    sprintf(line3, "MicroStep: %u", (unsigned int)pow(2, microStep));
  }
  if (DisplayMessageLX200(readBacklashLX200(axis, backlash)))
  {
    sprintf(line4, "Backlash: %u sec.", (unsigned int)backlash);
  }
  DisplayLongMessage(line1, line2, line3, line4, -1);
  line2[0] = 0;
  line3[0] = 0;
  line4[0] = 0;
  if (DisplayMessageLX200(readLowCurrLX200(axis, lowCurr)))
  {
    sprintf(line3, "Low Curr. %u0 mA", (unsigned int)lowCurr);
  }
  if (DisplayMessageLX200(readHighCurrLX200(axis, highCurr)))
  {
    sprintf(line4, "High Curr. %u0 mA", (unsigned int)highCurr);
  }

  DisplayLongMessage(line1, NULL, line3, line4, -1);
}


void SmartHandController::menuMain()
{
  current_selection_L0 = 1;
  while (current_selection_L0 != 0)
  {
    telInfo.updateTel();
    Telescope::ParkState currentstate = telInfo.getParkState();

    if (currentstate == Telescope::PRK_PARKED)
    {
      const char *string_list_main_ParkedL0 = "Unpark\n""Settings";
      current_selection_L0 = display->UserInterfaceSelectionList(&buttonPad, "Main Menu", 0, string_list_main_ParkedL0);
      switch (current_selection_L0)
      {
      case 1:
        break;
      case 2:
        menuSettings();
        break;
      default:
        break;
      }
    }
    else if (currentstate == Telescope::PRK_UNPARKED)
    {
      const char *string_list_main_UnParkedL0 = "Goto\nSync\nTracking\nSide of Pier\nSettings";
      current_selection_L0 = display->UserInterfaceSelectionList(&buttonPad, "Main Menu", 0, string_list_main_UnParkedL0);
      switch (current_selection_L0)
      {
      case 1:
        menuSyncGoto(false);
        break;
      case 2:
        menuSyncGoto(true);
        break;
      case 3:
        menuTrack();
        break;
      case 4:
        menuPier();
        break;
      case 5:
        menuSettings();
        break;
      default:
        break;
      }
    }
  }
}

void SmartHandController::menuSpeedRate()
{
  char * string_list_Speed = "0.25x\n0.5x\n1.0x\n2.0x\n4.0x\n16.0x\n32.0x\n64.0x\n0.5 Max\nMax";
  current_selection_speed = display->UserInterfaceSelectionList(&buttonPad, "Set Speed", current_selection_speed, string_list_Speed);
  if (current_selection_speed > 0)
  {
    char cmd[5]= ":Rn#";
    cmd[2] = '0' + current_selection_speed - 1;
    DisplayMessageLX200(SetLX200(cmd));
  }
}

void SmartHandController::menuTrack()
{
  telInfo.updateTel();
  Telescope::TrackState currentstate = telInfo.getTrackingState();
  uint8_t choice;
  if (currentstate == Telescope::TRK_ON)
  {
    const char *string_list_main_ParkedL0 = "Stop Tracking\n";
    choice = display->UserInterfaceSelectionList(&buttonPad, "Tracking State", 0, string_list_main_ParkedL0);
    switch (choice)
    {
    case 1:
      char out[20];
      memset(out, 0, sizeof(out));
      if (SetLX200(":Td#")== LX200VALUESET)
      {
        DisplayMessage("Tracking", "OFF", 500);
      }
      else
      {
        DisplayMessage("Set State", "Failed", 1000);
      }
      break;
    default:
      break;
    }
  }
  else if (currentstate == Telescope::TRK_OFF)
  {
    const char *string_list_main_UnParkedL0 = "Start Tracking\n";
    current_selection_L0 = display->UserInterfaceSelectionList(&buttonPad, "Tracking State", 0, string_list_main_UnParkedL0);
    switch (current_selection_L0)
    {
    case 1:
      if (SetLX200(":Te#") == LX200VALUESET)
      {
        DisplayMessage("Tracking", "ON", 500);
      }
      else
      {
        DisplayMessage("Set State", "Failed", 1000);
      }
      break;
    default:
      break;
    }
  }
  else
  {
    DisplayMessage("currently Tracking", "cannot be changed", 1000);
  }
}

void SmartHandController::menuSyncGoto(bool sync)
{
  current_selection_L1 = 1;
  while (current_selection_L1 != 0)
  {
    const char *string_list_gotoL1 = "Messier\nStar\nSolar System\nHerschel\nCoordinates\nHome\nPark";
    current_selection_L1 = display->UserInterfaceSelectionList(&buttonPad, sync ? "Sync" : "Goto", current_selection_L1, string_list_gotoL1);
    switch (current_selection_L1)
    {
    case 1:
      menuMessier(sync);
      break;
    case 2:
      menuStar(sync);
      break;
    case 3:
      menuSolarSys(sync);
      break;
    case 4:
      menuHerschel(sync);
      break;
    case 5:
      menuRADec(sync);
      break;
    case 6:
    {
      char cmd[5];
      sprintf(cmd, ":hX#");
      cmd[2] = sync ? 'F' : 'C';
      if (SetLX200(cmd) == LX200VALUESET)
      {
        DisplayMessage(sync ? "Reset at" : "Goto", " Home Position", -1);
      }
      // Quit Menu
      current_selection_L1 = 0;
      current_selection_L0 = 0;
    }
    break;
    case 7:
    {
      char cmd[5];
      sprintf(cmd, ":hX#");
      cmd[2] = sync ? 'O' : 'P';
      if (SetLX200(cmd) == LX200VALUESET)
      {
        DisplayMessage(sync ? "Reset at" : "Goto", " Park Position", -1);
      }
      else
      {
        DisplayMessageLX200(LX200NOTOK);
      }
      // Quit Menu
      current_selection_L1 = 0;
      current_selection_L0 = 0;
    }
    break;
    default:
      break;
    }
  }
}

void SmartHandController::menuSolarSys(bool sync)
{
  current_selection_SolarSys = max(current_selection_SolarSys, 1);
  const char *string_list_SolarSyst = "Sun\nMercure\nVenus\nMars\nJupiter\nSaturn\nUranus\nNeptun\nMoon\n";
  current_selection_SolarSys = display->UserInterfaceSelectionList(&buttonPad, sync ? "Sync" : "Goto", current_selection_SolarSys, string_list_SolarSyst);
  if (current_selection_SolarSys == 0)
  {
    return;
  }
  current_selection_SolarSys > 3 ? current_selection_SolarSys : current_selection_SolarSys--;
  bool ok = DisplayMessageLX200(SyncGotoPlanetLX200(sync, current_selection_SolarSys),false);
  if (current_selection_SolarSys != 0 && ok)
  {
    // Quit Menu
    current_selection_L1 = 0;
    current_selection_L0 = 0;
  }
}

void SmartHandController::menuHerschel(bool sync)
{
  current_selection_Herschel = display->UserInterfaceCatalog(&buttonPad, sync ? "Sync Herschel" : "Goto Herschel", current_selection_Herschel, HERSCHEL);
  if (current_selection_Herschel != 0)
  {
    bool ok = DisplayMessageLX200(SyncGotoCatLX200(sync, HERSCHEL, current_selection_Herschel - 1),false);
    if (ok)
    {
      // Quit Menu
      current_selection_L1 = 0;
      current_selection_L0 = 0;
    }
  }
}

void SmartHandController::menuMessier(bool sync)
{
  current_selection_Messier = display->UserInterfaceCatalog(&buttonPad, sync ? "Sync Messier" : "Goto Messier", current_selection_Messier, MESSIER);
  if (current_selection_Messier != 0)
  {
    bool ok = DisplayMessageLX200(SyncGotoCatLX200(sync, MESSIER, current_selection_Messier - 1), false);
    if (ok)
    {
      // Quit Menu
      current_selection_L1 = 0;
      current_selection_L0 = 0;
    }
  }
}

void SmartHandController::menuAlignment()
{
  current_selection_L1 = 1;
  while (current_selection_L1 != 0)
  {
    const char *string_list_AlignmentL1 = "1-Star Align.\n""2-Star Align.\n""3-Star Align.\n""Show Corr.\n""Clear Corr.";
    current_selection_L1 = display->UserInterfaceSelectionList(&buttonPad, "Alignment", current_selection_L1, string_list_AlignmentL1);
    switch (current_selection_L1)
    {
    case 1:
      if (SetLX200(":A1#") == LX200VALUESET)
      {
        telInfo.aliMode = Telescope::ALIM_ONE;
        telInfo.align = Telescope::ALI_SELECT_STAR_1;
      }
      else
      {
        DisplayMessage("Alignment", "Failed!", -1);
      }
      break;
    case 2:
      if (SetLX200(":A2#") == LX200VALUESET)
      {
        telInfo.aliMode = Telescope::ALIM_TWO;
        telInfo.align = Telescope::ALI_SELECT_STAR_1;
      }
      else
      {
        DisplayMessage("Alignment", "Failed!", -1);
      }
      break;
    case 3:
      if (SetLX200(":A3#") == LX200VALUESET)
      {
        telInfo.aliMode = Telescope::ALIM_THREE;
        telInfo.align = Telescope::ALI_SELECT_STAR_1;
      }
      else
      {
        DisplayMessage("Alignment", "Failed!", -1);
      }
      break;
    case 4:
      double val1;
      double val2;
      double val3;
      double val4;

      //DisplaylongMessage("", "", "", "",0);

      break;
    case 5:
      break;
    default:
      break;
    }
    // Quit Menu
    current_selection_L1 = 0;
    current_selection_L0 = 0;
  }
}

void SmartHandController::menuPier()
{
  telInfo.updateTel();
  uint8_t choice = ((uint8_t)telInfo.getPierState());
  choice = display->UserInterfaceSelectionList(&buttonPad, "Set Side of Pier", choice, "East\nWest");
  bool ok = false;
  if (choice)
  {
    if (choice == 1)
      ok = DisplayMessageLX200(SetLX200(":SmE#"),false);
    else
      ok = DisplayMessageLX200(SetLX200(":SmW#"),false);
    if (ok)
    {
      DisplayMessage("Please Sync", "with a Target", 1000);
      menuSyncGoto(true);
      current_selection_L1 = 0;
      current_selection_L0 = 0;
      DisplayMessageLX200(SetLX200(":SmN#"),false);
    }
  }
}

void SmartHandController::menuStar(bool sync)
{
  current_selection_Star = display->UserInterfaceCatalog(&buttonPad, sync ? "Sync Star" : "Goto Star", current_selection_Star, STAR);
  if (current_selection_Star != 0)
  {
    bool  ok = DisplayMessageLX200(SyncGotoCatLX200(sync, STAR, current_selection_Star - 1),false);
    if (ok)
    {
      // Quit Menu
      current_selection_L1 = 0;
      current_selection_L0 = 0;
    }
  }
}

bool SmartHandController::SelectStarAlign()
{
  telInfo.alignSelectedStar = display->UserInterfaceCatalog(&buttonPad, "select Star", telInfo.alignSelectedStar, STAR);
  if (telInfo.alignSelectedStar != 0)
  {
    bool ok = DisplayMessageLX200(SyncSelectedStarLX200(telInfo.alignSelectedStar),false);
    return ok;
  }
  return false;
}

void SmartHandController::menuRADec(bool sync)
{
  if (display->UserInterfaceInputValueRA(&buttonPad, &angleRA))
  {
    uint8_t vr1, vr2, vr3, vd2, vd3;
    short vd1;
    gethms(angleRA, vr1, vr2, vr3);
    if (display->UserInterfaceInputValueDec(&buttonPad, &angleDEC))
    {
      getdms(angleDEC, vd1, vd2, vd3);
      bool ok = DisplayMessageLX200(SyncGotoLX200(sync, vr1, vr2, vr3, vd1, vd2, vd3));
      if (ok)
      {
        // Quit Menu
        current_selection_L1 = 0;
        current_selection_L0 = 0;
      }
    }
  }
}

void SmartHandController::menuSettings()
{
  current_selection_L1 = 1;
  while (current_selection_L1 != 0)
  {
    const char *string_list_SettingsL1 = "Display\n""Alignment\n""Date\n""Time\n""Mount\n""Site\n""Limits\n""Wifi";
    current_selection_L1 = display->UserInterfaceSelectionList(&buttonPad, "Settings", current_selection_L1, string_list_SettingsL1);
    switch (current_selection_L1)
    {
    case 1:
      menuDisplay();
      break;
    case 2:
      menuAlignment();
      break;
    case 3:
      menuDate();
      break;
    case 4:
      menuUTCTime();
      break;
    case 5:
      menuMount();
      break;
    case 6:
      menuSite();
      break;
    case 7:
      menuLimits();
      break;
    case 8:
      menuWifi();
    default:
      break;
    }
  }
}

void SmartHandController::menuMount()
{
  current_selection_L2 = 1;
  while (current_selection_L2 != 0)
  {
    const char *string_list_Mount = "Predefined\n""Mount type\n""Motor 1\n""Motor 2";
    current_selection_L2 = display->UserInterfaceSelectionList(&buttonPad, "Mount", current_selection_L2, string_list_Mount);
    switch (current_selection_L2)
    {
    case 1:
      menuPredefinedMount();
      break;
    case 2:
      menuMountType();
      break;
    case 3:
      menuMotor(1);
      break;
    case 4:
      menuMotor(2);
      break;
    default:
      break;
    }
  }

}

void SmartHandController::menuMountType()
{
  current_selection_L3 = 1;
  while (current_selection_L3 != 0)
  {
    const char *string_list_Mount = "Equatorial Fork\n""German Equatorial";
    current_selection_L3 = display->UserInterfaceSelectionList(&buttonPad, "Mount Type", current_selection_L3, string_list_Mount);
    switch (current_selection_L3)
    {
    case 1:
      break;
    case 2:
      break;
    default:
      break;
    }
  }

}

void SmartHandController::menuPredefinedMount()
{
  current_selection_L3 = 1;
  while (current_selection_L3 != 0)
  {
    const char *string_list_Mount = "Alt Mount\n""ASTRO-PHYSICS\n""Fornax\n""Losmandy\n""Sideres85\n""Sky-Watcher\n""Takahashi\n""Vixen";
    current_selection_L3 = display->UserInterfaceSelectionList(&buttonPad, "Mount", current_selection_L3, string_list_Mount);
    switch (current_selection_L3)
    {
    case 1:
      menuAltMount();
      break;
    case 2:

      break;
    case 3:

      break;
    case 4:

      break;
    case 5:

      break;
    case 6:

    default:
      break;
    }
  }

}

void SmartHandController::menuAltMount()
{
  current_selection_L4 = 1;
  while (current_selection_L4 != 0)
  {
    const char *string_list_Mount = "Alt 5-ADN\n""Alt 6-ADN\n""Alt 7-ADN\n";
    current_selection_L4 = display->UserInterfaceSelectionList(&buttonPad, "Alt Mount", current_selection_L4, string_list_Mount);
    //if (current_selection_L4 > 0)
    //{
    //  switch (current_selection_L4)
    //  {
    //  case 1:
    //    mountMotor[0].setWorm(250);
    //    mountMotor[1].setWorm(200);
    //    break;
    //  case 2:
    //    mountMotor[0].setWorm(220);
    //    mountMotor[1].setWorm(200);
    //    break;
    //  case 3:
    //    mountMotor[0].setWorm(270);
    //    mountMotor[1].setWorm(210);
    //    break;
    //  default:
    //    break;
    //  }
    //  DisplayMessage("Set Parameters", "for Motor 1", -1);

    //  DisplayMessage("Set Parameters", "for Motor 2", -1);

    //}
  }
}

//void SmartHandController::menuAP() {}
//void SmartHandController::menuFornax() {}
//void SmartHandController::menuLosmandy() {}
//void SmartHandController::menuSideres() {}
//void SmartHandController::menuSkyWatcher() {}
//void SmartHandController::menuTakahashi() {}
//void SmartHandController::menuVixen() {}

void SmartHandController::menuMotor(uint8_t axis)
{
  current_selection_L3 = 1;

  while (current_selection_L3 != 0)
  {
    const char *string_list_Motor = "Show Settings\nRotation\nGear\n""Steps per Rotation\n""Micro Steps\n""Backlash\n""Low Current\n""High Current";
    current_selection_L3 = display->UserInterfaceSelectionList(&buttonPad, axis == 1 ? "Motor 1" : "Motor 2", current_selection_L3, string_list_Motor);
    switch (current_selection_L3)
    {
    case 1:
      //menuDisplayMotorSettings(axis);
      break;
    case 2:
      menuSetReverse(axis);
      break;
    case 3:
      menuSetTotGear(axis);
      break;
    case 4:
      menuSetStepPerRot(axis);
      break;
    case 5:
      menuSetMicro(axis);
      break;
    case 6:
      menuSetBacklash(axis);
      break;
    case 7:
      menuSetLowCurrent(axis);
      break;
    case 8:
      menuSetHighCurrent(axis);
      break;
    default:
      break;
    }
  }

}

void SmartHandController::menuSite()
{
  current_selection_L2 = 1;
  while (current_selection_L2 != 0)
  {
    const char *string_list_SiteL2 = "Latitude\n""Longitude\n""Select Site";
    current_selection_L2 = display->UserInterfaceSelectionList(&buttonPad, "Menu Site", current_selection_L2, string_list_SiteL2);
    switch (current_selection_L2)
    {
    case 1:
      menuLatitude();
      break;
    case 2:
      menuLongitude();
      break;
    case 3:
      menuSites();
      break;
    default:
      break;
    }
  }
}

void SmartHandController::menuSites()
{
  char out[20];
  int val;

  if (DisplayMessageLX200(GetSiteLX200(val)))
  {
    current_selection_L3 = val;
    const char *string_list_SiteL3 = "Site 0\n""Site 1\n""Site 2\n""Site 3";
    current_selection_L3 = display->UserInterfaceSelectionList(&buttonPad, "Menu Sites", current_selection_L3, string_list_SiteL3);
    if (current_selection_L3 != 0)
    {
      val = current_selection_L3 - 1;
      SetSiteLX200(val);
    }
  }
}

void SmartHandController::menuUTCTime()
{
  long value;
  if (DisplayMessageLX200(GetTimeLX200(value)))
  {
    if (display->UserInterfaceInputValueUTCTime(&buttonPad, &value))
    {
      DisplayMessageLX200(SetTimeLX200(value),false);
    }
  }
}

void SmartHandController::menuDisplay()
{
  const char *string_list_Display = "Turn Off\nContrast";
  current_selection_L2 = 1;
  while (current_selection_L2 != 0)
  {
    current_selection_L2 = display->UserInterfaceSelectionList(&buttonPad, "Display", current_selection_L2, string_list_Display);
    switch (current_selection_L2)
    {
    case 1:
      DisplayMessage("Press any key", "to turn on", 1500);
      sleepDisplay = true;
      display->sleepOn();
      current_selection_L2 = 0;
      current_selection_L1 = 0;
      current_selection_L0 = 0;
      break;
    case 2:
      menuContrast();
      break;
    default:
      break;
    }
  }
}

void SmartHandController::menuContrast()
{
  const char *string_list_Display = "Min\nLow\nHigh\nMax";
  current_selection_L3 = 1;

  current_selection_L3 = display->UserInterfaceSelectionList(&buttonPad, "Set Contrast", current_selection_L3, string_list_Display);
  if (current_selection_L3 > 0)
  {
    maxContrast = (uint)63 * (current_selection_L3 - 1);
    //EEPROM.write(14, maxContrast);
    //EEPROM.commit();
    display->setContrast(maxContrast);
  }
}

void SmartHandController::menuDate()
{
  char out[20];
  if (DisplayMessageLX200(GetLX200(":GC#", out)))
  {
    char* pEnd;
    uint8_t month = strtol(&out[0], &pEnd, 10);
    uint8_t day = strtol(&out[3], &pEnd, 10);
    uint8_t year = strtol(&out[6], &pEnd, 10);
    if (display->UserInterfaceInputValueDate(&buttonPad, "Date", year, month, day))
    {
      sprintf(out, ":SC%02d/%02d/%02d#", month, day, year);
      DisplayMessageLX200(SetLX200(out),false);
    }
  }
}

void SmartHandController::menuLatitude()
{
  char out[20];
  if (DisplayMessageLX200(GetLX200(":Gt#", out)))
  {
    char* pEnd;
    int degree = (int)strtol(&out[0], &pEnd, 10);
    int minute = (int)strtol(&out[4], &pEnd, 10);
    long angle = degree * 60;
    degree > 0 ? angle += minute : angle -= minute;
    angle *= 60;
    if (display->UserInterfaceInputValueLatitude(&buttonPad, &angle))
    {
      angle /= 60;
      minute = abs(angle % 60);
      degree = angle / 60;
      sprintf(out, ":St%+03d*%02d#", degree, minute);
      DisplayMessageLX200(SetLX200(out),false);
    }
  }
}

void SmartHandController::menuLongitude()
{
  char out[20];
  if (DisplayMessageLX200(GetLX200(":Gg#", out)))
  {
    char* pEnd;
    int degree = (int)strtol(&out[0], &pEnd, 10);
    int minute = (int)strtol(&out[5], &pEnd, 10);
    long angle = degree * 60;
    degree > 0 ? angle += minute : angle -= minute;
    angle *= 60;
    if (display->UserInterfaceInputValueLongitude(&buttonPad, &angle))
    {
      angle /= 60;
      minute = abs(angle) % 60;
      degree = angle / 60;
      sprintf(out, ":Sg%+04d*%02d#", degree, minute);
      DisplayMessageLX200(SetLX200(out),false);
    }
  }
}

void SmartHandController::menuLimits()
{
  const char *string_list_LimitsL2 = "Horizon\n""Overhead\n""Meridian";
  current_selection_L3 = 1;
  while (current_selection_L3 != 0)
  {
    current_selection_L3 = display->UserInterfaceSelectionList(&buttonPad, "Limits", current_selection_L3, string_list_LimitsL2);
    switch (current_selection_L3)
    {
    case 1:
      menuHorizon();
      break;
    case 2:
      menuOverhead();
      break;
    case 3:
      menuMeridian();
      break;
    default:
      break;
    }
  }
}

void SmartHandController::menuWifi()
{
  const char *string_list = (wifiOn) ? "Wifi off\nShow Password\nReset to factory" : "wifi on\nShow Password\nReset to factory";
  current_selection_L2 = 1;
  while (current_selection_L2 != 0)
  {
    current_selection_L2 = display->UserInterfaceSelectionList(&buttonPad, "Wifi", 1, string_list);
    switch (current_selection_L2)
    {
    case 1:
      wifiOn = !wifiOn;
      //EEPROM.write(15, wifiOn);
      //EEPROM.commit();
      DisplayMessage("Please", "Reboot!", 3000);
      current_selection_L2 = 0;
      current_selection_L1 = 0;
      current_selection_L0 = 0;
      powerCylceRequired = true;
      break;
    case 2:
      DisplayMessage("masterPassword is", "password", 1000);
    case 3:
      //EEPROM_writeInt(0, 0);
      //EEPROM.commit();
      DisplayMessage("Please", "Reboot!", 3000);
      current_selection_L2 = 0;
      current_selection_L1 = 0;
      current_selection_L0 = 0;
      powerCylceRequired = true;
    default:
      break;
    }
  }
}

void SmartHandController::menuHorizon()
{
  char out[20];
  if (DisplayMessageLX200(GetLX200(":Gh#", out)))
  {
    float angle = (float)strtol(&out[0], NULL, 10);
    if (display->UserInterfaceInputValueFloat(&buttonPad, "Horizon Limit", "", &angle, -10, 20, 2, 0, " degree"))
    {
      sprintf(out, ":Sh%+03d#", (int)angle);
      DisplayMessageLX200(SetLX200(out),false);
    }
  }
}

void SmartHandController::menuOverhead()
{
  char out[20];
  if (DisplayMessageLX200(GetLX200(":Go#", out)))
  {
    float angle = (float)strtol(&out[0], NULL, 10);
    if (display->UserInterfaceInputValueFloat(&buttonPad, "Overhead Limit", "", &angle, 60, 91, 2, 0, " degree"))
    {
      sprintf(out, ":S0%02d#", (int)angle);
      DisplayMessageLX200(SetLX200(out));
    }
  }
}

void SmartHandController::menuMeridian()
{
  uint8_t angle = 0;
  if (display->UserInterfaceInputValueInteger(&buttonPad, "Meridian Limit", "", &angle, 0, 20, 2, " degree"))
  {

  }
}



void SmartHandController::DisplayMessage(const char* txt1, const char* txt2, int duration)
{
  uint8_t x;
  uint8_t y = 40;
  display->firstPage();
  do {
    if (txt2 != NULL)
    {
      y = 50;
      x = (display->getDisplayWidth() - display->getStrWidth(txt2)) / 2;
      display->drawStr(x, y, txt2);
      y = 25;
    }
    x = (display->getDisplayWidth() - display->getStrWidth(txt1)) / 2;
    display->drawStr(x, y, txt1);
  } while (display->nextPage());
  if (duration >= 0)
    delay(duration);
  else
  {
    for (;;)
    {
      tickButtons();
      delay(50);
      if (buttonPressed())
        break;
    }
  }
}

void SmartHandController::DisplayLongMessage(const char* txt1, const char* txt2, const char* txt3, const char* txt4, int duration)
{
  display->setFont(u8g2_font_helvR10_tr);
  uint8_t h = 15;
  uint8_t x = 0;
  uint8_t y = h;
  display->firstPage();
  do {

    y = h;
    x = (display->getDisplayWidth() - display->getStrWidth(txt1)) / 2;
    display->drawStr(x, y, txt1);
    y += h;
    if (txt2 != NULL)
    {
      x = 0;
      display->drawStr(x, y, txt2);
    }
    else
    {
      y -= 7;
    }
    y += 15;
    if (txt3 != NULL)
    {
      x = 0;
      display->drawStr(x, y, txt3);
    }

    y += 15;
    if (txt4 != NULL)
    {
      x = 0;
      display->drawStr(x, y, txt4);
    }
  } while (display->nextPage());
  if (duration >= 0)
    delay(duration);
  else
  {
    for (;;)
    {
      tickButtons();
      delay(50);
      if (buttonPressed())
        break;
    }
  }

  display->setFont(u8g2_font_helvR12_te);
}

bool SmartHandController::DisplayMessageLX200(LX200RETURN val, bool silentOk)
{
  char text1[20] = "";
  char text2[20] = "";
  int time = -1;
  if (val < OK)
  {
    if (val == LX200NOTOK)
    {
      sprintf(text1, "LX200 Command");
      sprintf(text2, "has failed!");
    }
    else if (val == LX200SETVALUEFAILED)
    {
      sprintf(text1, "Set Value");
      sprintf(text2, "has failed!");
    }
    else if (val == LX200GETVALUEFAILED)
    {
      sprintf(text1, "Get Value");
      sprintf(text2, "has failed!");
    }
    else if (val == LX200SYNCFAILED)
    {
      sprintf(text1, "Sync");
      sprintf(text2, "has failed!");
    }
    else if (val == LX200SETTARGETFAILED)
    {
      sprintf(text1, "Set Target");
      sprintf(text2, "has failed!");
    }
    else if (val == LX200BELOWHORIZON)
    {
      sprintf(text1, "Target is");
      sprintf(text2, "Below Horizon!");
    }
    else if (val == LX200NOOBJECTSELECTED)
    {
      sprintf(text1, "No Object");
      sprintf(text2, "Selected!");
    }
    else if (val == LX200PARKED)
    {
      sprintf(text1, "Telescope");
      sprintf(text2, "is Parked!");
    }
    else if (val == LX200BUSY)
    {
      sprintf(text1, "Telescope");
      sprintf(text2, "is busy!");
    }
    else if (val == LX200LIMITS)
    {
      sprintf(text1, "Target");
      sprintf(text2, "outside limits");
    }
    else if (val == LX200UNKOWN)
    {
      sprintf(text1, "Unkown");
      sprintf(text2, "Error");
    }
    else
    {
      sprintf(text1, "Error");
      sprintf(text2, "-1");
    }
    DisplayMessage(text1, text2, -1);
  }
  else if (!silentOk)
  {
    time = 1000;
    if (val == OK)
    {
      sprintf(text1, "LX200 Command");
      sprintf(text2, "Done!");
    }
    else if (val == LX200VALUESET)
    {
      sprintf(text1, "Value");
      sprintf(text2, "Set!");
    }
    else if (val == LX200GETVALUEFAILED)
    {
      sprintf(text1, "Value");
      sprintf(text2, "Get!");
    }
    else if (val == LX200SYNCED)
    {
      sprintf(text1, "Telescope");
      sprintf(text2, "Synced!");
    }
    else if (LX200GOINGTO)
    {
      sprintf(text1, "Slew to");
      sprintf(text2, "Target");
    }
    DisplayMessage(text1, text2, time);
  }
  return isOk(val);
}
#pragma once

#ifndef _SETTING_ALL_h
#define _SETTING_ALL_h

#define ESP32
#include <Arduino.h>
#include "AsyncTelegram.h"

// Select camera model
//#define CAMERA_MODEL_WROVER_KIT
//#define CAMERA_MODEL_ESP_EYE
//#define CAMERA_MODEL_M5STACK_PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE
#define CAMERA_MODEL_AI_THINKER
//select pin for flash led
#define FLASH_LED 4
//select sd card reader
// Define where store images (on board SD card reader or internal flash memory)
#define USE_MMC true
#ifdef USE_MMC
#include <SD_MMC.h> // Use onboard SD Card reader
fs::FS &filesystem = SD_MMC;
#else
#include <FFat.h> // Use internal flash memory
extern fs::FS &filesystem = FFat; // Is necessary select the proper partition scheme (ex. "Default 4MB with ffta..")
#endif
// You only need to format FFat when error on mount (don't work with MMC SD card)
#define FORMAT_FS_IF_FAILED true
#define FILENAME_SIZE 20
#define KEEP_IMAGE true

#define LIGHT_ON_CALLBACK "lightON"		// callback data sent when "LIGHT ON" button is pressed
#define LIGHT_OFF_CALLBACK "lightOFF" // callback data sent when "LIGHT OFF" button is pressed

extern const uint8_t LED_BUILTIN = 33;

// Struct for saving time datas (needed for time-naming the image files)
extern struct tm timeinfo;
//select time zone
extern const char *timeZona = "EET-2EEST,M3.5.0/3,M10.5.0/4";

#ifdef DEFINE_VARIABLES
#define EXTERN /* пустота */
#else
#define EXTERN extern
#endif /* DEFINE_VARIABLES */

EXTERN int global_variable;

#endif

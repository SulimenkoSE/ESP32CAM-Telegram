#pragma once

#ifndef _SETTING_ALL_h
#define _SETTING_ALL_h

#define ESP32
#include <Arduino.h>
#include <WiFi.h>
//camera
#include "esp_http_server.h"
#include "esp_timer.h"
#include "esp_camera.h"
#include "img_converters.h"
#include "sensor.h"
#include "fb_gfx.h"
#include "fd_forward.h"
#include "fr_forward.h"
//filesysytems
#ifdef USE_MMC
#include <SD_MMC.h> // Use onboard SD Card reader
#else
#include <FFat.h> // Use internal flash memory
// Is necessary select the proper partition scheme (ex. "Default 4MB with ffta..")
#endif
#include "FS.h"
#include <time.h>

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
//#define USE_MMC true
// You only need to format FFat when error on mount (don't work with MMC SD card)
#define FORMAT_FS_IF_FAILED false
#define FILENAME_SIZE 20
#define KEEP_IMAGE true

#define LIGHT_ON_CALLBACK "lightON"		// callback data sent when "LIGHT ON" button is pressed
#define LIGHT_OFF_CALLBACK "lightOFF" // callback data sent when "LIGHT OFF" button is pressed

#define LED_BUILTIN 33
/* Setting timezona in Sd_card.cpp
To ensure certificate validation, WiFiClientSecure needs time updated. 
To choose correct time zone, Follow this link 
https://sites.google.com/a/usapiens.com/opnode/time-zones 
and replace String value inside
for Kyiv, Ukraine select time zone
const char *timeZona = "EET-2EEST,M3.5.0/3,M10.5.0/4"; */
extern const char *timeZona;
// REPLACE myToken WITH YOUR TELEGRAM BOT TOKEN in file AssyncRutine.cpp
//extern const char *token;
//AsyncTelegram variable bool isKeyboardActive = false;

extern fs::FS &filesystem;

#endif

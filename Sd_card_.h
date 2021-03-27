#pragma once

#ifndef _SD_CARD_h
#define _SD_CARD_h

void listDir(fs::FS &fs, const char *dirname, uint8_t levels);
void listDir_time(fs::FS &fs, const char *dirname, uint8_t levels);
void Sd_init();
String savePhoto();
#endif
#include "Sd_card_.h"

// List all files saved in the selected filesystem
void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
	Serial.printf("Listing directory: %s\r\n", dirname);

	File root = fs.open(dirname);
	if (!root)
	{
		Serial.println("- failed to open directory");
		return;
	}
	if (!root.isDirectory())
	{
		Serial.println(" - not a directory");
		return;
	}

	File file = root.openNextFile();
	while (file)
	{
		if (file.isDirectory())
		{
			Serial.printf("  DIR : %s", file.name());
			if (levels)
				listDir(fs, file.name(), levels - 1);
		}
		else
			Serial.printf("  FILE: %s\tSIZE: %d\n", file.name(), file.size());

		file = root.openNextFile();
	}
}

void listDir_teme(fs::FS &fs, const char *dirname, uint8_t levels)
{
	Serial.printf("Listing directory: %s\n", dirname);

	File root = fs.open(dirname);
	if (!root)
	{
		Serial.println("Failed to open directory");
		return;
	}
	if (!root.isDirectory())
	{
		Serial.println("Not a directory");
		return;
	}

	File file = root.openNextFile();
	while (file)
	{
		if (file.isDirectory())
		{
			Serial.print("  DIR : ");
			Serial.print(file.name());
			time_t t = file.getLastWrite();
			getLocalTime(&timeinfo);
			Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (timeinfo.tm_year) + 1900, (timeinfo.tm_mon) + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
			if (levels)
			{
				listDir(fs, file.name(), levels - 1);
			}
		}
		else
		{
			Serial.print("  FILE: ");
			Serial.print(file.name());
			Serial.print("  SIZE: ");
			Serial.print(file.size());
			time_t t = file.getLastWrite();
			struct tm *tmstruct = localtime(&t);
			Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (timeinfo.tm_year) + 1900, (timeinfo.tm_mon) + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
		}
		file = root.openNextFile();
	}
}

void Sd_init()
{

	if (!SD_MMC.begin())
	{
		Serial.println("Card Mount Failed");
		return;
	}
	uint8_t cardType = SD_MMC.cardType();

	if (cardType == CARD_NONE)
	{
		Serial.println("No SD card attached");
		return;
	}

	Serial.print("SD Card Type: ");
	if (cardType == CARD_MMC)
	{
		Serial.println("MMC");
	}
	else if (cardType == CARD_SD)
	{
		Serial.println("SDSC");
	}
	else if (cardType == CARD_SDHC)
	{
		Serial.println("SDHC");
	}
	else
	{
		Serial.println("UNKNOWN");
	}

	uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
	Serial.printf("SD Card Size: %lluMB\n", cardSize);
}

String savePhoto(camera_fb_t *fbc)
{
	// Set filename with current timestamp "YYYYMMDD_HHMMSS.jpg"
	char pictureName[FILENAME_SIZE];
	getLocalTime(&timeinfo);
	snprintf(pictureName, FILENAME_SIZE, "%02d%02d%02d_%02d%02d%02d.jpg", timeinfo.tm_year + 1900,
					 timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

	// Path where new picture will be saved
	String path = "/";
	path += String(pictureName);
	File file = filesystem.open(path.c_str(), FILE_WRITE);
	if (!file)
	{
		Serial.println("Failed to open file in writing mode");
		return "";
	}
#ifdef USE_MMC
	uint64_t freeBytes = SD_MMC.totalBytes() - SD_MMC.usedBytes();
#else
	uint64_t freeBytes = FFat.freeBytes();
#endif

	if (freeBytes > fbc->len)
	{
		file.write(fbc->buf, fbc->len); // payload (image), payload length
		Serial.printf("Saved file to path: %s\n", path.c_str());
		file.close();
		return path;
	}
	else
	{
		Serial.println("Not enough space avalaible");
		return "";
	}
}
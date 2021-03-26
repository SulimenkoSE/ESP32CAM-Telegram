/*
 const char *ssid = "Nina_Iot";                                        // REPLACE mySSID WITH YOUR WIFI SSID
const char *pass = "Sergik9876632";                                   // REPLACE myPassword YOUR WIFI PASSWORD, IF ANY
const char *token = "1554215186:AAEbD7gVmPe8dlIbMob_PsGV7vaE_L6bVHk"; // REPLACE myToken WITH YOUR TELEGRAM BOT TOKEN
*/
/*
  Name:        keyboards.ino
  Created:     20/06/2020
  Author:      Tolentino Cotesta <cotestatnt@yahoo.com>
  Description: a more complex example that do:
             1) if a "/inline_keyboard" text message is received, show the inline custom keyboard,
                if a "/reply_keyboard" text message is received, show the reply custom keyboard,
                otherwise reply the sender with "Try /reply_keyboard or /inline_keyboard" message
             2) if "LIGHT ON" inline keyboard button is pressed turn on the LED and show a message
             3) if "LIGHT OFF" inline keyboard button is pressed, turn off the LED and show a message
             4) if "GitHub" inline keyboard button is pressed,
                open a browser window with URL "https://github.com/cotestatnt/AsyncTelegram"
*/
#include "Setting_All.h"
#include "Sd_card_.h"
#include "camera.h"
#include "TelegramBot.h"

#include "soc/soc.h"          // Brownout error fix
#include "soc/rtc_cntl_reg.h" // Brownout error fix

const char *ssid = "Nina_Iot";      // REPLACE mySSID WITH YOUR WIFI SSID
const char *pass = "Sergik9876632"; // REPLACE myPassword YOUR WIFI PASSWORD, IF ANY

void setup()
{
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
                                             //pinMode 15 red;
  pinMode(LED_BUILTIN, OUTPUT);

  // initialize the Serial
  Serial.begin(115200);

  Serial.printf("setup() running on core  %d\n", xPortGetCoreID());
  Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
  Serial.print("\n\nStart connection to WiFi...");
  delay(100);

  // connects to access point
  WiFi.begin(ssid, pass);
  delay(500);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(500);
  }
  Serial.print("\nWiFi connected: ");
  Serial.println(WiFi.localIP());

  //initializing Sd
  Sd_init();
  //initializing camera
  camera_Init();
  //initializing Bot Telegram
  Bot_Init();
  //initializing rteboard in Bot Telegram
  KeyboardButon_Init();
  KeyboardInline_Init();
}

void loop()
{

  // In the meantime LED_BUILTIN will blink with a fixed frequency
  // to evaluate async and non-blocking working of library
  static uint32_t ledTime = millis();
  if (millis() - ledTime > 1000)
  {
    ledTime = millis();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    Bot_Message();
  }
  //
}
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
#include "camera.h"
#include "Sd_card_.h"
#include "AssyncRutine.h"

//camera
//#include "camera.h"

#include "soc/soc.h"          // Brownout error fix
#include "soc/rtc_cntl_reg.h" // Brownout error fix

const char *ssid = "Nina_Iot";      // REPLACE mySSID WITH YOUR WIFI SSID
const char *pass = "Sergik9876632"; // REPLACE myPassword YOUR WIFI PASSWORD, IF ANY

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  //pinMode(LED, OUTPUT);
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
  Serial.print(WiFi.localIP());

  /* // To ensure certificate validation, WiFiClientSecure needs time updated
  // myBot.setInsecure(false);
  myBot.setClock(timeZona);

  // Set the Telegram bot properies
  myBot.setUpdateTime(1000);
  myBot.setTelegramToken(token);

  // Check if all things are ok
  Serial.print("\nTest Telegram connection... ");
  myBot.begin() ? Serial.println("OK") : Serial.println("NOK");

  // Add reply keyboard
  isKeyboardActive = false;
  // add a button that send a message with "Simple button" text
  myReplyKbd.addButton("PHOTO", KeyboardButtonPhoto);
  myReplyKbd.addButton("Button2");
  myReplyKbd.addButton("Button3");
  // add a new empty button row
  myReplyKbd.addRow();
  // add another button that send the user position (location)
  myReplyKbd.addButton("Send Location", KeyboardButtonLocation);
  // add another button that send the user contact
  myReplyKbd.addButton("Send contact", KeyboardButtonContact);
  // add a new empty button row
  myReplyKbd.addRow();
  // add a button that send a message with "Hide replyKeyboard" text
  // (it will be used to hide the reply keyboard)
  myReplyKbd.addButton("/hide_keyboard");
  // resize the keyboard to fit only the needed space
  myReplyKbd.enableResize();

  // Add sample inline keyboard
  myInlineKbd.addButton("ON", LIGHT_ON_CALLBACK, KeyboardButtonQuery);
  myInlineKbd.addButton("OFF", LIGHT_OFF_CALLBACK, KeyboardButtonQuery);
  myInlineKbd.addRow();
  myInlineKbd.addButton("GitHub", "https://github.com/", KeyboardButtonURL); */
  asynk_Init();

  //Init Sd
  Sd_init();
}

void loop()
{

  // In the meantime LED_BUILTIN will blink with a fixed frequency
  // to evaluate async and non-blocking working of library
  static uint32_t ledTime = millis();
  if (millis() - ledTime > 200)
  {
    ledTime = millis();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }

  // a variable to store telegram message data
  TBMessage msg;

  // if there is an incoming message...
  if (myBot.getNewMessage(msg))
  {
    // check what kind of message I received
    String tgReply;
    MessageType msgType = msg.messageType;

    switch (msgType)
    {
    case MessageText:
      // received a text message
      tgReply = msg.text;
      Serial.print("\nText message received: ");
      Serial.println(tgReply);

      // check if is show keyboard command
      if (tgReply.equalsIgnoreCase("/reply_keyboard"))
      {
        // the user is asking to show the reply keyboard --> show it
        myBot.sendMessage(msg, "This is reply keyboard:", myReplyKbd);
        isKeyboardActive = true;
      }
      else if (tgReply.equalsIgnoreCase("/inline_keyboard"))
      {
        myBot.sendMessage(msg, "This is inline keyboard:", myInlineKbd);
      }

      // check if the reply keyboard is active
      else if (isKeyboardActive)
      {
        // is active -> manage the text messages sent by pressing the reply keyboard buttons
        if (tgReply.equalsIgnoreCase("/hide_keyboard"))
        {
          // sent the "hide keyboard" message --> hide the reply keyboard
          myBot.removeReplyKeyboard(msg, "Reply keyboard removed");
          isKeyboardActive = false;
        }
        else
        {
          // print every others messages received
          myBot.sendMessage(msg, msg.text);
        }
      }

      // the user write anything else and the reply keyboard is not active --> show a hint message
      else
      {
        myBot.sendMessage(msg, "Try /reply_keyboard or /inline_keyboard");
      }
      break;

    case MessageQuery:
      // received a callback query message
      tgReply = msg.callbackQueryData;
      Serial.print("\nCallback query message received: ");
      Serial.println(tgReply);

      if (tgReply.equalsIgnoreCase(LIGHT_ON_CALLBACK))
      {
        // pushed "LIGHT ON" button...
        Serial.println("\nSet light ON");
        digitalWrite(LED_BUILTIN, HIGH);
        // terminate the callback with an alert message
        myBot.endQuery(msg, "Light on", true);
      }
      else if (tgReply.equalsIgnoreCase(LIGHT_OFF_CALLBACK))
      {
        // pushed "LIGHT OFF" button...
        Serial.println("\nSet light OFF");
        digitalWrite(LED_BUILTIN, LOW);
        // terminate the callback with a popup message
        myBot.endQuery(msg, "Light off");
      }

      break;

    case MessageLocation:
      // received a location message --> send a message with the location coordinates
      char bufL[50];
      snprintf(bufL, sizeof(bufL), "Longitude: %f\nLatitude: %f\n", msg.location.longitude, msg.location.latitude);
      myBot.sendMessage(msg, bufL);
      Serial.println(bufL);
      break;

    case MessageContact:
      char bufC[50];
      snprintf(bufC, sizeof(bufC), "Contact information received: %s - %s\n", msg.contact.firstName, msg.contact.phoneNumber);
      // received a contact message --> send a message with the contact information
      myBot.sendMessage(msg, bufC);
      Serial.println(bufC);
      break;
    case MessagePhoto:
      /* char bufC[50];
      snprintf(bufC, sizeof(bufC), "Contact information received: %s - %s\n", msg.contact.firstName, msg.contact.phoneNumber);
      // received a contact message --> send a message with the contact information
      myBot.sendMessage(msg, bufC);
      Serial.println(bufC); */
      break;
    default:
      break;
    }
  }
}
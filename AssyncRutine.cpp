#include "AssyncRutine.h"

const char *token = "1554215186:AAEbD7gVmPe8dlIbMob_PsGV7vaE_L6bVHk"; // REPLACE myToken WITH YOUR TELEGRAM BOT TOKEN

AsyncTelegram myBot;
ReplyKeyboard myReplyKbd;		// reply keyboard object helper
InlineKeyboard myInlineKbd; // inline keyboard object helper

bool isKeyboardActive; // store if the reply keyboard is shown

void asynk_Init()
{
	// To ensure certificate validation, WiFiClientSecure needs time updated
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
	myInlineKbd.addButton("GitHub", "https://github.com/", KeyboardButtonURL);
}
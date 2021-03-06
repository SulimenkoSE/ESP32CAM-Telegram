#include "AsyncTelegram.h"
#include "TelegramBot.h"
#include "Setting_All.h"
#include "Sd_card_.h"

const char *token = "***********:***********************************"; // REPLACE myToken WITH YOUR TELEGRAM BOT TOKEN

AsyncTelegram myBot;
ReplyKeyboard myReplyKbd;		// reply keyboard object helper
InlineKeyboard myInlineKbd; // inline keyboard object helper
bool isKeyboardActive = false;
// store if the reply keyboard is shown

void Bot_Init()
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
}

void KeyboardButon_Init()
{
	// add a button that send a message with "Simple button" text
	myReplyKbd.addButton("PHOTO");
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
}

void KeyboardInline_Init()
{
	// Add sample inline keyboard
	myInlineKbd.addButton("ON", LIGHT_ON_CALLBACK, KeyboardButtonQuery);
	myInlineKbd.addButton("OFF", LIGHT_OFF_CALLBACK, KeyboardButtonQuery);
	myInlineKbd.addRow();
	myInlineKbd.addButton("GitHub", "https://github.com/", KeyboardButtonURL);
}

void Bot_Message()
{
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
					if (tgReply == "PHOTO")
					{
						myBot.sendMessage(msg, "PHOTO");
						// Take picture and save to file
						String myFile = savePhoto();
						if (myFile != "")
						{
							if (!myBot.sendPhotoByFile(msg.sender.id, myFile, filesystem))
								Serial.println("Photo send failed");
							//If you don't need to keep image in memory, delete it
							if (KEEP_IMAGE == false)
							{
								filesystem.remove("/" + myFile);
							}
						}
					}
					else
					{
						// print every others messages received
						myBot.sendMessage(msg, "msg.text line 103");
					}
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
			char bufML[50];
			snprintf(bufML, sizeof(bufML), "Longitude: %f\nLatitude: %f\n", msg.location.longitude, msg.location.latitude);
			myBot.sendMessage(msg, bufML);
			Serial.println(bufML);
			break;

		case MessageContact:
			char bufMC[50];
			snprintf(bufMC, sizeof(bufMC), "Contact information received: %s - %s\n", msg.contact.firstName, msg.contact.phoneNumber);
			// received a contact message --> send a message with the contact information
			myBot.sendMessage(msg, bufMC);
			Serial.println(bufMC);
			break;

		default:
			break;
		}
	}
}

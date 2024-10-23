/**
 * @file gui.cpp
 * @author PV
 * 
 * This is a test of GUI for Hat Day game to show off all of the features of ESPUI. 
 * This can be built using the Arduino IDE, or PlatformIO.
 * 
 * ---------------------------------------------------------------------------------------
 * If you just want to see examples of the ESPUI code, jump down to the setUpUI() function
 * ---------------------------------------------------------------------------------------
 * 
 * When this program boots, it will load an SSID and password from the EEPROM. 
 * The SSID is a null-terminated C string stored at EEPROM addresses 0-31
 * The password is a null-terminated C string stored at EEPROM addresses 32-95.
 * If these credentials do not work for some reason, the ESP will create an Access 
 * Point wifi with the SSID HOSTNAME (defined below). You can then connect and use 
 * the controls on the "Wifi Credentials" tab to store credentials into the EEPROM.
 * 
 */

#include "gui.h"



//UI handles
uint16_t wifi_ssid_text, wifi_pass_text;
uint16_t mainLabel, mainSwitcher, mainSlider, mainText, mainNumber, mainScrambleButton, mainTime;
uint16_t styleButton, styleLabel, styleSwitcher, styleSlider, styleButton2, styleLabel2, styleSlider2;
uint16_t graph;
volatile bool updates = false;

bool buttonPressed = false;



// This is the main function which builds our GUI
void setUpUI() {

#ifdef ESP8266
    { HeapSelectIram doAllocationsInIRAM;
#endif

	//Turn off verbose debugging
	ESPUI.setVerbosity(Verbosity::Quiet);

	//Make sliders continually report their position as they are being dragged.
	ESPUI.sliderContinuous = true;

	//This GUI is going to be a tabbed GUI, so we are adding most controls using ESPUI.addControl
	//which allows us to set a parent control. If we didn't need tabs we could use the simpler add
	//functions like:
	//    ESPUI.button()
	//    ESPUI.label()


	/*
	 * Tab: Basic Controls
	 * This tab contains all the basic ESPUI controls, and shows how to read and update them at runtime.
	 *-----------------------------------------------------------------------------------------------------------*/
	auto maintab = ESPUI.addControl(Tab, "", "Game controls");

	ESPUI.addControl(Separator, "Hat Day Racing. GiddyUp!", "", None, maintab);
	ESPUI.addControl(Button, "Race Control", "Start", Alizarin, maintab, extendedCallback, (void*)19);
	mainLabel = ESPUI.addControl(Label, "Label", "Label text", Emerald, maintab, generalCallback);
	mainSwitcher = ESPUI.addControl(Switcher, "Switcher", "", Sunflower, maintab, generalCallback);

	//Sliders default to being 0 to 100, but if you want different limits you can add a Min and Max control
	mainSlider = ESPUI.addControl(Slider, "Slider", "200", Turquoise, maintab, generalCallback);
	ESPUI.addControl(Min, "", "10", None, mainSlider);
	ESPUI.addControl(Max, "", "400", None, mainSlider);

	//These are the values for the selector's options. (Note that they *must* be declared static
	//so that the storage is allocated in global memory and not just on the stack of this function.)
	static String optionValues[] {"Value 1", "Value 2", "Value 3", "Value 4", "Value 5"};
	auto mainselector = ESPUI.addControl(Select, "Selector", "Selector", Wetasphalt, maintab, generalCallback);
	for(auto const& v : optionValues) {
		ESPUI.addControl(Option, v.c_str(), v, None, mainselector);
	}

	mainText = ESPUI.addControl(Text, "Text Input", "Initial value", Alizarin, maintab, generalCallback);

	//Number inputs also accept Min and Max components, but you should still validate the values.
	mainNumber = ESPUI.addControl(Number, "Number Input", "42", Emerald, maintab, generalCallback);
	ESPUI.addControl(Min, "", "10", None, mainNumber);
	ESPUI.addControl(Max, "", "50", None, mainNumber);



	
	//Finally, start up the UI. 
	//This should only be called once we are connected to WiFi.
	ESPUI.begin(HOSTNAME);

#ifdef ESP8266
    } // HeapSelectIram
#endif

}



//Most elements in this test UI are assigned this generic callback which prints some
//basic information. Event types are defined in ESPUI.h
void generalCallback(Control *sender, int type) {
	Serial.print("CB: id(");
	Serial.print(sender->id);
	Serial.print(") Type(");
	Serial.print(type);
	Serial.print(") '");
	Serial.print(sender->label);
	Serial.print("' = ");
	Serial.println(sender->value);
}

// Most elements in this test UI are assigned this generic callback which prints some
// basic information. Event types are defined in ESPUI.h
// The extended param can be used to hold a pointer to additional information
// or for C++ it can be used to return a this pointer for quick access
// using a lambda function
void extendedCallback(Control* sender, int type, void* param)
{
    Serial.print("CB: id(");
    Serial.print(sender->id);
    Serial.print(") Type(");
    Serial.print(type);
    Serial.print(") '");
    Serial.print(sender->label);
    Serial.print("' = ");
    Serial.println(sender->value);
    Serial.print("param = ");
    Serial.println((long)param);
	if((type == 1) && (sender->id == 3)) {
		Serial.println("Got the right combo!");
		buttonPressed = true;
	}
}

bool chk_buttonPressed() {
	bool temp;
	temp = buttonPressed;
	buttonPressed = false;		//always re-enable the button
	
	return temp;
}







//Utilities
//
//If you are here just to see examples of how to use ESPUI, you can ignore the following functions
//------------------------------------------------------------------------------------------------
void readStringFromEEPROM(String& buf, int baseaddress, int size) {
	buf.reserve(size);
	for (int i = baseaddress; i < baseaddress+size; i++) {
		char c = EEPROM.read(i);
		buf += c;
		if(!c) break;
	}	
}

void connectWifi() {
	int connect_timeout;


#if defined(ESP32)
	WiFi.setHostname(HOSTNAME);
#else
	WiFi.hostname(HOSTNAME);
#endif
	Serial.println("Begin wifi...");

	//Load credentials from EEPROM 
	if(!(FORCE_USE_HOTSPOT)) {
		yield();
		EEPROM.begin(100);
		String stored_ssid, stored_pass;
		readStringFromEEPROM(stored_ssid, 0, 32);
		readStringFromEEPROM(stored_pass, 32, 96);
		EEPROM.end();
	
		//Try to connect with stored credentials, fire up an access point if they don't work.
		#if defined(ESP32)
			WiFi.begin(stored_ssid.c_str(), stored_pass.c_str());
		#else
			WiFi.begin(stored_ssid, stored_pass);
		#endif
		connect_timeout = 28; //7 seconds
		while (WiFi.status() != WL_CONNECTED && connect_timeout > 0) {
			delay(250);
			Serial.print(".");
			connect_timeout--;
		}
	}
	
	if (WiFi.status() == WL_CONNECTED) {
		Serial.println(WiFi.localIP());
		Serial.println("Wifi started");

		if (!MDNS.begin(HOSTNAME)) {
			Serial.println("Error setting up MDNS responder!");
		}
	} else {
		Serial.println("\nCreating access point...");
		WiFi.mode(WIFI_AP);
		WiFi.softAPConfig(IPAddress(192, 168, 1, 1), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
		WiFi.softAP(HOSTNAME);

		connect_timeout = 20;
		do {
			delay(250);
			Serial.print(",");
			connect_timeout--;
		} while(connect_timeout);
	}
}

void enterWifiDetailsCallback(Control *sender, int type) {
	if(type == B_UP) {
		Serial.println("Saving credentials to EPROM...");
		Serial.println(ESPUI.getControl(wifi_ssid_text)->value);
		Serial.println(ESPUI.getControl(wifi_pass_text)->value);
		unsigned int i;
		EEPROM.begin(100);
		for(i = 0; i < ESPUI.getControl(wifi_ssid_text)->value.length(); i++) {
			EEPROM.write(i, ESPUI.getControl(wifi_ssid_text)->value.charAt(i));
			if(i==30) break; //Even though we provided a max length, user input should never be trusted
		}
		EEPROM.write(i, '\0');

		for(i = 0; i < ESPUI.getControl(wifi_pass_text)->value.length(); i++) {
			EEPROM.write(i + 32, ESPUI.getControl(wifi_pass_text)->value.charAt(i));
			if(i==94) break; //Even though we provided a max length, user input should never be trusted
		}
		EEPROM.write(i + 32, '\0');
		EEPROM.end();
	}
}

void textCallback(Control *sender, int type) {
	//This callback is needed to handle the changed values, even though it doesn't do anything itself.
}

void randomString(char *buf, int len) {
	for(auto i = 0; i < len-1; i++) 
		buf[i] = random(0, 26) + 'A';
	buf[len-1] = '\0';
}

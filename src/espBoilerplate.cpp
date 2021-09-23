#ifndef espBoilerplate_cpp
#define espBoilerplate_cpp
#include "espBoilerplate.h"
//Constructor function
espBoilerplateClass::espBoilerplateClass()
{
	//Do nothing
}
//Destructor function
espBoilerplateClass::~espBoilerplateClass()
{
	//Do nothing
}
bool espBoilerplateClass::begin(char* SSID, char* PSK)
{
	WiFi.persistent(false);
	Serial.print(F("\n\nESP MAC address:"));
	Serial.println(WiFi.macAddress());
	WiFi.begin(SSID, PSK);
	Serial.print(F("Trying to connect to \""));
	Serial.print(SSID);
	Serial.print('"');
	uint8_t retries = connectionRetries;
	while(WiFi.status() != WL_CONNECTED && retries > 0)
	{
		Serial.print('.');
		delay(connectionRetryFrequency);
		retries--;
	}
	Serial.println();
	if(WiFi.status() == WL_CONNECTED)
	{
		//WiFi.onStationModeDisconnected(&espBoilerplateOnStationDisconnected);
		printConnectionStatus();
		printIpStatus();
		return(true);
	}
	else
	{
		printConnectionStatus();
		return(false);
	}
}
void espBoilerplateClass::printIpStatus()
{
	Serial.print(F("IP address:"));
	Serial.print(WiFi.localIP());
	Serial.print(F(" Subnet mask:"));
	Serial.println(WiFi.subnetMask());
	Serial.print(F("Gateway:"));
	Serial.print(WiFi.gatewayIP());
	Serial.print(F(" DNS:"));
	Serial.println(WiFi.dnsIP());
	Serial.print(F("Hostname:"));
	Serial.println(WiFi.hostname());
}
void espBoilerplateClass::printConnectionStatus()
{
	switch (WiFi.status()) {
		case WL_IDLE_STATUS:
		Serial.println(F("Idle"));
		break;
		case WL_NO_SSID_AVAIL:
		Serial.println(F("SSID unavailable"));
		break;
		case WL_CONNECTED:
		Serial.print(F("Connected to SSID:\""));
		Serial.print(WiFi.SSID());
		Serial.print("\" BSSID:\"");
		Serial.print(WiFi.BSSIDstr());
		Serial.println('"');
		Serial.print(F("RSSI:"));
		Serial.print(WiFi.RSSI());
		Serial.print(F(" Channel:"));
		Serial.print(WiFi.channel());
		Serial.print(F(" Phy:"));
		switch (WiFi.getPhyMode()){
			case WIFI_PHY_MODE_11B:
			Serial.println(F("802.11b"));
			break;
			case WIFI_PHY_MODE_11G:
			Serial.println(F("802.11g"));
			break;
			case WIFI_PHY_MODE_11N:
			Serial.println(F("802.11n"));
			break;
			default:
			Serial.println(F("Unknown"));
			break;
		}
		break;
		case WL_CONNECT_FAILED:
		Serial.println(F("Connection failed"));
		break;
		case WL_WRONG_PASSWORD:
		Serial.println(F("Wrong PSK"));
		break;
		case WL_DISCONNECTED:
		Serial.println(F("Disconnected"));
		break;
		default:
		Serial.println(F("Unknown"));
		break;
	}
}
espBoilerplateClass espBoilerplate;
#endif
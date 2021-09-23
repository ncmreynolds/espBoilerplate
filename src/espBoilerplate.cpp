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
bool espBoilerplateClass::setOutputStream(Stream &streamToUse)
{
	if(&streamToUse != nullptr)
	{
		_outputStream = &streamToUse;		//Set the stream used for the terminal
		return(true);
	}
	else
	{
		return(false);
	}
}
void espBoilerplateClass::setRetries(uint8_t retries)
{
	connectionRetries = retries;
}
bool espBoilerplateClass::begin(char* SSID, char* PSK)
{
	if(_outputStream == nullptr)	//Check there's an output Stream set
	{
		_outputStream = &Serial;		//Assume Serial for output
	}
	WiFi.persistent(false);										//Avoid flash wear
	WiFi.setAutoReconnect(true);								//Because why not?
	_outputStream->print(F("\n\nRestart reason:"));
	_outputStream->println(ESP.getResetReason());
	#ifdef ESP8266
	_outputStream->print(F("ESP8266 MAC address:"));
	#elif ESP32
	_outputStream->print(F("ESP32 MAC address:"));
	#endif
	_outputStream->println(WiFi.macAddress());
	WiFi.begin(SSID, PSK);
	_outputStream->print(F("Trying to connect to SSID:\""));
	_outputStream->print(SSID);
	_outputStream->print(F("\" PSK:\""));
	_outputStream->print(WiFi.psk());
	_outputStream->print("\"\nTimeout:");
	_outputStream->print((connectionRetries*connectionRetryFrequency)/1000);
	_outputStream->print('s');
	uint8_t retries = connectionRetries;
	while((WiFi.status() == WL_DISCONNECTED || WiFi.status() == WL_IDLE_STATUS) && retries > 0)
	{
		retries--;
		_outputStream->print('.');
		delay(connectionRetryFrequency);
	}
	_outputStream->println();
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
	_outputStream->print(F("IP address:"));
	_outputStream->print(WiFi.localIP());
	_outputStream->print(F(" Subnet mask:"));
	_outputStream->println(WiFi.subnetMask());
	_outputStream->print(F("Gateway:"));
	_outputStream->print(WiFi.gatewayIP());
	_outputStream->print(F(" DNS:"));
	_outputStream->print(WiFi.dnsIP());
	_outputStream->print('/');
	_outputStream->println(WiFi.dnsIP(1));
	_outputStream->print(F("Hostname:"));
	_outputStream->println(WiFi.hostname());
}
void espBoilerplateClass::printConnectionStatus()
{
	switch (WiFi.status()) {
		case WL_IDLE_STATUS:
		_outputStream->println(F("Idle"));
		break;
		case WL_NO_SSID_AVAIL:
		_outputStream->println(F("SSID unavailable"));
		break;
		case WL_CONNECTED:
		_outputStream->print(F("Connected to SSID:\""));
		_outputStream->print(WiFi.SSID());
		_outputStream->print("\" BSSID:\"");
		_outputStream->print(WiFi.BSSIDstr());
		_outputStream->println('"');
		_outputStream->print(F("RSSI:"));
		_outputStream->print(WiFi.RSSI());
		_outputStream->print(F(" Channel:"));
		_outputStream->print(WiFi.channel());
		_outputStream->print(F(" Phy:"));
		switch (WiFi.getPhyMode()){
			case WIFI_PHY_MODE_11B:
			_outputStream->println(F("802.11b"));
			break;
			case WIFI_PHY_MODE_11G:
			_outputStream->println(F("802.11g"));
			break;
			case WIFI_PHY_MODE_11N:
			_outputStream->println(F("802.11n"));
			break;
			default:
			_outputStream->println(F("Unknown"));
			break;
		}
		break;
		case WL_CONNECT_FAILED:
		_outputStream->println(F("Connection failed"));
		break;
		case WL_WRONG_PASSWORD:
		_outputStream->println(F("Wrong PSK"));
		break;
		case WL_DISCONNECTED:
		_outputStream->println(F("Disconnected"));
		break;
		default:
		_outputStream->println(F("Unknown"));
		break;
	}
}
espBoilerplateClass espBoilerplate;
#endif
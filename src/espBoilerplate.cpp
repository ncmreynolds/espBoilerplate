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

void espBoilerplateClass::setHostname(String name)
{
	uint32_t length = name.length();
	if(name.length() > 0)
	{
		if(_outputStream != nullptr)
		{
			_outputStream->print(F("Setting hostname:"));
		}
		if(length > 64)
		{
			length = 64;
			hostname = new char[length + 1];
			String tempString = name.substring(0, length - 1);
			tempString.toCharArray(hostname, length + 1);
		}
		else
		{
			hostname = new char[length + 1];
			name.toCharArray(hostname, length + 1);
		}
		if(_outputStream != nullptr)
		{
			_outputStream->println(hostname);
		}
	}
}

void espBoilerplateClass::setHostname(char *name)
{
	if(name != nullptr)
	{
		if(_outputStream != nullptr)
		{
			_outputStream->print(F("Setting hostname:"));
		}
		uint32_t length = strlen(name);
		if(length > 64)
		{
			length = 64;
		}
		hostname = new char[length + 1];
		strncpy(hostname, name, length);
		hostname[length  + 1] = char(0);
		if(_outputStream != nullptr)
		{
			_outputStream->println(hostname);
		}
	}
}

void espBoilerplateClass::printIpStatus()
{
	if((WiFi.getMode() == WIFI_MODE_STA || WiFi.getMode() == WIFI_MODE_APSTA) && WiFi.status() == WL_CONNECTED)
	{
		_outputStream->println(F("-----------WiFi Client--------------"));
		_outputStream->print(F("Client IP address:"));
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
		#if defined (ESP8266)
		_outputStream->println(WiFi.hostname());
		#elif defined (ESP32)
		_outputStream->println(WiFi.getHostname());
		#endif
	}
	if(WiFi.getMode() == WIFI_MODE_AP || WiFi.getMode() == WIFI_MODE_APSTA)
	{
		_outputStream->println(F("-----------WiFi AP--------------"));
		_outputStream->print(F("AP IP address:"));
		_outputStream->print(WiFi.softAPIP());
		_outputStream->print(F(" Subnet mask:"));
		_outputStream->print(WiFi.softAPNetworkID());
		_outputStream->print('/');
		_outputStream->println(WiFi.softAPSubnetCIDR());
	}
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
		#if defined (ESP8266)
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
		#elif defined (ESP32)
		wifi_ap_record_t *ap_info;
		if(esp_wifi_sta_get_ap_info(ap_info) == ESP_OK)
		{
			if(ap_info->phy_11b)
			{
				_outputStream->println(F("802.11b"));
			}
			if(ap_info->phy_11g)
			{
				_outputStream->println(F("802.11b"));
			}
			if(ap_info->phy_11n)
			{
				_outputStream->println(F("802.11b"));
			}
			if(ap_info->phy_lr)
			{
				_outputStream->println(F("802.11 lr"));
			}
		}
		else
		{
			_outputStream->println(F("Unknown"));
		}
		#endif
		break;
		case WL_CONNECT_FAILED:
		_outputStream->println(F("Connection failed"));
		break;
		#if defined(ESP8266)
		case WL_WRONG_PASSWORD:
		_outputStream->println(F("Wrong PSK"));
		break;
		#elif defined(ESP32)
		case WL_SCAN_COMPLETED:
		_outputStream->println(F("Scan completed"));
		break;
		case WL_CONNECTION_LOST:
		_outputStream->println(F("Connection lost"));
		break;
		#endif
		case WL_DISCONNECTED:
		_outputStream->println(F("Disconnected"));
		break;
		default:
		_outputStream->println(F("Unknown"));
		break;
	}
}

#ifdef ESP32
void espBoilerplateClass::es32printResetReason(uint8_t core)
{
	switch (rtc_get_reset_reason(core))
	{
		case 1 : _outputStream->println ("POWERON_RESET");break;          /**<1,  Vbat power on reset*/
		case 3 : _outputStream->println ("SW_RESET");break;               /**<3,  Software reset digital core*/
		case 4 : _outputStream->println ("OWDT_RESET");break;             /**<4,  Legacy watch dog reset digital core*/
		case 5 : _outputStream->println ("DEEPSLEEP_RESET");break;        /**<5,  Deep Sleep reset digital core*/
		case 6 : _outputStream->println ("SDIO_RESET");break;             /**<6,  Reset by SLC module, reset digital core*/
		case 7 : _outputStream->println ("TG0WDT_SYS_RESET");break;       /**<7,  Timer Group0 Watch dog reset digital core*/
		case 8 : _outputStream->println ("TG1WDT_SYS_RESET");break;       /**<8,  Timer Group1 Watch dog reset digital core*/
		case 9 : _outputStream->println ("RTCWDT_SYS_RESET");break;       /**<9,  RTC Watch dog Reset digital core*/
		case 10 : _outputStream->println ("INTRUSION_RESET");break;       /**<10, Instrusion tested to reset CPU*/
		case 11 : _outputStream->println ("TGWDT_CPU_RESET");break;       /**<11, Time Group reset CPU*/
		case 12 : _outputStream->println ("SW_CPU_RESET");break;          /**<12, Software reset CPU*/
		case 13 : _outputStream->println ("RTCWDT_CPU_RESET");break;      /**<13, RTC Watch dog Reset CPU*/
		case 14 : _outputStream->println ("EXT_CPU_RESET");break;         /**<14, for APP CPU, reseted by PRO CPU*/
		case 15 : _outputStream->println ("RTCWDT_BROWN_OUT_RESET");break;/**<15, Reset when the vdd voltage is not stable*/
		case 16 : _outputStream->println ("RTCWDT_RTC_RESET");break;      /**<16, RTC Watch dog reset digital core and rtc module*/
		default : _outputStream->println ("NO_MEAN");
	}
}
#endif
espBoilerplateClass espBoilerplate;
#endif
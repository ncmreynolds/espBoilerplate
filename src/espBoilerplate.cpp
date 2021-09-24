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
bool espBoilerplateClass::begin(const char* SSID, const char* PSK)
{
	if(_outputStream == nullptr)								//Check there's an output Stream set
	{
		_outputStream = &Serial;								//Assume Serial for output
	}
	WiFi.persistent(false);										//Avoid flash wear
	WiFi.setAutoReconnect(true);								//Because why not?
	#ifdef ESP_IDF_VERSION_MAJOR
		_outputStream->print(F("\n\nIDF version:"));			
		#ifdef ESP_IDF_VERSION_MINOR
			_outputStream->print(ESP_IDF_VERSION_MAJOR);
			_outputStream->print('.');
			_outputStream->println(ESP_IDF_VERSION_MINOR);
		#else
			_outputStream->println(ESP_IDF_VERSION_MAJOR);
		#endif
	#else
		_outputStream->print(F("\n\n\n"));
	#endif
	_outputStream->print(F("Core:"));
	#if defined(ESP32)
		#ifdef ESP_IDF_VERSION_MAJOR // IDF 4+
			#if CONFIG_IDF_TARGET_ESP32 // ESP32/PICO-D4
				_outputStream->println(F("ESP32"));
			#elif CONFIG_IDF_TARGET_ESP32S2
				_outputStream->println(F("ESP32S2"));
			#elif CONFIG_IDF_TARGET_ESP32C3
				_outputStream->println(F("ESP32C3"));
			#else 
				#error Target CONFIG_IDF_TARGET is not supported
			#endif
		#else // ESP32 Before IDF 4.0
			_outputStream->println(F("ESP32"));
		#endif
	#else
		_outputStream->println(F("ESP8266"));
	#endif
	//_outputStream->print(F("Sketch:"));
	//_outputStream->println(__FILE__);
	_outputStream->print(F("Compiled: "));
	_outputStream->print(__DATE__);
	_outputStream->print(' ');
	_outputStream->println(__TIME__);
	#if defined (ESP8266)
		_outputStream->print(F("Restart reason:"));
		_outputStream->println(ESP.getResetReason());
		_outputStream->print(F("ESP8266 MAC address:"));
	#elif defined(ESP32)
		#ifdef ESP_IDF_VERSION_MAJOR // IDF 4+
			#if CONFIG_IDF_TARGET_ESP32 // ESP32/PICO-D4
				_outputStream->print(F("Restart reason core 0:"));
				es32getResetReason(0);
				_outputStream->print(F("Restart reason core 1:"));
				es32getResetReason(1);
				_outputStream->print(F("ESP32 MAC address:"));
			#elif CONFIG_IDF_TARGET_ESP32S2
				_outputStream->print(F("Restart reason:"));
				es32getResetReason(0);
		_outputStream->print(F("ESP32S2 MAC address:"));
			#elif CONFIG_IDF_TARGET_ESP32C3
				_outputStream->print(F("Restart reason:"));
				es32getResetReason(0);
				_outputStream->print(F("ESP32C3 MAC address:"));
			#else 
				#error Target CONFIG_IDF_TARGET is not supported
			#endif
		#else // ESP32 Before IDF 4.0
			_outputStream->print(F("Restart reason core 0:"));
			es32getResetReason(0);
			_outputStream->print(F("Restart reason core 1:"));
			es32getResetReason(1);
		_outputStream->print(F("ESP32 MAC address:"));
		#endif
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
		//_outputStream->print(WiFi.status());
		delay(connectionRetryFrequency);
	}
	_outputStream->println();
	if(WiFi.status() == WL_CONNECTED)
	{
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
	#if defined (ESP8266)
	_outputStream->println(WiFi.hostname());
	#elif defined (ESP32)
	_outputStream->println(WiFi.getHostname());
	#endif
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
void espBoilerplateClass::es32getResetReason(uint8_t core)
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
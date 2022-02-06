#ifndef espBoilerplate_h
#define espBoilerplate_h
#if defined(ESP32)
	#include <WiFi.h>
	//#include "esp32/include/esp_wifi.h"
	#include <esp_wifi.h>
	#if ESP_IDF_VERSION_MAJOR > 3	// IDF 4+
		#if CONFIG_IDF_TARGET_ESP32 // ESP32/PICO-D4
			#include "esp32/rom/rtc.h"
		#elif CONFIG_IDF_TARGET_ESP32S2
			#include "esp32s2/rom/rtc.h"
		#elif CONFIG_IDF_TARGET_ESP32C3
			#include "esp32c3/rom/rtc.h"
		#else 
			#error Target CONFIG_IDF_TARGET is not supported
		#endif
	#else // ESP32 Before IDF 4.0
		#include "rom/rtc.h"
	#endif
#else
	#include <ESP8266WiFi.h>
#endif

class espBoilerplateClass
{
	public:
		espBoilerplateClass();						//Constructor method
		~espBoilerplateClass();						//Destructor method
		//bool begin(const char* SSID,const char* PSK);			//Default way to start Wi-Fi connection
		template <class argumentA, class argumentB, class argumentC, class argumentD>
		bool begin(argumentA SSID, argumentB PSK,argumentC APSSID, argumentD APPSK)	//Default way to start Wi-Fi connection
		{
			bool connectResult = begin(SSID, PSK, false);
			if(connectResult)
			{
				#if defined(ESP32)
					WiFi.mode(WIFI_MODE_APSTA);
				#elif defined(ESP8266)
					WiFi.mode(WIFI_AP_STA);
				#endif
			}
			else
			{
				#if defined(ESP32)
					WiFi.mode(WIFI_MODE_AP);
				#elif defined(ESP8266)
					WiFi.mode(WIFI_AP);
				#endif
			}
			if(_outputStream != nullptr)
			{
				_outputStream->print(F("Creating SoftAP SSID:\""));
				_outputStream->print(APSSID);
				_outputStream->print(F("\" PSK:\""));
				_outputStream->print(APPSK);
				_outputStream->println('"');
			}
			WiFi.softAP(APSSID, APPSK);
			if(_outputStream != nullptr)
			{
				printIpStatus();
			}
			return(connectResult);
		}
		template <class argumentA, class argumentB>
		bool begin(argumentA SSID, argumentB PSK, bool displayIpStatus = true)	//Default way to start Wi-Fi connection
		{
			/*if(_outputStream == nullptr)								//Check there's an output Stream set
			{
				_outputStream = &Serial;								//Assume Serial for output
			}*/
			WiFi.persistent(false);										//Avoid flash wear
			WiFi.setAutoReconnect(true);								//Because why not?
			if(_outputStream != nullptr)
			{
				printGeneralInfo();
			}
			if(hostname != nullptr)
			{
				#if defined(ESP32)
					//WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
					WiFi.setHostname(hostname);
				#elif defined(ESP8266)
					WiFi.hostname(hostname);
				#endif
			}
			WiFi.begin(SSID, PSK);
			if(_outputStream != nullptr)
			{
				_outputStream->print(F("Trying to connect to SSID:\""));
				_outputStream->print(SSID);
				_outputStream->print(F("\" PSK:\""));
				_outputStream->print(WiFi.psk());
				_outputStream->print("\"\nTimeout:");
				_outputStream->print((connectionRetries*connectionRetryFrequency)/1000);
				_outputStream->print(F("s "));
			}
			uint8_t retries = connectionRetries;
			#if defined(ESP8266)
			while((WiFi.status() == WL_DISCONNECTED || WiFi.status() == WL_IDLE_STATUS) && retries > 0)
			#elif defined(ESP32)
			while((WiFi.status() == WL_DISCONNECTED || WiFi.status() == WL_IDLE_STATUS || WiFi.status() == WL_CONNECT_FAILED) && retries > 0)
			#endif
			{
				retries--;
				if(_outputStream != nullptr)
				{
					switch (WiFi.status()) {
						case WL_IDLE_STATUS:
							_outputStream->print('I');
						break;
						case WL_NO_SSID_AVAIL:
							_outputStream->print('U');
						break;
						case WL_SCAN_COMPLETED:
							_outputStream->print('S');
						break;
						case WL_CONNECT_FAILED:
							_outputStream->print('F');
						break;
						case WL_CONNECTION_LOST:
							_outputStream->print('L');
						break;
						case WL_DISCONNECTED:
							_outputStream->print('D');
						break;
						default:
							_outputStream->print('?');
						break;
					}
				}
				delay(connectionRetryFrequency);
			}
			if(_outputStream != nullptr)
			{
				_outputStream->println();
			}
			if(WiFi.status() == WL_CONNECTED)
			{
				if(_outputStream != nullptr)
				{
					printConnectionStatus();
					if(displayIpStatus)
					{
						printIpStatus();
					}
				}
				return(true);
			}
			else
			{
				if(_outputStream != nullptr)
				{
					printConnectionStatus();
				}
				return(false);
			}
		}
		template <class argumentC, class argumentD>
		bool beginAp(argumentC APSSID, argumentD APPSK)	//Default way to start Wi-Fi connection
		{
			if(WiFi.status() == WL_CONNECTED)
			{
				#if defined(ESP32)
					WiFi.mode(WIFI_MODE_APSTA);
				#elif defined(ESP8266)
					WiFi.mode(WIFI_AP_STA);
				#endif
			}
			else
			{
				#if defined(ESP32)
					WiFi.mode(WIFI_MODE_AP);
				#elif defined(ESP8266)
					WiFi.mode(WIFI_AP);
				#endif
			}
			if(generalInfoPrinted == false && _outputStream != nullptr)
			{
				printGeneralInfo();
			}
			if(_outputStream != nullptr)
			{
				_outputStream->print(F("Creating SoftAP SSID:\""));
				_outputStream->print(APSSID);
				_outputStream->print(F("\" PSK:\""));
				_outputStream->print(APPSK);
				_outputStream->println('"');
			}
			WiFi.softAP(APSSID, APPSK);
			if(_outputStream != nullptr)
			{
				printIpStatus();
			}
			return(true);
		}
		bool setOutputStream(Stream &);				//Change the Stream used for output
		void setRetries(uint8_t);					//Change how many retries before a connections attemp fails
		void setHostname(char *);					//Set the hostname, chooses the right function for ESP8266/ESP32
		void setHostname(String name);				//String version of above
	protected:
	private:
		Stream *_outputStream = nullptr;			//The stream used for the terminal
		uint8_t connectionRetries = 30;				//How many times a connection retries
		uint16_t connectionRetryFrequency = 1000;	//Interval between retries in ms
		void printConnectionStatus();				//Print client status
		void printIpStatus();						//Print IP status
		void printGeneralInfo();					//Detail about IDF version etc.
		#ifdef ESP32
		void es32printResetReason(uint8_t);			//Reset reason on ESP32 which lacks the function available in ESP8266
		#endif
		char* hostname = nullptr;					//Store hostname for setting once WiFi is started
		bool generalInfoPrinted = false;
};
extern espBoilerplateClass espBoilerplate;
#endif
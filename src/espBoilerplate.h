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
		template <class argumentA, class argumentB>
		bool begin(argumentA SSID, argumentB PSK)						//Default way to start Wi-Fi connection
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
		bool setOutputStream(Stream &);				//Change the Stream used for output
		void setRetries(uint8_t);					//Change how many retries before a connections attemp fails
	protected:
	private:
		Stream *_outputStream = nullptr;			//The stream used for the terminal
		uint8_t connectionRetries = 30;				//How many times a connection retries
		uint16_t connectionRetryFrequency = 1000;	//Interval between retries in ms
		void printConnectionStatus();				//Print client status
		void printIpStatus();						//Print IP status
		#ifdef ESP32
		void es32getResetReason(uint8_t);			//Reset reason on ESP32 which lacks the function available in ESP8266
		#endif
};
extern espBoilerplateClass espBoilerplate;
#endif
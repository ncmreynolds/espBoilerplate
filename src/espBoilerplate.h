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
		bool begin(const char* SSID,const char* PSK);			//Default way to start Wi-Fi connection
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
#ifndef espBoilerplate_h
#define espBoilerplate_h
#if defined(ESP32)
	#include <WiFi.h>
#else
	#include <ESP8266WiFi.h>
#endif

class espBoilerplateClass
{
	public:
		espBoilerplateClass();						//Constructor method
		~espBoilerplateClass();						//Destructor method
		bool begin(char* SSID, char* PSK);			//Default way to start Wi-Fi connection
		bool setOutputStream(Stream &);				//Change the Stream used for output
		void setRetries(uint8_t);					//Change how many retries before a connections attemp fails
	protected:
	private:
		Stream *_outputStream = nullptr;			//The stream used for the terminal
		uint8_t connectionRetries = 30;				//How many times a connection retries
		uint16_t connectionRetryFrequency = 1000;	//Interval between retries in ms
		void printConnectionStatus();				//Print client status
		void printIpStatus();						//Print IP status
};
extern espBoilerplateClass espBoilerplate;
#endif
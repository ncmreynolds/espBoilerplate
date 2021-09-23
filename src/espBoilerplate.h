#ifndef espBoilerplate_h
#define espBoilerplate_h
#if defined(ESP32)
	#include <WiFi.h>
#else
	#include <ESP8266WiFi.h>
	#include <ESP8266mDNS.h>
#endif
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

/*WiFiEventHandler gotIpEventHandler, disconnectedEventHandler;

gotIpEventHandler = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP& event)
{
	Serial.print("Station connected, IP: ");
	Serial.println(WiFi.localIP());
});

disconnectedEventHandler = WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected& event)
{
	Serial.println("Station disconnected");
});*/


/*void espBoilerplateOnStationDisconnected(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Station disconnected");
}*/

class espBoilerplateClass
{
	public:
		espBoilerplateClass();						//Constructor method
		~espBoilerplateClass();						//Destructor method
		bool begin(char* SSID, char* PSK);			//Default way to start Wi-Fi connection
		bool setOutputStream(Stream &);				//Change the Stream used for output
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
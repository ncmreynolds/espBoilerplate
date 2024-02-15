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
	#include <HTTPClient.h>
	#include <WiFi.h>
#else
	#include <ESP8266WiFi.h>
	#include <ESP8266HTTPClient.h>
	#include <WiFiClientSecureBearSSL.h>
#endif
#include <time.h>

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
			beginAp(APSSID,APPSK, false);
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
				_outputStream->print("\" timeout:");
				_outputStream->print((connectionRetries*connectionRetryFrequency)/1000);
				_outputStream->print(F("s "));
			}
			uint8_t retries = connectionRetries;
			uint8_t errorCount = 0;
			#if defined(ESP8266)
			while((WiFi.status() == WL_DISCONNECTED || WiFi.status() == WL_IDLE_STATUS) && retries > 0)
			#elif defined(ESP32)
			while((WiFi.status() == WL_DISCONNECTED || WiFi.status() == WL_IDLE_STATUS || WiFi.status() == WL_CONNECT_FAILED) && retries > 0 && errorCount < 3)
			#endif
			{
				retries--;
				if(_outputStream != nullptr)
				{
					switch (WiFi.status()) {
						case WL_IDLE_STATUS:
							_outputStream->print('I');
							errorCount++;
						break;
						case WL_NO_SSID_AVAIL:
							_outputStream->print('U');
							errorCount++;
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
							errorCount++;
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
				if(_ntpEnabled)
				{
					_enableNtp();
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
		bool beginAp(argumentC APSSID, argumentD APPSK, bool displayIpStatus = true)	//Default way to start Wi-Fi connection
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
			if(derivedApSubnet == true)
			{
				#if defined(ESP8266)
				uint8_t macAddr[6];
				WiFi.softAPmacAddress(macAddr);
				IPAddress softApIp(10,macAddr[4],macAddr[5],1);
				IPAddress softApGateway(10,macAddr[4],macAddr[5],1);
				IPAddress softApSubnet(255,255,255,0);
				WiFi.softAPConfig (softApIp, softApGateway, softApSubnet);
				#elif defined(ESP32)
				#endif
			}
			if(_outputStream != nullptr)
			{
				_outputStream->print(F("Creating SoftAP SSID:\""));
				_outputStream->print(APSSID);
				_outputStream->print(F("\" PSK:\""));
				_outputStream->print(APPSK);
				if(softApChannel != 0)
				{
					_outputStream->print("\" channel: ");
					_outputStream->println(softApChannel);
				}
				else
				{
					_outputStream->println('"');
				}
				if(derivedApSubnet == true)
				{
					_outputStream->println(F("Enabled derived AP subnet"));
				}
			}
			if(softApChannel != 0)
			{
				WiFi.softAP(APSSID, APPSK);
			}
			else
			{
				WiFi.softAP(APSSID, APPSK, softApChannel);
			}
			if(_outputStream != nullptr && displayIpStatus == true)
			{
				printIpStatus();
			}
			return(true);
		}
		bool setOutputStream(Stream &);				//Change the Stream used for output
		void setRetries(uint16_t);					//Change how many retries before a connections attemp fails
		void setHostname(char *);					//Set the hostname, chooses the right function for ESP8266/ESP32
		void setHostname(String name);				//String version of above
		void enableDerivedApSubnet();				//Enable use of AP subnet based off MAC address, instead of 192.168.4.0/24
		void configureNtp(bool autoconfigure = true);				//Enable NTP using the default server 'pool.ntp.org', autoconfiguring TZ
		void configureNtp(const char *, bool autoconfigure = true);//Set an NTP server and enable NTP
		void configureNtp(String, bool autoconfigure = true);		//Set an NTP server and enable NTP
		void configureTimeZone(const char *);						//Set a timezone for the NTP server, if not autoconfiguring
		void configureTimeZone(String);				//Set a timezone for the NTP server, if not autoconfiguring
		void setApChannel(uint8_t channel = 0);		//Set a channel for the softAp, 0 implies automatic (least congested)
	protected:
	private:
		Stream *_outputStream = nullptr;			//The stream used for the terminal
		uint16_t connectionRetries = 30;			//How many times a connection retries
		uint16_t connectionRetryFrequency = 1000;	//Interval between retries in ms
		void printConnectionStatus();				//Print client status
		void printIpStatus();						//Print IP status
		void printGeneralInfo();					//Detail about IDF version etc.
		void _enableNtp();							//Enable NTP internally
		bool _getTimeOffset();						//Geolocate to set UTC offset
		uint16_t _timeOffset = 0;					//UTC offset in seconds
		#ifdef ESP32
		void es32printResetReason(uint8_t);			//Reset reason on ESP32 which lacks the function available in ESP8266
		#endif
		char* hostname = nullptr;					//Store hostname for setting once WiFi is started
		bool generalInfoPrinted = false;
		bool derivedApSubnet = false;
		char* _ntpServer = nullptr;
		bool _ntpEnabled = false;
		bool _ntpAutoconfigure = true;
		char* _timezone = nullptr;
		uint8_t softApChannel = 0;
};
extern espBoilerplateClass espBoilerplate;
#endif
#include <espBoilerplate.h>
#include <credentials.h>  //Used to contain my own personal Wi-Fi credentials but not pushed to GitHub, comment this line out and use your own below
#ifndef WIFI_SSID
  #define WIFI_SSID "test"
  #define WIFI_PSK "testpresharedkey"
#endif

bool connectedOk = false; //Store the state of the connection
struct tm currentTime;  //Store the current time

void setup() {
  Serial.begin(115200);                                   //Set up the Serial device
  espBoilerplate.setOutputStream(Serial);                 //Optionally you can specifiy the Stream to send output to, eg. if using Serial1
  espBoilerplate.setRetries(60);                          //Optionally, increase retries on the connection to 60s. Default is 30s.
  espBoilerplate.configureNtp();                          //Optionally, enable NTP and set it to geolocate for the UTC offset
  connectedOk = espBoilerplate.begin(WIFI_SSID,WIFI_PSK); //Connect to the Wi-Fi SSID WIFI_SSID with pre-shared key WIFI_PSK, return is true if succesful
  if(connectedOk == false)
  {
    Serial.println("Restarting");
    ESP.restart();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(getLocalTime(&currentTime))
  {
    Serial.printf("Current time: %02u:%02u:%02u %02u/%02u/%02u\r\n", currentTime.tm_hour, currentTime.tm_min, currentTime.tm_sec, currentTime.tm_mday, currentTime.tm_mon +1, currentTime.tm_year + 1900);
  }
  delay(1000);
}

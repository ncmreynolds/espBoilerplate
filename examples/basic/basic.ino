#include <espBoilerplate.h>
#include "credentials.h"  //Used to contain my own personal Wi-Fi credentials but not pushed to GitHub, comment this line out and use your own below
#ifndef WIFI_SSID
  #define WIFI_SSID "test"
  #define WIFI_PSK "testpresharedkey"
#endif
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  espBoilerplate.begin(WIFI_SSID,WIFI_PSK);
}

void loop() {
  // put your main code here, to run repeatedly:

}

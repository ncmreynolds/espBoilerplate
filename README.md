# ESP Boilerplate
## Description

A low-functionality Arduino library for managing Wi-Fi connections on ESP8266/8285/32 boards, mostly it provides human-readable feedback over a Stream, usually "Serial". This library consists mostly of human readable boilerplate rather than trying to replicate the complexity of more complete Wi-Fi manager libraries.

![](exampleOutput.png)

## Why use this Arduino library?

This is for when you are sick of writing very repetitive Wi-Fi connection management code every time you start an Arduino project, but you don't want a fully featured Wi-Fi manager library. You just want it to connect to an SSID and give you diagnostic feedback about doing so.

Including the library pulls in the right versions of other libraries for the different ESP variants and just gets on with it for you. When having code you want to work on ESP8266, ESP32, ESP32S2 and so on that requires quite a lot of preprocessor cruft to include and use the right methods.

All of this is just a very verbose implementation of code no smarter than the usual example Wi-Fi code and various Wi-Fi diagnostic methods but that's kind of the point. You can just replace yawnsome stuff like...

```
#if defined(ESP32)
	#include <WiFi.h>
#else
	#include <ESP8266WiFi.h>
#endif
...
...
...
void setup()
{
...
...
...
  WiFi.begin(WIFI_SSID,WIFI_PSK);
  uint8_t retries = 30;
  while(WiFi.status() != WL_CONNECTED && retries > 0)
  {
    Serial.print('.');
    delay(1000);
    retries--;
  }
  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.print("IP address:");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("Connection failed");
  }
...
```

...with...

```
#include <espBoilerplate.h>
...
...
...
espBoilerplate.begin(WIFI_SSID,WIFI_PSK);
```

...and avoid all the dull boilerplate. It also fails a bit more helpfully/sooner than just checking for WiFi.status() == WL_CONNECTED if the SSID does not exist etc.

It's probably most useful when starting out creating a sketch and can be ditched once you don't want to see all the output and/or want to reduce the size of the sketch.

## Usage

There are only a handful of methods to this library, it's supposed to be simple.

```c++
bool espBoilerplate.begin(WIFI_SSID,WIFI_PSK);	//Does what it says on the tin, connects to this SSID with this PSK
```

```c++
bool espBoilerplate.begin(WIFI_SSID,WIFI_PSK,AP_SSID,AP_PSK);//Similar to the above but starts a SoftAp as well
```

```c++
bool espBoilerplate.setOutputStream(Serial1);	//Optionally, send the output to another stream, but it defaults to "Serial"
```

```c++
void setRetries(60);							//Set how many 1s retries before giving up on making a connection
```


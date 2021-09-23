# ESP Boilerplate
## Description

A low-functionality Arduino library for managing Wi-Fi connections on ESP8266/8285/32 boards, mostly it provides human-readable feedback over a Stream, usually "Serial". This library consists mostly of human readable boilerplate rather than trying to replicate the complexity of more complete Wi-Fi manager libraries.

## Why use this Arduino library?

This is for when you are sick of writing stuff like this...

```c++
while(WiFi.status() != WL_CONNECTED && retries > 0)
{
    Serial.print('.');
    delay(1000);
    retries--;
}
Serial.println();
if(WiFi.status() == WL_CONNECTED)
{
    Serial.print("IP address:");
    Serial.println(WiFi.localIP());
}
```

...every time you start and Arduino project, but you don't want a fully featured Wi-Fi manager library. You just want it to connect to an SSID and give you feedback about doing so.

Including the library pulls in the right versions of other libraries for the different ESP variants and just gets on with it for you. Because I often want real time clock available in my projects it also pulls in libraries for connecting to an NTP server, but this can be prevented if you like.

## Usage


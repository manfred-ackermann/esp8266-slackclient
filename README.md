# An Arduino library to wrap the Slack WebHook

This is a helper library to abstract away and favilitate Slack WebHook communication

```cpp
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "SlackClient.h"

WiFiClientSecure client;
SlackClient slackClient(client, "AAAAAAAAAA/BBBBBBBBBBB/CCCCCCCCCCCCCCCCCCCCCC");

void startWiFi() { /* setup wifi connection */ }

void setup()
{
    startWiFi();
    client.setInsecure();
    slackClient.setUsername("esp8266-slackclient");
    slackClient.sendMessage("Hello World!");
}
```

MIT license, all text above must be included in any redistribution
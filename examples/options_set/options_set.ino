#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "SlackClient.h"

// network credentials
const char *ssid = "WIFISSID";
const char *pass = "WIFIPASS";

WiFiClientSecure client;
SlackClient slackClient(client, "AAAAAAAAAA/BBBBBBBBBBB/CCCCCCCCCCCCCCCCCCCCCC");

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);
  while (!Serial);
  Serial.print("\n\n");

  // Connect to Wi-Fi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) { delay(250); }

  client.setInsecure();
  slackClient.setUsername("ESP8266");
  slackClient.setIconEmoji(":cold_face:");
  slackClient.sendMessage("Hello World!");
  Serial.println("Setup finished");
}

//=======================================================================
// Main loop
//=======================================================================
void loop();
/**
 * Copyright (c) 2023 Manfred Ackermann. All right reserved.
 * 
 * SlackClient - An Arduino library to wrap the Slack WebHook
 * 
 * MIT License
 **/

#include "SlackClient.h"

SlackClient::SlackClient(Client &client, const char* slackWebhook) {
  this->_slackWebhook = slackWebhook;
  this->_client = &client;
}

int SlackClient::sendMessage(const char* message) {
  // strlen(", \"icon_emoji\": \"\"") = 18
  size_t len_iconEmoji = strlen(this->_iconEmoji) == 0 ? 0 : strlen(this->_iconEmoji) + 18;
  // strlen(", \"username\": \"\") = 16
  size_t len_username  = strlen(this->_username ) == 0 ? 0 : strlen(this->_username ) + 16;
  // strlen(", \"channel\": \"\") = 15
  size_t len_channel   = strlen(this->_channel  ) == 0 ? 0 : strlen(this->_channel ) + 16;
  
  // strlen("{\"text\": \"\"}") = 12  
  char* payload = (char*) malloc((12+strlen(message)+len_iconEmoji+len_username+len_channel+1)*sizeof(char));

  strcpy(payload, "{\"text\": \"" );
  strcat(payload, message );
  if(len_iconEmoji) { // add if icon_emoji is set
    strcat(payload, "\", \"icon_emoji\": \"" );
    strcat(payload, this->_iconEmoji );
  }
  if(len_username) { // add if username is set
    strcat(payload, "\", \"username\": \"" );
    strcat(payload, this->_username );
  }
  if(len_channel) { // add if channel is set
    strcat(payload, "\", \"channel\": \"" );
    strcat(payload, this->_channel );
  }
  strcat(payload, "\"}" );

  _client->flush();
  _client->setTimeout(SLACK_TIMEOUT);
  if (!_client->connect(SLACK_HOST, 443)) {
      SLACK_SERIAL_LN(F("SlackClient Connection failed"));
      return -1;
  }
  // give the watchdog a hint
  yield();

  // Send HTTP request
  _client->print(F("POST /services/"));
  _client->print(this->_slackWebhook);
  _client->println(F(" HTTP/1.1"));

  //Headers
  _client->print(F("Host: "));
  _client->println(SLACK_HOST);

  SLACK_DEBUG_SERIAL(F("SlackClient Payload: '"));
  SLACK_DEBUG_SERIAL(payload);
  SLACK_DEBUG_SERIAL_LN(F("'"));

  _client->println(F("User-Agent: SlackClient/0.0.1"));
  _client->println(F("Accept: */*"));
  _client->println(F("Content-Type: application/json"));
  _client->print(F("Content-Length: "));
  _client->println(strlen(payload));
  _client->println();
  _client->print(payload);

  if (_client->println() == 0) {
    SLACK_SERIAL_LN(F("SlackClient Failed to send request"));
    return false;
  }

  return status();
}

void SlackClient::setUsername(const char* username) {
  this->_username = username;
}

void SlackClient::setIconEmoji(const char* iconEmoji) {
  this->_iconEmoji = iconEmoji;
}

void SlackClient::setChannel(const char* channel) {
  this->_channel = channel;
}

int SlackClient::status()
{
  // Check HTTP status
  if (_client->find("HTTP/1.1"))
  {
    int statusCode = _client->parseInt();
    SLACK_DEBUG_SERIAL(F("SlackClient Status : "));
    SLACK_DEBUG_SERIAL_LN(statusCode);
    return statusCode;
  }

  return -1;
}

void SlackClient::close()
{
  if (_client->connected())
  {

    SLACK_DEBUG_SERIAL_LN(F("SlackClient Closing client"));
    _client->stop();
  }
}
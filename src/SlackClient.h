/**
 * Copyright (c) 2023 Manfred Ackermann. All right reserved.
 * 
 * SlackClient - An Arduino library to wrap the Slack WebHook
 * 
 * MIT License
 **/

#ifndef SlackClient_h
#define SlackClient_h

#include "Arduino.h"
#include <Client.h>

#define SLACK_ENABLE_SERIAL

//un-mark following line to enable debug mode
//#define SLACK_ENABLE_DEBUG

#ifdef SLACK_ENABLE_SERIAL
#define SLACK_SERIAL(STR) Serial.print(STR)
#define SLACK_SERIAL_LN(STR) Serial.println(STR)
#else
#define SLACK_SERIAL(STR)
#define SLACK_SERIAL_LN(STR)
#endif

#ifdef SLACK_ENABLE_DEBUG
#define SLACK_DEBUG_SERIAL(STR) Serial.print(STR)
#define SLACK_DEBUG_SERIAL_LN(STR) Serial.println(STR)
#else
#define SLACK_DEBUG_SERIAL(STR)
#define SLACK_DEBUG_SERIAL_LN(STR)
#endif

#define SLACK_HOST "hooks.slack.com"
#define SLACK_TIMEOUT 2000

class SlackClient {
  private:
    const char* _slackWebhook;
    const char* _iconEmoji = "";
    const char* _username  = "";
    const char* _channel   = "";
    Client *_client;
    bool _enabled;
    
    int status();
    void skip();
    void close();

  public:
    /**
     * Constructor
     * 
     * @param client WiFiClientSecure class.
     * @param slackWebhook Slack WebHook URL. E.g. AAAA/BBBB/CCCC
     */
    SlackClient(Client &client, const char* slackWebhook);

    /**
     * Sends a message to the channel.
     * 
     * @param message The text to be send as message.
     * @return Returns HTTP response code (e.g. HTTP_CODE_OK) or -1 if connection failed.
     */
    int sendMessage(const char* message);

    /**
     * Override the legacy integration's default name.
     * 
     * @param username The username to appear as message sender.
     */
    void setUsername(const char* username);

    /**
     * Override the legacy integration's default icon_emoji.
     * 
     * @param iconEmoji The emoji code string to use in place of the default icon, such as :envelope:.
     */
    void setIconEmoji(const char* iconEmoji);

    /**
     * Override the legacy integration's default channel.
     * 
     * @param channel This should be an ID, such as C8UJ12P4P.
     */
    void setChannel(const char* channel);

    /**
     * Option to enable communication to Slack.
     */
    void enable();

    /**
     * Option to disable communication to Slack. All 
     * communication will be supressed.
     */
    void disable();

    /**
     * Option to enable communication to Slack.
     * 
     * @return true if enabled, false otherwise.
     */
    bool isEnabled();
};

#endif
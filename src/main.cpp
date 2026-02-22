#include <Arduino.h>

#include <WiFi.h>
#include <HTTPClient.h>

#include "config.h"

/**
  config.h
  
  #ifndef CONFIG_H
  #define CONFIG_H

  const char* ssid = "my_ssid";
  const char* password = "my_password";

  #endif
 */

const uint8_t Y_PIN = 27;
const uint8_t G_PIN = 25;

void preparePinMode();
void blinkTest();
void connectToWifi();
void checkWifiConnection();

void setup() {
  Serial.begin(115200);

  preparePinMode();
  blinkTest();

  connectToWifi();
}

void preparePinMode() {
  pinMode(Y_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
}

void blinkTest() {
  for(int i = 0; i < 2; i++) {
    digitalWrite(Y_PIN, HIGH);
    digitalWrite(G_PIN, HIGH);

    delay(100);

    digitalWrite(Y_PIN, LOW);
    digitalWrite(G_PIN, LOW);

    delay(100);
  }
}

void connectToWifi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  digitalWrite(G_PIN, LOW);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");

    digitalWrite(Y_PIN, HIGH);
    delay(500);

    digitalWrite(Y_PIN, LOW);
    delay(500);
  }

  digitalWrite(G_PIN, HIGH);

  Serial.println("\nConnected!");

  Serial.print("Local IP: ");
  Serial.print(WiFi.localIP());
}

void loop() {
  checkWifiConnection();

  // TODO: health check
  // TODO: alert if health check failed
}

void checkWifiConnection() {
  if(WiFi.status() != WL_CONNECTED) {
    connectToWifi();
  }
}
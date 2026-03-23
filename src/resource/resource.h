#ifndef RESOURCE_H
#define RESOURCE_H

#include <Arduino.h>
#include <WiFiClientSecure.h>

void initResourceClient(WiFiClientSecure& client);
void checkResourceAvailability();
void writeResourceConf(const char* url, int expectedCode, int checkInterval);
int getResourceCheckInterval();
const char* getResourceUrl();
int getResourceExpectedCode();

#endif

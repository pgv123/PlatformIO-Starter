//Header file for my gui functionality based on ESPUI

#include <Arduino.h>
#include <EEPROM.h>
#include <ESPUI.h>


#if defined(ESP32)
#include <WiFi.h>
#include <ESPmDNS.h>
#else
// esp8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <umm_malloc/umm_heap_select.h>
#ifndef CORE_MOCK
#ifndef MMU_IRAM_HEAP
#warning Try MMU option '2nd heap shared' in 'tools' IDE menu (cf. https://arduino-esp8266.readthedocs.io/en/latest/mmu.html#option-summary)
#warning use decorators: { HeapSelectIram doAllocationsInIRAM; ESPUI.addControl(...) ... } (cf. https://arduino-esp8266.readthedocs.io/en/latest/mmu.html#how-to-select-heap)
#warning then check http://<ip>/heap
#endif // MMU_IRAM_HEAP
#ifndef DEBUG_ESP_OOM
#error on ESP8266 and ESPUI, you must define OOM debug option when developping
#endif
#endif
#endif

//Settings
#define SLOW_BOOT 0
#define HOSTNAME "HatDay"
#define FORCE_USE_HOTSPOT 1

//Function Prototypes
void connectWifi();
void setUpUI();
void enterWifiDetailsCallback(Control *sender, int type);
void textCallback(Control *sender, int type);
void generalCallback(Control *sender, int type);
void scrambleCallback(Control *sender, int type);
void styleCallback(Control *sender, int type);
void updateCallback(Control *sender, int type);
void getTimeCallback(Control *sender, int type);
void graphAddCallback(Control *sender, int type);
void graphClearCallback(Control *sender, int type);
void randomString(char *buf, int len);
void extendedCallback(Control* sender, int type, void* param);
bool chk_buttonPressed();
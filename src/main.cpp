#include <Arduino.h>

#define USE_EXAMPLE

#ifdef USE_EXAMPLE
#include "main_example.hpp"
#else

#include "driver/display/Display_ST7796.h"

static constexpr const char *TAG = "app";

Display_ST7796 display;

void rainBow()
{
  display.fillScreenBlack();
  delay(500);
  display.fillScreenRed();
  delay(500);
  display.fillScreenGreen();
  delay(500);
  display.fillScreenBlue();
  delay(500);
  ESP_LOGI(TAG, "done rainbow");
}

void setup()
{
  Serial.begin(115200);
  display.begin();
  // rainBow();
}

void loop()
{
  // lv_timer_handler();
  // delay(5);
}

#endif
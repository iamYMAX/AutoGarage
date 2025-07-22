// Module for handling the Real-Time Clock (RTC).

#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setupRtc() {
  // if (!rtc.begin()) {
  //   Serial.println("Couldn't find RTC");
  //   while (1);
  // }

  // if (rtc.lostPower()) {
  //   Serial.println("RTC lost power, let's set the time!");
  //   // When the RTC loses power, it sets the time to a default value.
  //   // This line sets the RTC to the date & time this sketch was compiled
  //   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // }
}

void loopRtc() {
  // The RTC keeps time on its own, so the loop is only needed if we want to
  // periodically fetch the time for display or other purposes.
}

DateTime getCurrentTime() {
  return rtc.now();
}

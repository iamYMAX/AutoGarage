// Module for logging data to an SD card.

#include <SPI.h>
#include <SD.h>

#define SD_CS_PIN 10 // Note: This conflicts with the encoder. This will need to be resolved.
File logFile;

void setupLogger() {
  // For now, we assume the SD card is on a different CS pin.
  // The pin conflict with the encoder needs to be addressed in the final design.
  // For example, by moving the SD CS to another pin.

  // Serial.print("Initializing SD card...");
  // if (!SD.begin(SD_CS_PIN)) {
  //   Serial.println("initialization failed!");
  //   return;
  // }
  // Serial.println("initialization done.");

  // logFile = SD.open("log.csv", FILE_WRITE);
  // if (logFile) {
  //   logFile.println("time,rpm,generator_duty");
  //   logFile.close();
  // } else {
  //   Serial.println("error opening log.csv");
  // }
}

void loopLogger() {
  // Log data periodically
  static unsigned long lastLogTime = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastLogTime > 1000) { // Log every second
    lastLogTime = currentTime;
    // logData();
  }
}

void logData() {
  // logFile = SD.open("log.csv", FILE_WRITE);
  // if (logFile) {
  //   logFile.print(millis());
  //   logFile.print(",");
  //   logFile.print(rpm);
  //   logFile.print(",");
  //   logFile.println(pwmDutyCycle); // Assumes pwmDutyCycle is global
  //   logFile.close();
  // }
}

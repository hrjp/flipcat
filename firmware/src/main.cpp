#include <Arduino.h>
#include <TinyGPS++.h>
#include <SdFat.h>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "CSVread.h"

static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;

CSVread csv("202112.csv");

void setup() {
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  Serial.begin(115200);
  Serial1.begin(GPSBaud);

  csv.print();
  //Serial.print(csv.data[0][1].c_str());
}

void loop() {

  while (Serial1.available()){
      gps.encode(Serial1.read());
  }
  char sz[128];
  sprintf(sz, "%02d:%02d:%02d:%02d:%02d:%02d ", gps.date.year(),gps.date.month(), gps.date.day(),gps.time.hour(), gps.time.minute(), gps.time.second());
  //Serial.println(sz);
  // put your main code here, to run repeatedly:
}
#include <Arduino.h>
#include <TinyGPS++.h>
#include <SdFat.h>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iostream>

#include "CSVread.h"

static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;

CSVread csv("202112.csv");

//unix時間に変換
time_t get_time_t(const int year,const int month,const int day,
                  const int hour,const int min,const int sec=0,const int time_zone=0){
  struct tm get_time={sec,min,hour,day,month,year};
  return std::mktime(&get_time)+3600*time_zone;
}

std::string to_string(auto tostr){
  std::stringstream out;
  out << tostr;
  return out.str();
}

void setup() {
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  Serial.begin(115200);
  Serial1.begin(GPSBaud);

  csv.print();
  //Serial.print(csv.data[0][1].c_str());
}

void loop() {

  //get gps time
  while (Serial1.available()){
      gps.encode(Serial1.read());
  }
  time_t gps_time=get_time_t(gps.date.year(),gps.date.month(),gps.date.day(),gps.time.hour(),gps.time.minute(),gps.time.second(),9);
  struct tm *gmtime(const time_t *gps_time);
  tm gps_t;

  //日付が変化したら新しい営業時間を取得
  static int pre_day=0;
  if(pre_day!=gps_t.tm_mday){
    std::string file_name=to_string(gps_t.tm_year)+to_string(gps_t.tm_mon)+".csv";
    csv.getCSV(file_name);
    pre_day=gps_t.tm_mday;
  }

  //get time
  time_t cafe_open_time=get_time_t(gps_t.tm_year,gps_t.tm_mon,gps_t.tm_mday,atoi(csv.data[gps_t.tm_mday][0].c_str()),atoi(csv.data[gps_t.tm_mday][1].c_str()));
  time_t cafe_close_time=get_time_t(gps_t.tm_year,gps_t.tm_mon,gps_t.tm_mday,atoi(csv.data[gps_t.tm_mday][2].c_str()),atoi(csv.data[gps_t.tm_mday][3].c_str()));
  time_t shop_open_time=get_time_t(gps_t.tm_year,gps_t.tm_mon,gps_t.tm_mday,atoi(csv.data[gps_t.tm_mday][4].c_str()),atoi(csv.data[gps_t.tm_mday][5].c_str()));
  time_t shop_close_time=get_time_t(gps_t.tm_year,gps_t.tm_mon,gps_t.tm_mday,atoi(csv.data[gps_t.tm_mday][6].c_str()),atoi(csv.data[gps_t.tm_mday][7].c_str()));
  
  if(cafe_open_time<gps_time && gps_time<cafe_close_time){
    
  }
  else{

  }

  if(shop_open_time<gps_time && gps_time<shop_close_time){
    
  }
  else{

  }

  char sz[128];
  sprintf(sz, "%02d:%02d:%02d:%02d:%02d:%02d ", gps.date.year(),gps.date.month(), gps.date.day(),gps.time.hour(), gps.time.minute(), gps.time.second());
  //Serial.println(sz);
  // put your main code here, to run repeatedly:
}
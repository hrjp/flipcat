#include <Arduino.h>
#include <TinyGPS++.h>
#include <SdFat.h>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iostream>
#include <iomanip>
#include "CSVread.h"
#include <Servo.h>

static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;

CSVread csv;

Servo shop_motor,cafe_motor;
const int up_angle=30;
const int down_angle=150;

//unix時間に変換
time_t get_time_t(int year,int month,int day,
                  int hour,int min,int sec=0,int time_zone=0){
  //struct tm get_time={sec,min,hour,day,month,year};
  struct std::tm get_time;
  get_time.tm_year=year;
  get_time.tm_mon=month;
  get_time.tm_mday=day;
  get_time.tm_hour=hour;
  get_time.tm_min=min;
  get_time.tm_sec=sec;

  return std::mktime(&get_time)+168687344+3600*time_zone;
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
  shop_motor.attach(26);
  cafe_motor.attach(28);
}

void loop() {

  //get gps time
  while (Serial1.available()){
      gps.encode(Serial1.read());
  }
  //Serial.println("AAAAA");
  time_t gps_time=get_time_t(gps.date.year(),gps.date.month(),gps.date.day(),gps.time.hour(),gps.time.minute(),gps.time.second(),18);
  //time_t gps_time=get_time_t(2021,12,24,7,35,0,9);
  struct std::tm *gps_t;
  gps_t=gmtime(&gps_time);
  //Serial.println("BBBB");
  //日付が変化したら新しい営業時間を取得
  static int pre_day=0;
  if(pre_day!=gps_t->tm_mday){
    std::string file_name=to_string(gps_t->tm_year)+to_string(gps_t->tm_mon)+".csv";
    csv.getCSV(file_name);
    pre_day=gps_t->tm_mday;
    Serial.println("csv file loading...");
    csv.print();
  }

  //get time
  time_t cafe_open_time=get_time_t(gps_t->tm_year+1900,gps_t->tm_mon+1,gps_t->tm_mday,atoi(csv.data[gps_t->tm_mday-1][4].c_str()),atoi(csv.data[gps_t->tm_mday-1][5].c_str()),0,9);
  time_t cafe_close_time=get_time_t(gps_t->tm_year+1900,gps_t->tm_mon+1,gps_t->tm_mday,atoi(csv.data[gps_t->tm_mday-1][6].c_str()),atoi(csv.data[gps_t->tm_mday-1][7].c_str()),0,9);
  //time_t cafe_close_time=get_time_t(gps_t->tm_year+1900,gps_t->tm_mon+1,gps_t->tm_mday,17,38,0,9);
  time_t shop_open_time=get_time_t(gps_t->tm_year+1900,gps_t->tm_mon+1,gps_t->tm_mday,atoi(csv.data[gps_t->tm_mday-1][0].c_str()),atoi(csv.data[gps_t->tm_mday-1][1].c_str()),0,9);
  time_t shop_close_time=get_time_t(gps_t->tm_year+1900,gps_t->tm_mon+1,gps_t->tm_mday,atoi(csv.data[gps_t->tm_mday-1][2].c_str()),atoi(csv.data[gps_t->tm_mday-1][3].c_str()),0,9);
  //Serial.printf("%d:%d", atoi(csv.data[gps_t->tm_mday-1][2].c_str()),atoi(csv.data[gps_t->tm_mday-1][3].c_str()));
  
  if(cafe_open_time<gps_time && gps_time<cafe_close_time){
    Serial.print("Cafe:OPEN ");
    cafe_motor.write(up_angle);
  }
  else{
    Serial.print("Cafe:CLOSE ");
    cafe_motor.write(down_angle);
  }

  if(shop_open_time<gps_time && gps_time<shop_close_time){
    Serial.print("Shop:OPEN ");
    shop_motor.write(up_angle);
  }
  else{
    Serial.print("Shop:CLOSE ");
    shop_motor.write(down_angle);
  }

  //char sz[128];
  //sprintf(sz, "%02d:%02d:%02d:%02d:%02d:%02d ", gps.date.year(),gps.date.month(), gps.date.day(),gps.time.hour(), gps.time.minute(), gps.time.second());
  //Serial.print(sz);

  char sz2[128];
  sprintf(sz2, "UNIX TIME:%d %02d:%02d:%02d:%02d:%02d:%02d ", gps_time,gps_t->tm_year+1900,gps_t->tm_mon+1, gps_t->tm_mday,gps_t->tm_hour, gps_t->tm_min, gps_t->tm_sec);
  Serial.print(sz2);

  //char sz3[128];
  //sprintf(sz3, "gps:%d cafe:%d-%d shop:%d-%d",gps_time,cafe_open_time,cafe_close_time,shop_open_time,shop_close_time);
  //Serial.print(sz3);

  Serial.println();
}
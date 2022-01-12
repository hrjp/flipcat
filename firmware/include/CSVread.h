#pragma once
#include <SdFat.h>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>


class CSVread{
public:
    CSVread();
    CSVread(const std::string& file_path);
    bool getCSV(const std::string& file_path);
    void print();
    std::vector<std::vector<std::string> > data;
private:
    SdFs sd;
    FsFile file;
    
};

CSVread::CSVread():sd(),file(){

}

CSVread::CSVread(const std::string& file_path):sd(),file(){
    getCSV(file_path);
}

bool CSVread::getCSV(const std::string& file_path){
    if (!sd.begin(SdioConfig(FIFO_SDIO))) {
        sd.initErrorHalt(&Serial);
        return false;
    }
    if (!file.open(file_path.c_str(), FILE_WRITE)) {
        Serial.println("file open failed");
        return false;
    }
    else{
        Serial.println("file open successful");
    }
    data.resize(1);
    data[0].resize(0);
    char line[1000];
    int i = 0;
    int j = 0;
    file.rewind();
    while (file.available()) {
        file.fgets(line, sizeof(line));
        std::string tmp = "";
        std::string str(line);
        std::istringstream stream(str);
        // 区切り文字がなくなるまで文字を区切っていく
        while (getline(stream, tmp, ',')){
            data.at(i).push_back(tmp);
            j++;
        }
        j = 0;
        i++;  
        data.resize(i+1);
        //Serial.print(line);
    }
    file.close();
    return true;
}

void CSVread::print(){
    bool sepalator_skip=true;
    for(auto k:data){
        for(auto l:k){
            if(!sepalator_skip){
                Serial.print(",");
            }
            sepalator_skip=false;
            Serial.print(l.c_str());
        }
        sepalator_skip=true;
    }
}

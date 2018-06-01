#ifndef LEDLATTICE_H
#define LEDLATTICE_H
#include<sys/stat.h>
#include<unistd.h>
#include<stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdint.h>
#include<iostream>

class ledLattice
{
public:
    ledLattice();
    void sendCmd(int fd,uint8_t* cmd, uint8_t len);
    void showHanzi(int fd, uint8_t* hz);
    int show();

    void LightLed();
    void LightWithLetter();
};

#endif // LEDLATTICE_H

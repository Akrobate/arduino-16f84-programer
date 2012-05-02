/*
  Morse.h - Library for flashing 16F84.
  Created by Artiom FEDOROV Avri 2012
  Released into the public domain.
*/
#ifndef Pic16F84_h
#define Pic16F84_h

#include "Arduino.h"

class Pic16F84
{
  public:
    Pic16F84(int pinVmax, int pinReset, int pinClock, int pinData);

    void eraseChip();
    void loadConfiguration(String confWord);
    void loadProgram(String program);

    int writeWordandCheck(String data);
    int writeHexWord(String hex);
    String writeHexWord2(String hex);
    void writeConfigurationWord(String cWord);


    void commandLoadConfiguration(String wordtoload);
    void commandIncrementAdress();
    void commandEraseAndProgram();
    String commandReadDataProgram();
    void commandLoadDataProgram(String wordtoload);
    void commandBulkEraseProgram();


    void sendData(String commande);
    
    void startProgMode();
    void stopProgMode();
    void resetPic();
    
    int sendTickAndRead();
    void sendOne();
    void sendZero();

    String revertStr(String str);
    String convertHexCharToBin(char carr);
    
    String reverseHexa(String str);
    
    
  private:
    int _time;
    int _pinVmax;
    int _pinReset;
    int _pinClock;
    int _pinData;
    
    
    void _delay(int time);    
};

#endif

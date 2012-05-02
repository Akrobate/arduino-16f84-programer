#include "Arduino.h"
#include "Pic16F84.h"




/* Methodes de haut niveau */

void Pic16F84::eraseChip() {

   startProgMode();
   delay(200);
   
  commandLoadDataProgram("11111111111111");
  commandBulkEraseProgram();
  commandEraseAndProgram();
  delay(11);
 
 
  stopProgMode(); 
}

/* Prends en parametre un mot hexadecimal correspondant a la configuration: ex: 3FFA */
void Pic16F84::loadConfiguration(String confWord) {
  
     startProgMode();
     delay(200);    

      writeConfigurationWord(confWord);
      
      stopProgMode();
      delay(20);
}


void Pic16F84::loadProgram(String program) {

   startProgMode();
   delay(200);
 
   String hword = "";
  
  for (int i = 0; i < program.length(); i = i+4) {
      
      hword = "";
          for (int j = 0; j < 4; j++) {
            hword += program[i+j]; 
          }
      
       hword = reverseHexa(hword);
       
      writeHexWord(hword);
      commandIncrementAdress();
  }
  
    stopProgMode();
    delay(20);
}



/* Constructeur */

Pic16F84::Pic16F84(int pinVmax, int pinReset, int pinClock, int pinData) {
  _pinVmax = pinVmax;
  _pinReset = pinReset;
  _pinClock = pinClock;
  _pinData = pinData;
  
//  _time = 80000;

  _time = 1;
  
  pinMode(_pinVmax, OUTPUT);
  pinMode(_pinReset, OUTPUT);  
  pinMode(_pinClock, OUTPUT);
  pinMode(_pinData, OUTPUT);
  
  digitalWrite(_pinVmax, LOW); 
  digitalWrite(_pinReset, LOW); 
  digitalWrite(_pinClock, LOW); 
  digitalWrite(_pinData, LOW); 
}

int Pic16F84::writeHexWord(String hex) {
  
  String str = "";
  
  for  (int i=0; i < hex.length(); i++) {
    str += convertHexCharToBin( hex[i] );
  }
  
  str = str.substring(2);
  str = revertStr(str);
  return writeWordandCheck(str);
  
}

// DEBUG FUNCTION

String Pic16F84::writeHexWord2(String hex) {
 
  String str = "";
  
  for  (int i=0; i < hex.length(); i++) {
    str = str + convertHexCharToBin( hex[i] );
  }
  
  str = str.substring(2);
  
  // str = revertStr(str);
  
  return str;
 
}


int Pic16F84::writeWordandCheck(String data) {
  
  commandLoadDataProgram(data);   
  delay(1);
  commandEraseAndProgram();
  delay(8);

  String dataR = commandReadDataProgram();
  if (dataR == data) {
   return 1; 
  } else {
   return 0; 
  }
  
  
}



void Pic16F84::writeConfigurationWord(String cWord) {
  String str = "";
  for  (int i=0; i < cWord.length(); i++) {
    str += convertHexCharToBin( cWord[i] );
  }
  
  str = str.substring(2);
  str = revertStr(str);
  commandLoadConfiguration(str);
    
  commandIncrementAdress();
  commandIncrementAdress();    
  commandIncrementAdress();    
  commandIncrementAdress();

  commandIncrementAdress();
  commandIncrementAdress();
  commandIncrementAdress();

  commandEraseAndProgram();
  
}



void Pic16F84::commandLoadConfiguration(String wordtoload) {
   sendData("000000");
   _delay(_time*2);
   
  _delay(_time*4);
  sendZero();  // Bit de start
  sendData(wordtoload);
  sendZero();  // Bit de stop  

  _delay(_time*4);

   
   
}

void Pic16F84::commandBulkEraseProgram() {
   sendData("100100");
   _delay(_time*2);
}
  



void Pic16F84::commandIncrementAdress() {
  sendData("011000");
 _delay(_time*2);
}

void Pic16F84::commandEraseAndProgram() {
  sendData("000100"); 
  _delay(_time*2);  
  
}


// prends en parametres le mot de 14bits a envoyer en binaire sous forme String "101010101.." 
void Pic16F84::commandLoadDataProgram(String wordtoload) {
  sendData("010000"); //  load data command privram verify

  _delay(_time*4);
  sendZero();  // Bit de start
  sendData(wordtoload);
  sendZero();  // Bit de stop  

  _delay(_time*4);
  
  // Attente du cycle 8ms pour le 16F84A et 20ms tous les autres

  
}

String Pic16F84::commandReadDataProgram() {
  
    String result = "";

    sendData(String("001000")); //  read data command program
   pinMode(_pinData, INPUT);
   
   _delay(_time*2);
   
   sendTickAndRead();
   
   for (int i = 0; i < 14; i++) {
      result += sendTickAndRead();       
   }   
   
   sendTickAndRead();
   _delay(_time*2);
   pinMode(_pinData, OUTPUT);
   
   return result;
   
}


void Pic16F84::sendData(String commande) {
  for (int i = 0; i < commande.length(); i++) {
     if (commande[i] == '1') {
       sendOne(); 
     } else {
       sendZero();
     }
  }
}


void Pic16F84::startProgMode() {
  digitalWrite(_pinClock, LOW); 
  digitalWrite(_pinData, LOW); 
  digitalWrite(_pinReset, LOW); 
  delayMicroseconds(1); 
  digitalWrite(_pinVmax, HIGH);
  delayMicroseconds(1); 
  
}


void Pic16F84::stopProgMode() {
  digitalWrite(_pinVmax, LOW);
  digitalWrite(_pinReset, LOW); 
  digitalWrite(_pinClock, LOW); 
  digitalWrite(_pinClock, LOW); 

  _delay(4 * _time);
}


void Pic16F84::resetPic() {
  digitalWrite(_pinVmax, LOW);
  digitalWrite(_pinReset, HIGH); 
  digitalWrite(_pinClock, LOW); 
  digitalWrite(_pinClock, LOW); 
  delay(5);
  digitalWrite(_pinReset, LOW); 
  delay(4);
}


void Pic16F84::sendOne() {
  digitalWrite(_pinClock, HIGH);
  _delay(_time);
  digitalWrite(_pinData, HIGH);
  _delay(_time);
  digitalWrite(_pinClock, LOW);
  _delay(_time);
  //digitalWrite(_pinData, LOW);
  _delay(_time);
}

void Pic16F84::sendZero() {
  digitalWrite(_pinClock, HIGH);
  _delay(_time);
  digitalWrite(_pinData, LOW);
  _delay(_time);
  digitalWrite(_pinClock, LOW);
  _delay(_time);
  digitalWrite(_pinData, LOW);
  _delay(_time); 
}

int Pic16F84::sendTickAndRead() {
  int result;
  digitalWrite(_pinClock, HIGH);
  _delay(_time);

  result = digitalRead(_pinData);

  _delay(_time);

  digitalWrite(_pinClock, LOW);
  _delay(_time*2);
  return result;
}


void Pic16F84::_delay(int time) {
 delayMicroseconds(time); 
  
}


// STRINGS Tools //

String Pic16F84::revertStr(String str) {
 String result;
 result = "";
 
 for (int i = str.length(); i != 0; i--) {
  result += str[i-1];
 }
 
  return result;
  
}








String Pic16F84::convertHexCharToBin(char carr) {
  String bin = "";

  switch (carr) {
   case '0':
     bin = "0000";
   break;
   case '1':
     bin = "0001";
   break;
   case '2':
     bin = "0010";
   break;
   case '3':
     bin = "0011";
   break;
   case '4':
     bin = "0100";
   break;
   case '5':
     bin = "0101";
   break;
   case '6':
     bin = "0110";
   break;  
   case '7':
     bin = "0111";
   break;
   case '8':
     bin = "1000";
   break;
   case '9':
     bin = "1001";
   break;
   case 'A':
     bin = "1010";
   break;
   case 'B':
     bin = "1011";
   break; 
   case 'C':
     bin = "1100";
   break;
   case 'D':
     bin = "1101";
   break;
   case 'E':
     bin = "1110";
   break;
   case 'F':
     bin = "1111";
   break;
  
   
  } 

   return bin;

  
}


String Pic16F84::reverseHexa(String str) {
  String result="";
  
  result += str.substring(2);
  result += str.substring(0,2);
  
  return result;
  
  
  
}



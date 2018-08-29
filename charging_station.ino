#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROMex.h>
#include "Arduino.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);

const byte ROWS = 4;
const byte COLS = 4;
const int memBase = 350;

int addressCharArray1;
int addressCharArray2;
int addressInt1;
int addressInt2;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {22, 24, 26, 28};
byte colPins[COLS] = {30, 32, 34, 36};

char password[7];
char adminPass[7] = {'2', '2', '2', '2'};
char stringBuffer[7];

int page = 0;
int passCounter = 0;

char input1[7];
char passboxA[7];

boolean userA = false;
boolean userB = false;

int reEnterA = false;
boolean reEnterB = false;

boolean state1 = false;
boolean state2 = false;

boolean state3 = false;
boolean state4 = false;

char input2[7];
char passboxB[7];

int d = 0;

#define boxSwitchA 7
#define boxSwitchB 8
#define solenoidA 9
#define solenoidB 10
#define relayPinA 11
#define relayPinB 12
#define piezoPin 13

int buttonstate1 = LOW;
int buttonstate2 = LOW;

unsigned long prevMillis1 = 0;
unsigned long prevMillis2 = 0;

const long interval = 1000; // every second

int mintimer1 = 29;
int mintimer2 = 29;

int sectimer1 = 60;
int sectimer2 = 60;

int minCounter1;
int minCounter2;

int secCounter1;
int secCounter2;

boolean timer1 = false;
boolean timer2 = false;

boolean initial1 = false;
boolean initial2 = false;

boolean trigger1 = false;
boolean trigger2 = false;

boolean endA = false;
boolean endB = false;
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup()
{
  lcd.init();
  lcd.init();
  Serial.begin(9600);

  for (int q = 9; q < 14; q++) { //set arduino pin 9 - 13 as output
    pinMode(q, OUTPUT);
  }
  pinMode(boxSwitchA, INPUT);
  pinMode(boxSwitchB, INPUT);

  minCounter1 = mintimer1;
  secCounter1 = sectimer1;

  minCounter2 = mintimer2;
  secCounter2 = sectimer2;

  addressInt1        = EEPROM.getAddress(sizeof(int));
  addressInt2        = EEPROM.getAddress(sizeof(int));
  addressCharArray1  = EEPROM.getAddress(sizeof(char) * 7);
  addressCharArray2  = EEPROM.getAddress(sizeof(char) * 7);

  EEPROM.setMemPool(memBase, EEPROMSizeMega); //Initialize the max EEPROM buffer

  EEPROM.readBlock<char>(addressCharArray1, passboxA, 7); //Read CharArray from EEPROM
  if (passboxA[0] > 0 && passboxA[1] > 0 && passboxA[2] > 0 && passboxA[3] > 0) {
    for (int u = 0; u < 7; u++) {
      Serial.print(passboxA[u]);
    }
    Serial.println(" ");
    userA = true;
    timer1 = true;
    mintimer1 = EEPROM.readInt(addressInt1);
    digitalWrite(relayPinA, HIGH);
    
  }

  EEPROM.readBlock<char>(addressCharArray2, passboxB, 7);
  if (passboxB[0] > 0 && passboxB[1] > 0 && passboxB[2] > 0 && passboxB[3] > 0) {
    for (int u = 0; u < 7; u++) {
      Serial.print(passboxB[u]);
    }
    Serial.println(" ");
    userB = true;
    timer2 = true;
    mintimer2 = EEPROM.readInt(addressInt2);
    digitalWrite(relayPinB, HIGH);
  }

  lcd.backlight();
  menu();
}


void loop() {
  TimerA();
  TimerB();
  triggers();
  //Serial.println(reEnterA);
  char customKey = customKeypad.getKey();
  //  delay(100);
  if (page == 0) {
    //BOX A
    if (timer1 ==  false && userA == false) {
      lcd.setCursor(11, 1);
      lcd.print("[Empty]  ");
    } else if (timer1 == false && userA == true) {
      lcd.setCursor(11, 1);
      lcd.print("[Done]     ");
    } else {
      lcd.setCursor(11, 1);
      lcd.print('[');
      lcd.print(minCounter1);
      if (minCounter1 > 10) {
        lcd.setCursor(14, 1);
      } else {
        lcd.setCursor(13, 1);
      }
      lcd.print(" mins]");
    }
    //BOX B
    if (timer2 ==  false && userB == false) {
      lcd.setCursor(11, 2);
      lcd.print("[Empty]  ");
    } else if (timer2 == false && userB == true) {
      lcd.setCursor(11, 2);
      lcd.print("[Done]     ");
    } else {
      lcd.setCursor(11, 2);
      lcd.print('[');
      lcd.print(minCounter2);
      if (minCounter2 > 10) {
        lcd.setCursor(14, 2);
      } else {
        lcd.setCursor(13, 2);
      }
      lcd.print(" mins]");
    }

    if (customKey == 'A') {
      refresh();
      page = 1;
      boxAlogin();
    } else if (customKey == 'B') {
      refresh();
      boxBlogin();
      page = 2;
    } else if (customKey == 'C') {
      refresh();
      page = 3;
      admin();
      //Serial.print("D: ");
     // Serial.println(d);
    } else if (customKey == 'D') {
      menu();
      page = 0;
    }
  } else if (page == 1) {//BOX1 LOGIN
    if (customKey == '#' && reEnterA == false && userA == false && d > 3) {//re-enter pword
      reEnterPW();
      d = 0;
      lcd.setCursor(0, 1);
      lcd.print("                    ");
      reEnterA = true;
    } else if (customKey == '#' && reEnterA == false && userA == true && d > 3) {
      validateLoginA();
    } else if (customKey == '#' && reEnterA == true && d > 2 ) {
      validateBoxA();
    } else if (customKey == '*') { //backspace
      del();
    } else if (customKey == 'D') {
      refresh();
      menu();
      page = 0;
    }  else if (customKey == '1' || customKey == '2' || customKey == '3' || customKey == '4' || customKey == '5' || customKey == '6' ||  customKey == '7' || customKey == '8' || customKey == '9' || customKey == '0') {
      if (d > 2) {
        lcd.setCursor(0, 2);
        lcd.print("# - Enter");
      }

      if (reEnterA == true && d != 7) {
        input1[d] = customKey;
        pword();
      } else if (reEnterA == false && d != 7) {
        stringBuffer[d] = customKey;
        pword();
      }

    }
  }  else if (page == 2) {//BOX2 LOGIN
    if (customKey == '#' && reEnterB == false && userB == false && d > 3) {//re-enter pword
      reEnterPW();
      d = 0;
      lcd.setCursor(0, 1);
      lcd.print("                    ");
      reEnterB = true;
    } else if (customKey == '#' && reEnterB == false && userB == true && d > 3) {
      validateLoginB();
    } else if (customKey == '#' && reEnterB == true && d > 2 ) {
      validateBoxB();
    } else if (customKey == '*') { //backspace
      del();
    } else if (customKey == 'D') {
      refresh();
      menu();
      page = 0;
    }  else if (customKey == '1' || customKey == '2' || customKey == '3' || customKey == '4' || customKey == '5' || customKey == '6' ||  customKey == '7' || customKey == '8' || customKey == '9' || customKey == '0') {
      if (d > 2) {
        lcd.setCursor(0, 2);
        lcd.print("# - Enter");
      }

      if (reEnterB == true && d != 7) {
        input2[d] = customKey;
        pword();
      } else if (reEnterB == false && d != 7) {
        stringBuffer[d] = customKey;
        pword();
      }

    }
  } else if (page == 3) {//ADMIN LOGIN
    if (customKey == '#') {
      validateAdmin();
    } else if (customKey == '*') {
      del();
    } else if (customKey == 'D') {
      refresh();
      menu();
      page = 0;
    } else if ((d != 7) && (customKey == '1' || customKey == '2' || customKey == '3' || customKey == '4' || customKey == '5' || customKey == '6' ||  customKey == '7' || customKey == '8' || customKey == '9' || customKey == '0')) {
      stringBuffer[d] = customKey;
      pword();
    }
  } else if (page == 4) { //ADMIN LOCK
    if (customKey == '#') {//enter
      validateAdmin();
    } else if (customKey == '*') {
      del();
    } else if ((d != 7) && (customKey == '1' || customKey == '2' || customKey == '3' || customKey == '4' || customKey == '5' || customKey == '6' ||  customKey == '7' || customKey == '8' || customKey == '9' || customKey == '0')) {
      stringBuffer[d] = customKey;
      pword();
    }
  } else if (page == 5) { //ADMIN PAGE
    if (customKey == 'A') {
      endServiceA();
      //Remove EEPROM A
    } else if (customKey == 'B') {
      endServiceB();
      //Remove EEPROM B
    } else if (customKey == '1') {
      state3 = !state3;
      digitalWrite(solenoidA, state3);
    } else if (customKey == '2') {
      state4 = !state4;
      digitalWrite(solenoidB, state4);
    } else if (customKey == 'D') {
      refresh();
      menu();
      page = 0;
    }
  } else if (page == 6) { //BOX A PAGE
    //=====LIVE TIMER=====
    if (timer1 == false && userA == true) {
      //Serial.println(initial1);
      if (initial1 == false) {
        lcd.setCursor(0, 1);
        lcd.print("     Time is up!    ");
      } else {
        lcd.setCursor(0, 1);
        lcd.print("  device is ready   ");
      }
      //  Serial.print(initial1);
    } else if (timer1 == true && userA == true) {
      lcd.setCursor(12, 1);
      lcd.print(minCounter1);
      if (minCounter1 > 10) {
        lcd.setCursor(14, 1);
        lcd.print(':');
        lcd.setCursor(15, 1);
      } else {
        lcd.setCursor(13, 1);
        lcd.print(':');
        lcd.setCursor(14, 1);
      }
      lcd.print(secCounter1);
      lcd.print(' ');
    }
    //====================
    if (customKey == 'D') {
      refresh();
      menu();
      page = 0;
    } else if (customKey == '#') {
      state1 = !state1;
      digitalWrite(solenoidA, state1);
    } else if (customKey == '*') {
      endServiceA();
      //reset everything on userA
    }
  } else if (page == 7) { //BOX B PAGE
    //=====LIVE TIMER=====
    if (timer2 == false && userB == true) {
      //Serial.println(initial1);
      if (initial2 == false) {
        lcd.setCursor(0, 1);
        lcd.print("     Time is up!    ");
      } else {
        lcd.setCursor(0, 1);
        lcd.print("  device is ready   ");
      }
      //  Serial.print(initial1);
    } else if (timer2 == true && userB == true) {
      lcd.setCursor(12, 1);
      lcd.print(minCounter2);
      if (minCounter2 > 10) {
        lcd.setCursor(14, 1);
        lcd.print(':');
        lcd.setCursor(15, 1);
      } else {
        lcd.setCursor(13, 1);
        lcd.print(':');
        lcd.setCursor(14, 1);
      }
      lcd.print(secCounter2);
      lcd.print(' ');
    }
    //====================
    if (customKey == 'D') {
      refresh();
      menu();
      page = 0;
    } else if (customKey == '#') {
      state2 = !state2;
      digitalWrite(solenoidB, state2);
    } else if (customKey == '*') {
      endServiceB();
      //reset everything on userA
    }
  }
}





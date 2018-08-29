void del() {
  d--;
  stringBuffer[d] = 0;
  lcd.setCursor(d, 1);
  lcd.print(' ');

  if (d < 0) {
    d = 0;
  }
}

void pword() {

  lcd.setCursor(d, 1);
  lcd.print('*');
  d++;
  if (d > 7) {
    d = 7;
  }
}

void alarmCall() {
  if (passCounter == 3) {
    page = 5;
    digitalWrite(piezoPin, HIGH);
  }
}

void refresh() {
  for (d = 0; d < 7; d++) {//reset the buffers
    stringBuffer[d] = 0;
    input1[d] = 0;
    input2[d] = 0;
    reEnterA = false;
    reEnterB = false;
  }
  d = 0;
}

void validateAdmin() {
  if (stringBuffer[0] == adminPass[0] && stringBuffer[1] == adminPass[1] && stringBuffer[2] == adminPass[2] && stringBuffer[3] == adminPass[3] && stringBuffer[4] == adminPass[4] && stringBuffer[5] == adminPass[5] && stringBuffer[6] == adminPass[6]) {
    adminValid();
    page = 5;
  } else {
    lcd.setCursor(0, 1);
    lcd.print("  Password Invalid");
    delay(700);
    refresh();
    lcd.setCursor(0, 1);
    lcd.print("                  ");
  }
}

void triggers() {
  if (trigger1 == true) {
    if (digitalRead(boxSwitchA) == HIGH && buttonstate1 == LOW) {
      if (initial1 == true) {
        buttonstate1 = HIGH;
        initial1 = false;
        if (page == 6) {
          lcd.setCursor(0, 1);
          lcd.print("expires in:");
        }
      }
      //do something
      digitalWrite(relayPinA, HIGH); //turn on the charger
      minCounter1 = mintimer1;
      secCounter1 = sectimer1;
      timer1 = true;//run millis
    }
    else {
      buttonstate1 = digitalRead(boxSwitchA);
    }
  }

  if (trigger2 == true) {
    if (digitalRead(boxSwitchB) == HIGH && buttonstate2 == LOW) {
      if (initial2 == true) {
        buttonstate2 = HIGH;
        initial2 = false;
        if (page == 7) {
          lcd.setCursor(0, 1);
          lcd.print("expires in:");
        }
      }
      //do something
      digitalWrite(relayPinB, HIGH); //turn on the charger
      minCounter2 = mintimer2;
      secCounter2 = sectimer2;
      timer2 = true;//run millis
    }
    else {
      buttonstate2 = digitalRead(boxSwitchB);
    }
  }
}

void adminLock() {
  page = 4;
  admin();
  digitalWrite(piezoPin, HIGH);
}


void validateBoxA() {
  if (stringBuffer[0] == input1[0] && stringBuffer[1] == input1[1] && stringBuffer[2] == input1[2] && stringBuffer[3] == input1[3] && stringBuffer[4] == input1[4] && stringBuffer[5] == input1[5] && stringBuffer[6] == input1[6]) {
    digitalWrite(solenoidA, HIGH); //open the solenoid
    delay(700);//enough time for the spring to open the box
    reEnterA = false;
    userA = true;
    trigger1 = true; //enable box trigger (if closed)
    boxA();
    initial1 = true;
    page = 6;
    for (int q = 0; q < 7; q++) { //saves the pword to a buffer
      passboxA[q] = input1[q];
    }
    EEPROM.writeBlock<char>(addressCharArray1, passboxA, 7); //saves the buffer to EEPROM
  } else {
    lcd.setCursor(0, 1);
    lcd.print("  Password Invalid");

    delay(700);
    for (d = 0; d < 7; d++) {//reset the buffers
      input1[d] = 0;
    }
    d = 0;

    lcd.setCursor(0, 1);
    lcd.print("                  ");
  }
}

void validateLoginA() {
  if (stringBuffer[0] == passboxA[0] && stringBuffer[1] == passboxA[1] && stringBuffer[2] == passboxA[2] && stringBuffer[3] == passboxA[3] && stringBuffer[4] == passboxA[4] && stringBuffer[5] == passboxA[5] && stringBuffer[6] == passboxA[6]) {
    // initial1 = true;
    page = 6;
    boxA();
    // Serial.println(initial1);

  } else {
    lcd.setCursor(0, 1);
    lcd.print("  Password Invalid");
    delay(700);
    for (d = 0; d < 7; d++) {//reset the buffers
      stringBuffer[d] = 0;
    }
    d = 0;
    passCounter++;
    Serial.println(passCounter);
    if (passCounter > 2) {
      adminLock();
    }
    lcd.setCursor(0, 1);
    lcd.print("                  ");
  }
}

void TimerA() {
  if (timer1 == true) {
    unsigned long current1 = millis();

    if (current1 - prevMillis1 >= interval) {
      secCounter1--;
      if (secCounter1 < 1) { //if timer reaches 60 seconds
        minCounter1--;
        secCounter1 = 59;
        EEPROM.writeInt(addressInt1, minCounter1); //saves the minCounter to EEPROM

        if (minCounter1 < 0) {
          minCounter1 = 29;
          secCounter1 = 60;
          digitalWrite(relayPinA, LOW);
          timer1 = false;
          initial1 = false;
          trigger1 = false;
        }
      }

      prevMillis1 = current1;
    }
  }
}

void endServiceA() {

  userA = 0;
  minCounter1 = 1;
  secCounter1 = 60;

  reEnterA = false;
  timer1 = false;
  initial1 = false;
  trigger1 = false;
  state1 =  false;

  digitalWrite(relayPinA, LOW); //kill the charger
  digitalWrite(solenoidA, HIGH); //open the box
  digitalWrite(piezoPin,LOW);

  for (d = 0; d < 7; d++) {//reset the buffers
    stringBuffer[d] = 0;
    input1[d] = 0;
    passboxA[d] = 0;
  }
  EEPROM.writeBlock<char>(addressCharArray1, passboxA, 7); //saves the buffer to EEPROM
  if (page == 6) {
    lcd.setCursor(0, 1);
    lcd.print("      thank you     ");
  }
}

void validateBoxB() {
  if (stringBuffer[0] == input2[0] && stringBuffer[1] == input2[1] && stringBuffer[2] == input2[2] && stringBuffer[3] == input2[3] && stringBuffer[4] == input2[4] && stringBuffer[5] == input2[5] && stringBuffer[6] == input2[6]) {
    digitalWrite(solenoidB, HIGH); //open the solenoid
    delay(700);//enough time for the spring to open the box
    reEnterB = false;
    userB = true;
    trigger2 = true; //enable box trigger (if closed)
    boxB();
    initial2 = true;
    page = 7;
    for (int q = 0; q < 7; q++) { //saves the pword to a buffer
      passboxB[q] = input2[q];
    }
    EEPROM.writeBlock<char>(addressCharArray2, passboxB, 7);
  } else {
    lcd.setCursor(0, 1);
    lcd.print("  Password Invalid");

    delay(700);
    for (d = 0; d < 7; d++) {//reset the buffers
      input2[d] = 0;
    }
    d = 0;

    lcd.setCursor(0, 1);
    lcd.print("                  ");
  }
}

void validateLoginB() {
  if (stringBuffer[0] == passboxB[0] && stringBuffer[1] == passboxB[1] && stringBuffer[2] == passboxB[2] && stringBuffer[3] == passboxB[3] && stringBuffer[4] == passboxB[4] && stringBuffer[5] == passboxB[5] && stringBuffer[6] == passboxB[6]) {
    // initial1 = true;
    page = 7;
    boxB();
    // Serial.println(initial1);

  } else {
    lcd.setCursor(0, 1);
    lcd.print("  Password Invalid");
    delay(700);
    for (d = 0; d < 7; d++) {//reset the buffers
      stringBuffer[d] = 0;
    }
    d = 0;
    passCounter++;
    Serial.println(passCounter);
    if (passCounter > 2) {
      adminLock();
    }
    lcd.setCursor(0, 1);
    lcd.print("                  ");
  }
}

void TimerB() {
  if (timer2 == true) {
    unsigned long current2 = millis();

    if (current2 - prevMillis2 >= interval) {
      secCounter2--;
      if (secCounter2 < 1) { //if timer reaches 60 seconds
        minCounter2--;
        secCounter2 = 59;
        EEPROM.writeInt(addressInt2, minCounter2); //saves the minCounter to EEPROM
        if (minCounter2 < 0) {
          minCounter2 = 29;
          secCounter2 = 60;
          digitalWrite(relayPinB, LOW);
          timer2 = false;
          initial2 = false;
          trigger2 = false;
        }
      }

      prevMillis2 = current2;
    }
  }
}

void endServiceB() {
  userB = 0;
  minCounter2 = 29;
  secCounter2 = 60;

  reEnterB = false;
  timer2 = false;
  initial2 = false;
  trigger2 = false;
  state2 =  false;

  digitalWrite(relayPinB, LOW); //kill the charger
  digitalWrite(solenoidB, HIGH); //open the box
  digitalWrite(piezoPin,LOW);
  for (d = 0; d < 7; d++) {//reset the buffers
    stringBuffer[d] = 0;
    input2[d] = 0;
    passboxB[d] = 0;
  }
 EEPROM.writeBlock<char>(addressCharArray2, passboxB, 7); //saves the buffer to EEPROM
  if (page == 7) {
    lcd.setCursor(0, 1);
    lcd.print("      thank you     ");
  }
}




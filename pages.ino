void reEnterPW() {
  lcd.setCursor(3, 0);
  lcd.print("Re-Enter PWord");
}

void admin()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Admin Password:");
  lcd.setCursor(0, 2);
  lcd.print("# - Enter");
  lcd.setCursor(10, 2);
  lcd.print("* - Delete");
  if (page == 3) {
    lcd.setCursor(0, 3);
    lcd.print("D - Menu");
  } else if (page == 4) {

  }
}

void adminValid()
{
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Welcome Admin");
  lcd.setCursor(0, 1);
  lcd.print("1-Toggle ");
  lcd.setCursor(11, 1);
  lcd.print("2-Toggle ");
  lcd.setCursor(0, 2);
  lcd.print("A-[Rmv A]");
  lcd.setCursor(11, 2);
  lcd.print("B-[Rmv B]");
  lcd.setCursor(0, 3);
  lcd.print("D - Menu");
}

void menu()
{
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("MAIN MENU");
  lcd.setCursor(0, 1);
  lcd.print(" A - BOX 1");
  lcd.setCursor(0, 2);
  lcd.print(" B - BOX 2");
  lcd.setCursor(0, 3);
  lcd.print(" C - ADMIN MODE");
}



void boxA()
{
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Welcome to Box A");
  lcd.setCursor(0, 1);
  lcd.print("expires in:");
  lcd.setCursor(12, 1);
  lcd.print(minCounter1);
  lcd.setCursor(14, 1);
  lcd.print(':');
  lcd.setCursor(15, 1);
  lcd.print(secCounter1);
  lcd.setCursor(10, 2);
  lcd.print("# - Open");
  lcd.setCursor(0, 2);
  lcd.print("D - Menu");
  lcd.setCursor(0, 3);
  lcd.print("* - End Service   ");
}

void boxAlogin()
{
  lcd.clear();
  lcd.setCursor(1, 0);
  if (userA ==  true) {
    lcd.print("Please Enter Pword");
  } else {
    lcd.print("   Set Password:");
  }
  lcd.setCursor(10, 2);
  lcd.print("* - Delete");
  lcd.setCursor(0, 3);
  lcd.print("D - Menu");
}

void boxB()
{
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Welcome to Box B");
  lcd.setCursor(0, 1);
  lcd.print("expires in:");
  lcd.setCursor(12, 1);
  lcd.print(minCounter2);
  lcd.setCursor(14, 1);
  lcd.print(':');
  lcd.setCursor(15, 1);
  lcd.print(secCounter2);
  lcd.setCursor(10, 2);
  lcd.print("# - Open");
  lcd.setCursor(0, 2);
  lcd.print("D - Menu");
  lcd.setCursor(0, 3);
  lcd.print("* - End Service   ");
}

void boxBlogin()
{
  lcd.clear();
  lcd.setCursor(1, 0);
  if (userB ==  true) {
    lcd.print("Please Enter Pword");
  } else {
    lcd.print("   Set Password:");
  }
  lcd.setCursor(10, 2);
  lcd.print("* - Delete");
  lcd.setCursor(0, 3);
  lcd.print("D - Menu");
}

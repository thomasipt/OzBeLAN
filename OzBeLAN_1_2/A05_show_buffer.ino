void showbuffer() {

  if (arrow == true) {
    unsigned long currentARROWMillis = millis();
    if (currentARROWMillis - previousARROWMillis > (400))
    {
      previousARROWMillis = currentARROWMillis;
      arrowstep = !arrowstep;
    }
    lcd.setCursor(0, 0);
    if (arrowstep == true) lcd.print (">"); else lcd.print ("-");
    lcd.print( LCDbuffer1 );
    lcd.print(lcdspaces);
    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.print( LCDbuffer2 );
    lcd.print(lcdspaces);
  }

  if (pinging == true) {
    unsigned long currentARROWMillis = millis();
    if (currentARROWMillis - previousARROWMillis > (400))
    {
      previousARROWMillis = currentARROWMillis;
      arrowstep = !arrowstep;
    }
    lcd.setCursor(0, 0);
    lcd.print( LCDbuffer1 );
    lcd.print(lcdspaces);
    lcd.setCursor(0, 1);
    if (arrowstep == true) lcd.print ("+"); else lcd.print ("*");
    for (byte i = 0; i < 4; i++) {
      if (pingReturn[i] == 1)
      {
        lcd.print(" OK");
      }
      else
      {
        lcd.print(" --");
      }
    }
    lcd.print(lcdspaces);
  }

  if (edit == true && change == false) {
    unsigned long currentARROWMillis = millis();
    if (currentARROWMillis - previousARROWMillis > (400))
    {
      previousARROWMillis = currentARROWMillis;
      arrowstep = !arrowstep;
    }
    lcd.setCursor(0, 0);
    lcd.print( LCDbuffer1 );
    lcd.print(lcdspaces);
    lcd.setCursor(0, 1);
    lcd.print( LCDbuffer2 );
    if (arrowstep == true) lcd.print ("?"); else lcd.print ("!");
    lcd.print(lcdspaces);
  }
  if (edit == true && change == true) {
    unsigned long currentARROWMillis = millis();
    if (currentARROWMillis - previousARROWMillis > (300))
    {
      previousARROWMillis = currentARROWMillis;
      arrowstep = !arrowstep ;
    }
    lcd.setCursor(0, 0);
    lcd.print( LCDbuffer1 );
    lcd.print(lcdspaces);
    if (arrowstep == true) {
      lcd.setCursor(0, 1);
      lcd.print(LCDbuffer2);
      lcd.print(lcdspaces);
    }
    else
    {
      lcd.setCursor(CursorPosition, 1);
      lcd.print(" ");
    }
  }
  if (edit == false && arrow == false && change == false && pinging == false) {
    lcd.setCursor(0, 0);
    lcd.print( LCDbuffer1 );
    lcd.print(lcdspaces);
    lcd.setCursor(0, 1);
    lcd.print( LCDbuffer2 );
    lcd.print(lcdspaces);
  }
}

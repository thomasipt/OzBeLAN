void buttonpress() {
  debouncerUP.update();
  debouncerDOWN.update();
  debouncerSELECT.update();

  if (debouncerUP.fell()) {
    button3 = true;
  }
  if (debouncerDOWN.fell()) {
    button2 = true;
  }
  if (debouncerSELECT.fell()) {
    button1 = true;
  }

  unsigned long currentAGBMillis = millis();
  if (currentAGBMillis - previousAGBMillis > (deviceParameters[4] * 1000L) && (level != 0) && (pinging == false)) {
    previousAGBMillis = currentAGBMillis;
    level = 0;
    levelstep = 0;
    sublevel = 0;
    editstep = 0;
    savestep = 0;
    memset(pingReturn, 0x00, sizeof(pingReturn));
  }

  unsigned long currentDBLMillis = millis();
  if ((currentDBLMillis - previousDBLMillis > (deviceParameters[1] * 1000L)) && (pinging == false)) {
    previousDBLMillis = currentDBLMillis;
  }

  if (button1 == true || button2 == true || button3 == true) {
    previousAGBMillis = currentAGBMillis;
    PortConnected = false;
    if (deviceParameters[1] != 0) {
      previousDBLMillis = currentDBLMillis;
      SetLevelBacklight();
    }
  }
}



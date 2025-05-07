void setup() {
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(SELECT, INPUT_PULLUP);

  debouncerUP.attach(UP);
  debouncerDOWN.attach(DOWN);
  debouncerSELECT.attach(SELECT);

  debouncerUP.interval(1);
  debouncerDOWN.interval(1);
  debouncerSELECT.interval(1);

  lcd.autoAddress();
  lcd.begin();
  lcd.print("TESTLAN V.");
  lcd.print(LCDbuffer1);
  lcd.setCursor(0, 1);
  lcd.print("SETTLING");
  delay(100);

  boolean point = true;
  for (byte x = 0; x < 6; x++) {
    lcd.setCursor(10, 1);
    for (byte i = 0; i < 5; i++) {
      if (point == true) lcd.print(".");
      else lcd.print(" ");
      delay(100);
    }
    point = !point;
  }

  lcd.clear();
  lcd.print("WE ARE HAVING A");
  lcd.setCursor(0, 1);
  lcd.print(" DHCP PROBLEM ! ");

  if (EEPROM.read(1023) != ID) {
    Ethernet.begin(mac, ip, dnsserver, gateway, subnet);
  }

  if (EEPROM.read(1023) == ID) {
    for (byte i = 0; i < 6; i++) {
      mac[i] = EEPROM.read(i + 999);
    }

    staticdhcp = EEPROM.read(993);

    if (staticdhcp == 2) {
      if (Ethernet.begin(mac) != 0) {
        for (byte i = 0; i < 4; i++) {
          ip[i] = Ethernet.localIP()[i];
          subnet[i] = Ethernet.subnetMask()[i];
          gateway[i] = Ethernet.gatewayIP()[i];
          dnsserver[i] = Ethernet.dnsServerIP()[i];
          dhcpip[i] = Ethernet.dhcpServerIP()[i];
          if (EEPROM.read(989) != 0) {
            cloudip[i] = EEPROM.read(i + 989);
          }
        }
      }
      else {
        fail = true;
        Ethernet.begin(mac, ip, dnsserver, gateway, subnet);
      }
    }

    else {
      for (byte i = 0; i < 4; i++) {
        if (EEPROM.read(1005) != 0) {
          ip[i] = EEPROM.read(i + 1005);
        }
        if (EEPROM.read(1009) != 0) {
          subnet[i] = EEPROM.read(i + 1009);
        }
        if (EEPROM.read(1013) != 0) {
          gateway[i] = EEPROM.read(i + 1013);
        }
        if (EEPROM.read(1017) != 0) {
          dnsserver[i] = EEPROM.read(i + 1017);
        }
        if (EEPROM.read(989) != 0) {
          cloudip[i] = EEPROM.read(i + 989);
        }
      }
      Ethernet.begin(mac, ip, dnsserver, gateway, subnet);
    }
    testport = 0;
    testport = EEPROM.read(1021);
    testport = (testport << 8) + EEPROM.read(1022);

    if (EEPROM.read(994) != 0) {
      deviceParameters[0] = EEPROM.read(994);
    }
    if (EEPROM.read(995) != 0) {
      deviceParameters[1] = EEPROM.read(995);
    }
    if (EEPROM.read(996) != 0) {
      deviceParameters[2] = EEPROM.read(996);
    }
    if (EEPROM.read(997) != 0) {
      deviceParameters[3] = EEPROM.read(997);
    }
    if (EEPROM.read(998) != 0) {
      deviceParameters[4] = EEPROM.read(998);
    }
  }
  lcd.clear();
  Udp.begin(testport);
  SetLevelBacklight();
}

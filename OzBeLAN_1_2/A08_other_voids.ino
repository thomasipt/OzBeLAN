void Reboot() {
  noInterrupts();
  __asm("wdr");
  WDTCSR = (1 << WDCE) | (1 << WDE);
  WDTCSR = (1 << WDE);
  for (;;) {};
}

void runpingtest(byte *pingaddress) {
  unsigned long currentPINGMillis = millis();
  if ((currentPINGMillis - previousPINGMillis > (1000L)) && (pingStep < 5))
  {
    previousPINGMillis = currentPINGMillis;
    if (pingaddress[0] != 0) {
      ICMPPing ping(pingSocket);
      pingReturn[pingStep] = ping(4, pingaddress, pingbuffer);
    }
    else {
      level = 4;
      pingStep = 0;
    }
    pingStep++;
    if (pingStep >= 5) {
      level = 4;
      pingStep = 0;
    }
  }
}

void SetLevelBacklight() {
  byte BlackLightLevel;
  BlackLightLevel = map(deviceParameters[2], 0, 10, 0, 255);
}

void SaveAll() {
  for (byte i = 0; i < 8; i++) {
    levelstep = i;
    SaveConfig();
  }
  for (byte i = 0; i < 5; i++) {
    levelstep = i;
    SaveSetup();
  }
  EEPROM.write(1023, ID);
}

void GoToLevel2() {
  level = 2;
  editstep = 0;
  savestep = 0;
  pingStep = 0;
  memset(pingReturn, 0x00, sizeof(pingReturn));
}

void EraseAll() {
  for (int i = 0; i < 1024; i++) {
    EEPROM.write(i, 0);
  }
}

void FillBufferMAC (unsigned char *address) {
  memset(waitbuffer, ' ', 16);
  for (byte i = 0; i < 6 ; i++) {
    AddZeroMac (address[i]);
    for (byte y = 0; y < 2; y++) {
      waitbuffer[y + 2 * i] = FlowText[y];
    }
  }
}

void FillBufferIP (unsigned char *address) {
  memset(waitbuffer, ' ', 16);
  for (byte i = 0; i < 4 ; i++) {
    AddZeroIP (address[i]);
    for (byte y = 0; y < 3; y++) {
      waitbuffer[y + 4 * i] = FlowText[y];
    } if (i < 3) waitbuffer[3 + 4 * i] = '.';
  }
}

void FillBufferPORT (unsigned int port) {
  memset(waitbuffer, ' ', 16);
  AddZeroPORT (port);
  for (byte i = 0; i < 5; i++) {
    waitbuffer[i] = FlowText[i];
  }
}

void AddZeroMac (unsigned int digit) {
  if (digit < 10) sprintf(FlowText, "0%X", digit);
  if (digit > 9) sprintf(FlowText, "%X", digit);
}

void AddZeroIP (unsigned int digit) {
  if (digit < 10) sprintf(FlowText, "00%d", digit);
  if (digit > 9 && digit < 100) sprintf(FlowText, "0%d", digit);
  if (digit > 99) sprintf(FlowText, "%d", digit);
}

void AddZeroPORT (unsigned int digit) {
  if (digit < 10) sprintf(FlowText, "0000%d", digit);
  if (digit > 9 && digit < 100) sprintf(FlowText, "000%d", digit);
  if (digit > 99 && digit < 1000) sprintf(FlowText, "00%d", digit);
  if (digit > 999 && digit < 10000) sprintf(FlowText, "0%d", digit);
  if (digit > 9999) sprintf(FlowText, "%d", digit);
}

void SaveConfig() {
  switch (levelstep) {
    case 0: for (byte i = 0; i < 6; i++) {
        EEPROM.write(i + 999, mac[i]);
      } break;
    case 1: EEPROM.write(993, staticdhcp); break;

    case 2: for (byte i = 0; i < 4; i++) {
        EEPROM.write(i + 1005, ip[i]);
      } break;
    case 3: for (byte i = 0; i < 4; i++) {
        EEPROM.write(i + 1009, subnet[i]);
      } break;
    case 4: for (byte i = 0; i < 4; i++) {
        EEPROM.write(i + 1013, gateway[i]);
      } break;
    case 5: for (byte i = 0; i < 4; i++) {
        EEPROM.write(i + 1017, dnsserver[i]);
      } break;
    case 6: for (byte i = 0; i < 4; i++) {
        EEPROM.write(i + 989, cloudip[i]);
      } break;

    case 7: EEPROM.write(1021, testport >> 8); EEPROM.write(1022, testport); break;
  }
  if (sublevel != 3) {
    EEPROM.write(1023, ID);
  }
}

void SetSetup() {
  deviceParameters[levelstep] = editstep; if (levelstep == 2) {
    SetLevelBacklight();
  }
}

void SaveSetup() {
  EEPROM.write((994 + levelstep), deviceParameters[levelstep]);
  if (sublevel != 3) {
    EEPROM.write(1023, ID);
  }
}

void CharToValue (char* chars) {
  byte char1; byte char2; byte char3; byte char4; byte char5;
  byte charplace[5];
  switch (levelstep) {
    case 0:
      for (byte a = 0; a < 6; a++)
      {
        for (byte i = 0; i < 16; i++)
        {
          for (byte b = 0; b < 2; b++) {
            if (chars[(a * 2) + b] == hexa[i]) charplace[b] = i;
          }
        }
        mac[a] = ((charplace[0] * 16) + charplace[1]);
      }
      break;
    case 2: case 3: case 4: case 5: case 6:
      for (byte a = 0; a < 4; a++)
      {
        for (byte i = 0; i < 10; i++)
        {
          for (byte b = 0; b < 3; b++) {
            if (chars[(a * 4) + b] == hexa[i]) charplace[b] = i;
          }
        }
        switch (levelstep)
        {
          case 2: ip[a] = ((charplace[0] * 100) + (charplace[1] * 10) + charplace[2]); break;
          case 3: subnet[a] = ((charplace[0] * 100) + (charplace[1] * 10) + charplace[2]); break;
          case 4: gateway[a] = ((charplace[0] * 100) + (charplace[1] * 10) + charplace[2]); break;
          case 5: dnsserver[a] = ((charplace[0] * 100) + (charplace[1] * 10) + charplace[2]); break;
          case 6: cloudip[a] = ((charplace[0] * 100) + (charplace[1] * 10) + charplace[2]); break;
        }
      }
      break;
    case 7:
      for (byte i = 0; i < 10; i++)
      {
        for (byte b = 0; b < 5; b++) {
          if (chars[b] == hexa[i]) charplace[b] = i;
        }
      }
      testport = ((charplace[0] * 10000) + (charplace[1] * 1000) + (charplace[2] * 100) + (charplace[3] * 10) + charplace[4]);
      Udp.stop();
      Udp.begin(testport);
      break;
  }
}

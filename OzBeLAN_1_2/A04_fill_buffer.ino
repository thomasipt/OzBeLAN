void fillbuffer() {
  switch (PortConnected) {
    case false: {
        switch (level) {
          case 0:
            {
              memset (LCDbuffer1, ' ', 15);
              memset (LCDbuffer2, ' ', 16);
              arrow = false; edit = false; change = false; pinging = false;
              if (button1 == true) {
                button1 = false;
                level = 1;
                levelstep = 0;
                break;
              }
              if (button2 == true) {
                button2 = false;
                levelstep++;
                if (levelstep > 7) levelstep = 0;
              }
              if (button3 == true) {
                button3 = false;
                levelstep--;
                if (levelstep < 0) levelstep = 7;
              }


              unsigned long currentLCDMillis = millis();
              if (currentLCDMillis - previousLCDMillis > (deviceParameters[0] * 1000))
              {
                previousLCDMillis = currentLCDMillis;
                if (firstboot == false) {
                  levelstep++;
                }
                firstboot = false;
                if (levelstep > 7) levelstep = 0;
              }
              strcpy_P(LCDbuffer1, (char*)pgm_read_word(&(string_table[levelstep])));

              switch (levelstep)
              {
                case 0:
                  if (EEPROM.read(1023) != ID) {
                    strcpy_P(LCDbuffer2, (char*)pgm_read_word(&(string_table[0 + 8])));
                  }
                  else
                  {
                    if (staticdhcp == 2)
                    {
                      if (fail == false) {
                        strcpy_P(LCDbuffer2, (char*)pgm_read_word(&(string_table[2 + 8])));
                      }
                      else {
                        strcpy_P(LCDbuffer2, (char*)pgm_read_word(&(string_table[3 + 8])));
                      }
                    }
                    else
                    {
                      strcpy_P(LCDbuffer2, (char*)pgm_read_word(&(string_table[1 + 8])));
                    }
                  }
                  break;
                case 1: FillBufferMAC (mac); strncpy(LCDbuffer2, waitbuffer, 12); break;
                case 6: FillBufferPORT (testport); strncpy(LCDbuffer2, waitbuffer, 16); break;
                case 7:
                  {
                    if (staticdhcp != 2)
                    {
                      strcpy_P(LCDbuffer2, (char*)pgm_read_word(&(string_table[4 + 8])));
                    }
                    else {
                      FillBufferIP (dhcpip);
                      strncpy(LCDbuffer2, waitbuffer, 15);
                    }
                    break;
                  }
                default: FillBufferIP (bufferIP[levelstep - 2]); strncpy(LCDbuffer2, waitbuffer, 15); break;
              }
              break;
            }

          case 1:
            {
              memset (LCDbuffer1, ' ', 15);
              memset (LCDbuffer2, ' ', 16);
              arrow = true; edit = false; change = false; pinging = false;
              if (button1 == true) {
                button1 = false;
                if (levelstep == 5) {
                  level = 0;
                  levelstep = 0;
                  break;
                } else {
                  level = 2;
                  sublevel = levelstep;
                  levelstep = 0;
                  break;
                }
              }
              if (button2 == true) {
                button2 = false;
                levelstep++;
                if (levelstep > 5) levelstep = 0;
              }
              if (button3 == true) {
                button3 = false;
                levelstep--;
                if (levelstep < 0) levelstep = 5;
              }
              strcpy_P(LCDbuffer1, (char*)pgm_read_word(&(string_table[levelstep + 13])));
              strcpy_P(LCDbuffer2, (char*)pgm_read_word(&(string_table[levelstep + 1 + 13])));
              break;
            }

          case 2:
            {
              memset (LCDbuffer1, ' ', 15);
              memset (LCDbuffer2, ' ', 16);
              byte maxvalues[] = {8, 5, 5, 4, 0};
              byte StringTablePlaces[] = {20, 30, 37, 44, 50};
              arrow = true; edit = false; change = false; pinging = false;
              if (button1 == true)
              {
                button1 = false;
                switch (sublevel)
                {
                  case 0:
                    {
                      CursorPosition = 0;
                      readmychar = true;
                      switch (levelstep)
                      {
                        case 0: FillBufferMAC (mac); strncpy (changebuffer, waitbuffer, 12); level = 3; break;
                        case 1: editstep = staticdhcp - 1; level = 3; break;
                        case 8: levelstep = sublevel; sublevel = 0; level = 1; break;
                        default: FillBufferIP (bufferIP[levelstep - 2]); strncpy (changebuffer, waitbuffer, 16); level = 3; break;
                      }
                      break;
                    }
                  case 1: {
                      level = 3;
                      if (levelstep == 5) {
                        levelstep = sublevel;
                        sublevel = 0;
                        level = 1;
                      } break;
                    }
                  case 2: {
                      if (levelstep == 5) {
                        levelstep = sublevel;
                        sublevel = 0;
                        level = 1;
                      } else {
                        editstep = deviceParameters[levelstep];
                        level = 3;
                      } break;
                    }
                  case 3: {
                      level = 3;
                      if (levelstep == 4) {
                        levelstep = sublevel;
                        sublevel = 0;
                        level = 1;
                      } break;
                    }
                  case 4: {
                      levelstep = sublevel;
                      sublevel = 0;
                      level = 1;
                      break;
                    }
                }
              }

              if (button2 == true) {
                button2 = false;
                levelstep++;
                if (levelstep > maxvalues[sublevel]) {
                  levelstep = 0;
                }
              }
              if (button3 == true) {
                button3 = false;
                levelstep--;
                if (levelstep < 0) {
                  levelstep = maxvalues[sublevel];
                }
              }

              strcpy_P(LCDbuffer1, (char*)pgm_read_word(&(string_table[levelstep + (StringTablePlaces[sublevel])])));
              strcpy_P(LCDbuffer2, (char*)pgm_read_word(&(string_table[levelstep + 1 + (StringTablePlaces[sublevel])])));
              break;
            }

          case 3:
            {
              memset (LCDbuffer1, ' ', 15);
              memset (LCDbuffer2, ' ', 16);
              byte minvalues[] = {15, 1, 9, 9, 9, 9, 9, 9};
              byte maxvalues[] = {60, 240, 10, 60, 240};
              arrow = false; edit = true; pinging = false;
              if (sublevel == 3) {
                editstep = levelstep;
                level = 4;
              }

              if (button1 == true)
              {
                button1 = false;
                CursorPosition++;
                switch (sublevel)
                {
                  case 0:
                    {
                      switch (levelstep)
                      {
                        case 0: {
                            readmychar = true;
                            if (CursorPosition > 11) {
                              level = 4;
                              editstep = 0;
                            } break;
                          }
                        case 1: {
                            level = 4;
                            break;
                          }
                        case 7: readmychar = true; if (CursorPosition > 4) {
                            level = 4;
                          } break;
                        default:
                          {
                            readmychar = true;
                            if (CursorPosition == 3 || CursorPosition == 7 || CursorPosition == 11) {
                              CursorPosition++;
                            }
                            if (CursorPosition == 15) {
                              level = 4;
                            }

                          }
                          break;
                      }
                      break;
                    }
                  case 1: {
                      GoToLevel2();
                      break;
                    }
                  case 2: {
                      level = 4;
                      break;
                    }
                }
              }
              if (button2 == true)
              {
                button2 = false;
                editstep--;
                switch (sublevel)
                {
                  case 0: {
                      if (editstep < 0) {
                        editstep = (minvalues[levelstep]);
                      } break;
                    }
                  case 2: {
                      if (editstep < 0) {
                        editstep = (maxvalues[levelstep]);
                      } break;
                    }
                }
              }
              if (button3 == true)
              {
                button3 = false;
                editstep++;
                switch (sublevel)
                {
                  case 0: {
                      if (editstep > (minvalues[levelstep])) {
                        editstep = 0;
                      } break;
                    }
                  case 2: {
                      if (editstep > (maxvalues[levelstep])) {
                        editstep = 0;
                      } break;
                    }
                }
              }
              switch (sublevel)
              {
                case 0:
                  {
                    change = true;
                    strcpy_P(LCDbuffer1, (char*)pgm_read_word(&(string_table[levelstep + 20])));
                    switch (levelstep)
                    {
                      case 1:
                        change = false;
                        strcpy_P(LCDbuffer1, (char*)pgm_read_word(&(string_table[levelstep + 20])));
                        strcpy_P(LCDbuffer2, (char*)pgm_read_word(&(string_table[editstep + 3 + 52])));
                        break;
                      default:
                        if (readmychar == true)
                        {
                          readmychar = false;
                          readchar[0] = changebuffer[CursorPosition];
                          for (byte i = 0; i < 16; i++) {
                            if (readchar[0] == hexa[i]) {
                              editstep = i;
                            }
                          }
                        }
                        changebuffer[CursorPosition] = hexa[editstep];
                        strncpy (LCDbuffer2, changebuffer, 16);
                        break;
                    }
                    break;
                  }
                case 1:
                  {
                    edit = false;
                    pinging = true;
                    if (levelstep == 4) {
                      FindMyIP();
                      FilterMyIP();
                      level = 4;
                    }
                    else FillBufferIP(allip[levelstep]); strncpy (LCDbuffer1, waitbuffer, 15); runpingtest(allip[levelstep]);
                    break;
                  }
                case 2:
                  {
                    strcpy_P(LCDbuffer1, (char*)pgm_read_word(&(string_table[levelstep + 37])));
                    sprintf_P(LCDbuffer2, (char*)pgm_read_word(&(string_table[levelstep + 63])), editstep);
                    break;
                  }
              }
              break;
            }

          case 4:
            {
              memset (LCDbuffer1, ' ', 15);
              memset (LCDbuffer2, ' ', 16);
              byte maxvalue[] = {2, 0, 2, 1};
              arrow = false; edit = true; change = false; pinging = false;
              if (button1 == true)
              {
                button1 = false;
                switch (sublevel)
                {
                  case 0:
                    {
                      if (levelstep == 1) {
                        staticdhcp = editstep + 1;
                      }
                      else {
                        CharToValue(changebuffer);
                      }
                      if (savestep == 2) {
                        SaveConfig();
                      }
                      break;
                    }
                  case 2:
                    {
                      switch (savestep)
                      {
                        case 1: {
                            SetSetup();
                            break;
                          }
                        case 2: {
                            SetSetup();
                            SaveSetup();
                            break;
                          }
                      }
                      break;
                    }
                  case 3:
                    {
                      if (savestep == 1)
                      {
                        switch (levelstep)
                        {
                          case 0: {
                              SaveAll();
                              Reboot();
                              break;
                            }
                          case 1: {
                              SaveAll();
                              break;
                            }
                          case 2: {
                              Reboot();
                              break;
                            }
                          case 3: {
                              EraseAll();
                              Reboot();
                              break;
                            }
                        }
                      }
                      break;
                    }
                }
                GoToLevel2();
              }
              if (button2 == true) {
                button2 = false;
                savestep++;
                if (savestep > maxvalue[sublevel]) {
                  savestep = 0;
                }
              }

              if (button3 == true) {
                button3 = false;
                savestep--;
                if (savestep < 0) {
                  savestep = maxvalue[sublevel];
                }
              }

              switch (sublevel)
              {
                case 0:
                  {
                    switch (levelstep)
                    {
                      case 0: {
                          strncpy (LCDbuffer1, changebuffer, 12);
                          break;
                        }
                      case 1: {
                          strcpy_P(LCDbuffer1, (char*)pgm_read_word(&(string_table[editstep + 3 + 52])));
                          break;
                        }
                      case 7: {
                          strncpy (LCDbuffer1, changebuffer, 5);
                          break;
                        }
                      default: {
                          strncpy (LCDbuffer1, changebuffer, 15);
                          break;
                        }
                    }
                    strcpy_P(LCDbuffer2, (char*)pgm_read_word(&(string_table[savestep + 52])));
                    break;
                  }
                case 1:
                  {
                    edit = false;
                    if (levelstep == 4)
                    {
                      strcpy_P(LCDbuffer1, (char*)pgm_read_word(&(string_table[0 + 60])));
                      strncpy (LCDbuffer2, wanip, 15);
                      break;
                    }
                    else
                    {
                      FillBufferIP(allip[levelstep]); strncpy (LCDbuffer2, waitbuffer, 15);
                      if (pingReturn[0] == 1 && pingReturn[1] == 1 && pingReturn[2] == 1 && pingReturn[3] == 1)
                      {
                        strcpy_P(LCDbuffer1, (char*)pgm_read_word(&(string_table[0 + 61])));
                      }
                      else
                      {
                        strcpy_P(LCDbuffer1, (char*)pgm_read_word(&(string_table[0 + 62])));
                      }
                    }
                    break;
                  }
                case 2:
                  {
                    sprintf_P(LCDbuffer1, (char*)pgm_read_word(&(string_table[levelstep + 63])), editstep);
                    strcpy_P(LCDbuffer2, (char*)pgm_read_word(&(string_table[savestep + 52])));
                    break;
                  }
                case 3:
                  {
                    strcpy_P(LCDbuffer1, (char*)pgm_read_word(&(string_table[editstep + 44])));
                    strcpy_P(LCDbuffer2, (char*)pgm_read_word(&(string_table[savestep + 5 + 52])));
                    break;
                  }
                case 4: {
                    break;
                  }
              }
              break;
            }
        }
        break;
      }
    case true:
      {
        arrow = false;
        edit = false;
        change = false;
        pinging = false;
        flash = flash + 5;
        if (flash >= 255) {
          flash = 0;
        }
        strcpy_P(LCDbuffer1, (char*)pgm_read_word(&(string_table[7 + 52])));
        sprintf_P(LCDbuffer2, (char*)pgm_read_word(&(string_table[0 + 64])), testport);
        break;
      }
  }
}

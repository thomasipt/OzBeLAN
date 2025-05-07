char LCDbuffer1[16] = "1.0";

#include <SPI.h>
#include <Ethernet.h>
#include <EEPROM.h>
#include <ICMPPing.h>
#include <Bounce2.h>

#include <avr/pgmspace.h>
const byte ID = 0x81;

byte staticdhcp = 1;
boolean fail = false;
byte mac[] = {0xDE, 0xAD, 0xBE, 0x00, 0xFE, 0x29 };
byte ip[] =  {192, 168, 0, 100};
byte subnet[] = {255, 255, 255, 0};
byte gateway[] = {192, 168, 0, 1};
byte dnsserver[] = {192, 168, 0, 1};
byte dhcpip[4];
byte cloudip[] = {8, 8, 8, 8};
char wanip[16];

byte *allip[] = {dhcpip, gateway, dnsserver, cloudip};
byte *bufferIP[] = {ip, subnet, gateway, dnsserver, cloudip};

EthernetUDP Udp;
long testport = 8080;

#define UP 5
#define DOWN 6
#define SELECT 7

Bounce debouncerUP = Bounce();
Bounce debouncerDOWN = Bounce();
Bounce debouncerSELECT = Bounce();

boolean button1 = false;
boolean button2 = false;
boolean button3 = false;

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(16, 2);
byte CursorPosition = 0;

boolean arrow = false;
boolean edit = false;
boolean change = false;
boolean pinging = false;
boolean getbuffer = false;
boolean readmychar = false;
boolean PortConnected = false;
boolean arrowstep = true;
boolean firstboot = true;

long previousLCDMillis = 0;
long previousARROWMillis = 0;
long previousAGBMillis = 0;
long previousDBLMillis = 0;
long previousPINGMillis = 0;

int flash = 0;

byte deviceParameters[] = {2, 20, 10, 30, 120};

int level = 0;
int levelstep = 0;
int sublevel = 0;
int editstep = 0;
int savestep = 0;

char LCDbuffer2[17];
char waitbuffer[17];
char changebuffer[17];
char FlowText[4];
char readchar[1];
char lcdspaces[] = "                ";
char hexa[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

const char lcda0[] PROGMEM = "ETH-SHIELD UP";
const char lcda1[] PROGMEM = "MAC-ADDRESS";
const char lcda2[] PROGMEM = "IP-ADDRESS";
const char lcda3[] PROGMEM = "MASK-ADDRESS";
const char lcda4[] PROGMEM = "GW-ADDRESS";
const char lcda5[] PROGMEM = "DNS-ADDRESS";
const char lcda6[] PROGMEM = "TEST-PORT";
const char lcda7[] PROGMEM = "DHCP-SERVER";

const char lcdb0[] PROGMEM = "IP'S FROM SKETCH";
const char lcdb1[] PROGMEM = "IP'S FROM EEPROM";
const char lcdb2[] PROGMEM = "IP'S FROM DHCP";
const char lcdb3[] PROGMEM = "DHCP FAILED";
const char lcdb4[] PROGMEM = "DHCP NOT USED";

const char lcdc0[] PROGMEM = "CONFIG ETHERNET";
const char lcdc1[] PROGMEM = "RUN SOME TESTS";
const char lcdc2[] PROGMEM = "SETUP DEVICE";
const char lcdc3[] PROGMEM = "SAVE & REBOOT";
const char lcdc4[] PROGMEM = "SHOW CREDITS";
const char lcdc5[] PROGMEM = "GO 1 LEVEL BACK";

const char lcdd0[] PROGMEM = "EDIT MACADDRESS";
const char lcdd1[] PROGMEM = "STATIC OR DCHP";
const char lcdd2[] PROGMEM = "EDIT FIXED IP";
const char lcdd3[] PROGMEM = "EDIT SUBNETMASK";
const char lcdd4[] PROGMEM = "EDIT GATEWAY";
const char lcdd5[] PROGMEM = "EDIT DNS-SERVER";
const char lcdd6[] PROGMEM = "EDIT CLOUD-IP";
const char lcdd7[] PROGMEM = "EDIT TEST-PORT";

const char lcde0[] PROGMEM = "PING DHCPSERVER";
const char lcde1[] PROGMEM = "PING GATEWAY";
const char lcde2[] PROGMEM = "PING DNS-SERVER";
const char lcde3[] PROGMEM = "PING CLOUD IP";
const char lcde4[] PROGMEM = "FIND OUT WAN IP";

const char lcdf0[] PROGMEM = "DELAY CYCLES";
const char lcdf1[] PROGMEM = "DELAY BACKLIGHT";
const char lcdf2[] PROGMEM = "BACKLIGHT LEVEL";
const char lcdf3[] PROGMEM = "DELAY GO2SLEEP";
const char lcdf4[] PROGMEM = "AUTO BACK2INFO";

const char lcdg0[] PROGMEM = "SAVE & REBOOT";
const char lcdg1[] PROGMEM = "SAVE NO REBOOT";
const char lcdg2[] PROGMEM = "CANCEL & REBOOT";
const char lcdg3[] PROGMEM = "ERASE & REBOOT";

const char lcdh0[] PROGMEM = "MADE BY IPT";
const char lcdh1[] PROGMEM = "IT SOLUTION";

const char lcdi0[] PROGMEM = "NO/YES/SAVE: N";
const char lcdi1[] PROGMEM = "NO/YES/SAVE: Y";
const char lcdi2[] PROGMEM = "NO/YES/SAVE: S";
const char lcdi3[] PROGMEM = "STATIC/DHCP: S";
const char lcdi4[] PROGMEM = "STATIC/DHCP: D";
const char lcdi5[] PROGMEM = "NO/YES: N";
const char lcdi6[] PROGMEM = "NO/YES: Y";
const char lcdi7[] PROGMEM = "CONNECTION MADE";

const char lcdj0[] PROGMEM = "TESTLAN's WAN-IP";
const char lcdj1[] PROGMEM = "Test: SUCCESSFUL";
const char lcdj2[] PROGMEM = "Test: FAILED";
const char lcdj3[] PROGMEM = "%d SEC (0-60)";
const char lcdj4[] PROGMEM = "%d SEC (0-240)";
const char lcdj5[] PROGMEM = "%d LEVEL (0-10)";
const char lcdj6[] PROGMEM = "ON PORT: %d";

PGM_P const string_table[]  PROGMEM = {
  lcda0, lcda1, lcda2, lcda3, lcda4, lcda5, lcda6, lcda7,
  lcdb0, lcdb1, lcdb2, lcdb3, lcdb4,
  lcdc0, lcdc1, lcdc2, lcdc3, lcdc4, lcdc5, lcdc0,
  lcdd0, lcdd1, lcdd2, lcdd3, lcdd4, lcdd5, lcdd6, lcdd7, lcdc5, lcdd0,
  lcde0, lcde1, lcde2, lcde3, lcde4, lcdc5, lcde0,
  lcdf0, lcdf1, lcdf2, lcdf3, lcdf4, lcdc5, lcdf0,
  lcdg0, lcdg1, lcdg2, lcdg3, lcdc5, lcdg0,
  lcdh0, lcdh1,
  lcdi0, lcdi1, lcdi2, lcdi3, lcdi4, lcdi5, lcdi6, lcdi7,
  lcdj0, lcdj1, lcdj2, lcdj3, lcdj4, lcdj5, lcdj3, lcdj4, lcdj6
};

SOCKET pingSocket = 0;
char pingbuffer [256];
byte pingReturn[] = {0, 0, 0, 0, 0, 0};
byte pingStep = 0;

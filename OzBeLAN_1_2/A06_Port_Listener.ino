void portlistener() {
  memset (pingbuffer, 0x04, sizeof(pingbuffer));

  int packetSize = Udp.parsePacket();
  if (packetSize) {
    IPAddress remote = Udp.remoteIP();
    PortConnected = true;
    Udp.beginPacket(Udp.remoteIP(), testport);
    Udp.write("TESTLAN says HI");
    Udp.endPacket();
  }
  delay(10);
}

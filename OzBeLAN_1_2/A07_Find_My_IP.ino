void FindMyIP() {
  memset (pingbuffer, 0x04, sizeof(pingbuffer));
  memset(wanip, ' ', sizeof(wanip));
  EthernetClient client;
  if (client.connect("checkip.dyndns.org", 80)) {
    int i = 0;
    client.print("GET /");
    client.println();
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        pingbuffer[i] = c;
        i++;
      }
    }
    delay(1);
    client.stop();
  }
}

void FilterMyIP() {
  byte pos1;
  byte pos2;
  for (int i = 0; i < 256; i++) {
    if ( pingbuffer[i] == 's' && pingbuffer[i + 1] == 's' && pingbuffer[i + 2] == ':') {
      pos1 = i + 4;
    }
    if ( pingbuffer[i] == '<' && pingbuffer[i + 1] == '/' && pingbuffer[i + 2] == 'b') {
      pos2 = i;
    }
  }
  for (int i = pos1; i < pos2; i++) {
    wanip[i - pos1] = pingbuffer[i];
  }
}

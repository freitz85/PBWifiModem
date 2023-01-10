void wifiSetup(){
  if (tcpServerPort > 0) tcpServer.begin();

  WiFi.mode(WIFI_STA);
  connectWiFi();
  sendResult(R_OK);
  //tcpServer(tcpServerPort); // can't start tcpServer inside a function-- must live outside

  digitalWrite(LED_PIN, LOW); // on

  webServer.on("/", handleRoot);
  webServer.on("/ath", handleWebHangUp);
  webServer.begin();
  mdns.begin("C64WiFi", WiFi.localIP());  
}

void handleRoot() {
  String page = "WIFI STATUS: ";
  if (WiFi.status() == WL_CONNECTED) {
    page.concat("CONNECTED");
  }
  if (WiFi.status() == WL_IDLE_STATUS) {
    page.concat("OFFLINE");
  }
  if (WiFi.status() == WL_CONNECT_FAILED) {
    page.concat("CONNECT FAILED");
  }
  if (WiFi.status() == WL_NO_SSID_AVAIL) {
    page.concat("SSID UNAVAILABLE");
  }
  if (WiFi.status() == WL_CONNECTION_LOST) {
    page.concat("CONNECTION LOST");
  }
  if (WiFi.status() == WL_DISCONNECTED) {
    page.concat("DISCONNECTED");
  }
  if (WiFi.status() == WL_SCAN_COMPLETED) {
    page.concat("SCAN COMPLETED");
  }
  yield();
  page.concat("\nSSID.......: " + WiFi.SSID());

  byte mac[6];
  WiFi.macAddress(mac);
  page.concat("\nMAC ADDRESS: ");
  page.concat(String(mac[0], HEX));
  page.concat(":");
  page.concat(String(mac[1], HEX));
  page.concat(":");
  page.concat(String(mac[2], HEX));
  page.concat(":");
  page.concat(String(mac[3], HEX));
  page.concat(":");
  page.concat(String(mac[4], HEX));
  page.concat(":");
  page.concat(String(mac[5], HEX));
  yield();

  page.concat("\nIP ADDRESS.: "); page.concat(ipToString(WiFi.localIP()));
  page.concat("\nGATEWAY....: "); page.concat(ipToString(WiFi.gatewayIP()));
  yield();

  page.concat("\nSUBNET MASK: "); page.concat(ipToString(WiFi.subnetMask()));
  yield();
  page.concat("\nSERVER PORT: "); page.concat(tcpServerPort);
  page.concat("\nCALL STATUS: ");
  if (callConnected) {
    page.concat("CONNECTED TO ");
    if (ppp) {
      page.concat("PPP");
    } else {
      page.concat(ipToString(tcpClient.remoteIP()));
    }
    page.concat("\nCALL LENGTH: "); page.concat(connectTimeString()); yield();
  } else {
    page.concat("NOT CONNECTED");
  }
  page.concat("\n");
  webServer.send(200, "text/plain", page);
  delay(100);
}

void connectWiFi() {
  if (ssid == "" || password == "") {
    Serial.println("CONFIGURE SSID AND PASSWORD. TYPE AT? FOR HELP.");
    return;
  }
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.print("\nCONNECTING TO SSID "); Serial.print(ssid);
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) {
    digitalWrite(LED_PIN, LOW);
    delay(250);
    digitalWrite(LED_PIN, HIGH);
    delay(250);
    Serial.print(".");
  }
  Serial.println();
  if (i == 21) {
    Serial.print("COULD NOT CONNECT TO "); Serial.println(ssid);
    WiFi.disconnect();
    updateLed();
  } else {
    Serial.print("CONNECTED TO "); Serial.println(WiFi.SSID());
    Serial.print("IP ADDRESS: "); Serial.println(WiFi.localIP());
    updateLed();
    check_for_firmware_update();
  }
}

void updateLed() {
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_PIN, LOW);  // on
  } else {
    digitalWrite(LED_PIN, HIGH); //off
  }
}

void disconnectWiFi() {
  WiFi.disconnect();
  updateLed();
}

void displayNetworkStatus() {
  Serial.print("WIFI STATUS: ");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("CONNECTED");
  }
  if (WiFi.status() == WL_IDLE_STATUS) {
    Serial.println("OFFLINE");
  }
  if (WiFi.status() == WL_CONNECT_FAILED) {
    Serial.println("CONNECT FAILED");
  }
  if (WiFi.status() == WL_NO_SSID_AVAIL) {
    Serial.println("SSID UNAVAILABLE");
  }
  if (WiFi.status() == WL_CONNECTION_LOST) {
    Serial.println("CONNECTION LOST");
  }
  if (WiFi.status() == WL_DISCONNECTED) {
    Serial.println("DISCONNECTED");
  }
  if (WiFi.status() == WL_SCAN_COMPLETED) {
    Serial.println("SCAN COMPLETED");
  }
  yield();

  Serial.print("SSID.......: "); Serial.println(WiFi.SSID());

  //  Serial.print("ENCRYPTION: ");
  //  switch(WiFi.encryptionType()) {
  //    case 2:
  //      Serial.println("TKIP (WPA)");
  //      break;
  //    case 5:
  //      Serial.println("WEP");
  //      break;
  //    case 4:
  //      Serial.println("CCMP (WPA)");
  //      break;
  //    case 7:
  //      Serial.println("NONE");
  //      break;
  //    case 8:
  //      Serial.println("AUTO");
  //      break;
  //    default:
  //      Serial.println("UNKNOWN");
  //      break;
  //  }

  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC ADDRESS: ");
  Serial.print(mac[0], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.println(mac[5], HEX);
  yield();

  Serial.print("IP ADDRESS.: "); Serial.println(WiFi.localIP()); yield();
  Serial.print("GATEWAY....: "); Serial.println(WiFi.gatewayIP()); yield();
  Serial.print("SUBNET MASK: "); Serial.println(WiFi.subnetMask()); yield();
  Serial.print("SERVER PORT: "); Serial.println(tcpServerPort); yield();
  Serial.print("WEB CONFIG.: HTTP://"); Serial.println(WiFi.localIP()); yield();
  Serial.print("CALL STATUS: "); yield();
  if (callConnected) {
    if (ppp) {
      Serial.print("CONNECTED TO PPP"); yield();
    } else {
      Serial.print("CONNECTED TO "); Serial.println(ipToString(tcpClient.remoteIP())); yield();
    }
    Serial.print("CALL LENGTH: "); Serial.println(connectTimeString()); yield();
  } else {
    Serial.println("NOT CONNECTED");
  }
}

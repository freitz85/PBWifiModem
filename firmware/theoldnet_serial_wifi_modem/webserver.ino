#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

ESP8266HTTPUpdateServer httpUpdater;
ESP8266WebServer webServer(80);

void handleWebServer(){
  // Service the Web server
  webServer.handleClient();
}

void webserverSetup(){
  webServer.on("/", handleRoot);
  webServer.on("/ath", handleWebHangUp);
  webServer.begin();
}

void webserverHangup(String t){
  webServer.send(200, "text/plain", t);  
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
  page.concat("<br>SSID.......: " + WiFi.SSID());

  byte mac[6];
  WiFi.macAddress(mac);
  page.concat("<br>MAC ADDRESS: ");
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

  page.concat("<br>IP ADDRESS.: "); page.concat(ipToString(WiFi.localIP()));
  page.concat("<br>GATEWAY....: "); page.concat(ipToString(WiFi.gatewayIP()));
  yield();

  page.concat("<br>SUBNET MASK: "); page.concat(ipToString(WiFi.subnetMask()));
  yield();
  page.concat("<br>SERVER PORT: "); page.concat(tcpServerPort);
  page.concat("<br>CALL STATUS: ");
  if (callConnected) {
    page.concat("CONNECTED TO ");
    if (ppp) {
      page.concat("PPP");
    } else {
      page.concat(ipToString(tcpClient.remoteIP()));
    }
    page.concat("<br>CALL LENGTH: "); page.concat(connectTimeString()); yield();
  } else {
    page.concat("NOT CONNECTED");
  }
  page.concat("<br>");
  page.concat("<input type=\"file\">");
  webServer.send(200, "text/html", page);
  delay(100);
}

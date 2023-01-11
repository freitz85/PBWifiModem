#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

ESP8266HTTPUpdateServer httpUpdater;
ESP8266WebServer webServer(80);

#include "webserver.h" //Our HTML webpage contents

void handleWebServer(){
  // Service the Web server
  webServer.handleClient();
}

void webserverSetup(){
  webServer.on("/old", handleOldRoot);
  webServer.on("/", handleRoot);
  webServer.on("/ath", handleWebHangUp);
  webServer.on("/get-settings", handleGetSettings);
  webServer.on("/update-settings", handleUpdateSettings);
  webServer.on("/update-firmware", handleUpdateFirmware);
  webServer.on("/update-speeddial", handleUpdateSpeeddial);
  webServer.on("/factory-defaults", handleFactoryDefaults);
  webServer.on("/file-upload", handleFileUpload);
  webServer.begin();
}

void handleUpdateSettings(){}
void handleUpdateFirmware(){}
void handleUpdateSpeeddial(){}
void handleFactoryDefaults(){}
void handleFileUpload(){}

void handleGetSettings(){
  String json = "{ ";
    json += "\"wifiStatus\": \"" + getWifiStatus() + "\",";
    json += "\"ssidStatus\": \"" + WiFi.SSID() + "\",";
    json += "\"macAddress\": \"" + getMacAddress() + "\",";
    json += "\"ipAddress\": \"" + ipToString(WiFi.localIP()) + "\",";
    json += "\"gateway\": \"" + ipToString(WiFi.gatewayIP()) + "\",";
    json += "\"subnet\": \"" + ipToString(WiFi.subnetMask()) + "\",";
    json += "\"serverPort\": \"" + String(tcpServerPort) + "\",";
    json += "\"callStatus\": \"" + getCallStatus() + "\",";
    json += "\"callLength\": \"" + getCallLength() + "\",";
    json += "\"baudStatus\": \"" + String(bauds[serialspeed]) + "\"";
  json += "}";
  webServer.send(200, "application/json", json);
}

String getWifiStatus(){
  if (WiFi.status() == WL_CONNECTED) {
    return "CONNECTED";
  }
  if (WiFi.status() == WL_IDLE_STATUS) {
    return "OFFLINE";
  }
  if (WiFi.status() == WL_CONNECT_FAILED) {
    return "CONNECT FAILED";
  }
  if (WiFi.status() == WL_NO_SSID_AVAIL) {
    return "SSID UNAVAILABLE";
  }
  if (WiFi.status() == WL_CONNECTION_LOST) {
    return "CONNECTION LOST";
  }
  if (WiFi.status() == WL_DISCONNECTED) {
    return "DISCONNECTED";
  }
  if (WiFi.status() == WL_SCAN_COMPLETED) {
    return "SCAN COMPLETED";
  }  
  return "ERROR";
}

String getMacAddress(){
  byte mac[6];
  WiFi.macAddress(mac);
  String macAddress = "";
  macAddress.concat(String(mac[0], HEX));
  macAddress.concat(":");
  macAddress.concat(String(mac[1], HEX));
  macAddress.concat(":");
  macAddress.concat(String(mac[2], HEX));
  macAddress.concat(":");
  macAddress.concat(String(mac[3], HEX));
  macAddress.concat(":");
  macAddress.concat(String(mac[4], HEX));
  macAddress.concat(":");
  macAddress.concat(String(mac[5], HEX));
  return macAddress;  
}

String getCallStatus(){
  String status = "";
  if (callConnected) {
    status.concat("CONNECTED TO ");
    if (ppp) {
      status.concat("PPP");
    } else {
      status.concat(ipToString(tcpClient.remoteIP()));
    }
  //    yield(); //why?
  } else {
    status.concat("NOT CONNECTED");
  }
  return status;
}

String getCallLength(){
  String status = "";
  if (callConnected) {
    status.concat(connectTimeString()); 
  } else {
    status.concat("00:00:00");
  }
  return status;  
}

void handleWebHangUp() {
  String t = "NO CARRIER (" + connectTimeString() + ")";
  hangUp();
  webServer.send(200, "text/plain", t);
}

void handleRoot(){
 String s = MAIN_page; //Read HTML contents
 webServer.send(200, "text/html", s);
 delay(100);
}

void handleOldRoot() {
  String page = "WIFI STATUS: ";
  page.concat(getWifiStatus());
  
  yield();
  page.concat("<br>SSID.......: " + WiFi.SSID());

  page.concat("<br>MAC ADDRESS: ");
  page.concat(getMacAddress());
  yield();

  page.concat("<br>IP ADDRESS.: "); page.concat(ipToString(WiFi.localIP()));
  page.concat("<br>GATEWAY....: "); page.concat(ipToString(WiFi.gatewayIP()));
  yield();

  page.concat("<br>SUBNET MASK: "); page.concat(ipToString(WiFi.subnetMask()));
  yield();
  page.concat("<br>SERVER PORT: "); page.concat(tcpServerPort);
  page.concat("<br>CALL STATUS: ");
  page.concat(getCallStatus());
  page.concat("<br>");
  webServer.send(200, "text/html", page);
  delay(100);
}

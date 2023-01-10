void setBaudRate(int inSpeed) {
  if (inSpeed == 0) {
    sendResult(R_ERROR);
    return;
  }
  int foundBaud = -1;
  for (int i = 0; i < sizeof(bauds)/sizeof(bauds[0]); i++) {
    if (inSpeed == bauds[i]) {
      foundBaud = i;
      break;
    }
  }
  // requested baud rate not found, return error
  if (foundBaud == -1) {
    sendResult(R_ERROR);
    return;
  }
  if (foundBaud == serialspeed) {
    sendResult(R_OK);
    return;
  }
  Serial.print("SWITCHING SERIAL PORT TO ");
  Serial.print(inSpeed);
  Serial.println(" IN 5 SECONDS");
  delay(5000);
  Serial.end();
  delay(200);
  Serial.begin(bauds[foundBaud]);
  serialspeed = foundBaud;
  delay(200);
  sendResult(R_OK);
}

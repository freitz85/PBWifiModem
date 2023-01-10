/**
   Turn on the LED and store the time, so the LED will be shortly after turned off
*/
void led_on()
{
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  ledTime = millis();
}

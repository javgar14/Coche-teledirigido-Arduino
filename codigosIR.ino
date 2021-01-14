#include <IRremote.h>

IRrecv receptor(SENSOR_IR);
decode_results codigo_boton;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  receptor.enableIRIn();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (receptor.decode(&codigo_boton)) {
    Serial.print("0x");
    Serial.println(codigo_boton.value, HEX);
    receptor.resume();
  }
  delay(100);
}

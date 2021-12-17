#include "G2x.h"

LOGITECH_G2x G27;
void setup() {
  Serial.begin(115200);
  G27.begin(&Serial1);
}

void loop() {

  uint8_t w,t,b,c;

  G27.getWheel(&w);
  G27.getThrottle(&t);
  G27.getBrake(&b);
  G27.getClutch(&c);
  Serial.print("W: ");
  Serial.print(w);
  Serial.print(" T: ");
  Serial.print(t);
  Serial.print(" B: ");
  Serial.print(b);
  Serial.print(" C: ");
  Serial.println(c);
  for(int i= 0; i < 23; i++)
  {
    bool val;
    G27.getButton(i, &val);
    Serial.print(val);
    Serial.print(" ");
  }
  String hat;
  G27.getHat(0, &hat);
  Serial.println(hat);
  Serial.println();
  Serial.println("--------------------");
  delay(100);
}

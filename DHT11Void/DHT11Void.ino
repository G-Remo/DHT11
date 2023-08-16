#include "dht11.h"

void setup() {
  Serial.begin(115200);
  init_dht11(14);
}

void loop() {
  dht11();
}

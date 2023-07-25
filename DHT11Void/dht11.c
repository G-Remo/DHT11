const uint8_t pin_dht11 = 14;

uint32_t time_dht11;
uint8_t Humidite; 
float Temperature;

void init_dht11() {
  Serial.begin(115200);
  pinMode( pin_dht11, OUTPUT); 
  digitalWrite( pin_dht11, HIGH);
  time_dht11 = millis();
}

bool dht11( bool fahrenheit = false, bool serial_print = true){
  if( millis() - time_dht11 < 2000) return false;
  time_dht11 = millis();

  float temp_Temperature;
  uint32_t DataTime;
  uint8_t DataArray;
  uint8_t DHTData[4];
  bool ResultB[45];
  bool BlockDHT;

  digitalWrite(pin_dht11,LOW);
  delay(18);
  digitalWrite(pin_dht11,HIGH);
  delayMicroseconds(20);
  pinMode(pin_dht11,INPUT); 
  
  do {
    if( !digitalRead(pin_dht11) && !BlockDHT ) {  //If DHT pin is low, go to next Dataset
      BlockDHT = true;
      if( ( micros() - DataTime) <= 90 && DataArray > 1) ResultB[DataArray-2] = false;
      else ResultB[DataArray-2] = true;
      DataArray++;
      DataTime = micros();
    }
    if( digitalRead(pin_dht11)) BlockDHT = false; // As long as DHT pin is Hight add time in Microseconds to Result
    
  
  } while( (micros() - DataTime) < 150); // if DTH Sensor high for more than 150 usec, leave loop
  
  for (int  j=0; j< 5; j++){     // redo it for the 5 uint8_ts (40 Databits /8 = 5)
    for (int  i=0; i< 8; i++){  // Create 5 Data uint8_ts from the 40 Databits (Ignoring the 2 first Databits)
      bitWrite(DHTData[j], 7-i, ResultB[i+(j*8)]);
    }
  }

  pinMode(pin_dht11,OUTPUT); 
  digitalWrite(pin_dht11,HIGH);

  if (DHTData[4] == (DHTData[0]+DHTData[1]+DHTData[2]+DHTData[3])){
    Humidite = DHTData[0];
    temp_Temperature = DHTData[2];
    temp_Temperature += DHTData[3] / 10.0;
    if( fahrenheit) temp_Temperature = ( temp_Temperature * 9.0 / 5) + 32;
    Temperature = temp_Temperature;
    if( serial_print){
      char CHAR[40];
      sprintf( CHAR, "Humidité: %u%%, Température: %.1f°%s",Humidite, Temperature, fahrenheit ? "F" : "C");
      Serial.println(CHAR);
    }
    return true;
  }

  if( serial_print) Serial.println( "Erreur de lecture du DHT11");
  return false;
}

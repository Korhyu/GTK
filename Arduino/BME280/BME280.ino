/***************************************************************************
  Programa de prueba de las funciones para el manejo del sensor BME280
 ***************************************************************************/




void setup() {
  Serial.begin(115200);

  //Inicializo el BME
  //test_bme();
}

void loop() {

  
  Serial.print(F("Temperature = "));
  Serial.print(bme_temp());
  Serial.println(" *C");
  
  Serial.print(F("Pressure = "));
  Serial.print(bme_pres());
  Serial.println(" hPa");

  Serial.print(F("Humidity = "));
  Serial.print(bme_hume());
  Serial.println(" %");


  Serial.println();
  delay(3000);
}

/***************************************************************************
  Programa de prueba de las funciones para el manejo del sensor BME280
 ***************************************************************************/


/* Librerias del BME280 */
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BME280.h>


/* Variables del BME280 */
Adafruit_BME280 bme; // use I2C interface
Adafruit_Sensor *bme_temp = bme.getTemperatureSensor();
Adafruit_Sensor *bme_pressure = bme.getPressureSensor();
Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();

void setup() {
  Serial.begin(115200);

  //Inicializo el BME
  //test_bme();
  bmeInit();

}

void loop() {

  float valores[3];

  valores[0] = bmeTemp();
  valores[1] = bmePres();
  valores[2] = bmeHumi();
  
  Serial.print(F("Temperature = "));
  Serial.print(valores[0]);
  Serial.println(" *C");
  
  Serial.print(F("Pressure = "));
  Serial.print(valores[1]);
  Serial.println(" hPa");

  Serial.print(F("Humidity = "));
  Serial.print(valores[2]);
  Serial.println(" %");


  Serial.println();
  delay(3000);
}

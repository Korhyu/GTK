/***************************************************************************
 * Funciones para el uso del sensor BME 280
 * 
 * Para arduino NODEMCU
 * BME -> NODEMCU
 * Vcc -> 3.3V
 * GND -> GND
 * SCL -> GPIO5 - D1
 * SDA -> GPIO4 - D2
 * CSB -> NC
 * SD0 -> Vcc (este le da el address al I2C)
 */


#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BME280.h>




bool test_bme(){
  Serial.println(F("BME280 Sensor event test"));

  Adafruit_BME280 bme; // use I2C interface
  Adafruit_Sensor *bme_temp = bme.getTemperatureSensor();
  Adafruit_Sensor *bme_pressure = bme.getPressureSensor();
  Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();

  if (!bme.begin()) {
    //Sensor no detectado    
    Serial.println(F("Could not find a valid BME280 sensor, check wiring!"));
  }
  else
  {
    //Sensor detectado, imprimo las caracteristicas
    bme_temp->printSensorDetails();
    bme_pressure->printSensorDetails();
    bme_humidity->printSensorDetails();
  }
}



float* bme_medicion (){
  //Esta funcion no esta funcionando, cuando devuelve el puntero con los elementos
  //algo raro pasa porque no puedo acceder bien a los otros 2 elementos, solo al primero

  float mediciones[3];

  Adafruit_BME280 bme; // use I2C interface
  Adafruit_Sensor *bme_temp = bme.getTemperatureSensor();
  Adafruit_Sensor *bme_pressure = bme.getPressureSensor();
  Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();

  bme.begin();

  sensors_event_t temp_event, pressure_event, humidity_event;
  bme_temp->getEvent(&temp_event);
  bme_pressure->getEvent(&pressure_event);
  bme_humidity->getEvent(&humidity_event);

  String aux;

  mediciones[0] = temp_event.temperature;
  aux = pressure_event.pressure;
  //mediciones[1] = aux.toFloat();
  mediciones[1] = 300;
  aux = humidity_event.relative_humidity;
  //mediciones[2] = aux.toFloat();
  mediciones[2] = 4;


  return mediciones;
}

float bme_temp (){
  float temp;
  Adafruit_BME280 bme; // use I2C interface
  Adafruit_Sensor *bme_temp = bme.getTemperatureSensor();

  bme.begin();

  sensors_event_t temp_event;
  bme_temp->getEvent(&temp_event);

  temp = temp_event.temperature;

  return temp;
}

float bme_pres (){
  float pres;
  Adafruit_BME280 bme; // use I2C interface
  Adafruit_Sensor *bme_pressure = bme.getPressureSensor();

  bme.begin();

  sensors_event_t pressure_event;
  bme_pressure->getEvent(&pressure_event);

  pres = pressure_event.pressure;

  return pres;
}

float bme_hume (){
  float hume;
  Adafruit_BME280 bme; // use I2C interface
  Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();

  bme.begin();

  sensors_event_t humidity_event;
  bme_humidity->getEvent(&humidity_event);

  hume = humidity_event.pressure;

  return hume;
}

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


bool bmeInit(){
    if (!bme.begin()) {
    //Sensor no detectado    
    return false;
  }
  else
  {
    
    return true;    
  }
}

bool bmeMedicion (float* vector){

  sensors_event_t temp_event;
  sensors_event_t pressure_event;
  sensors_event_t humidity_event;
  
  bme_temp->getEvent(&temp_event);
  bme_pressure->getEvent(&pressure_event);
  bme_humidity->getEvent(&humidity_event);
  
  vector[0] = temp_event.temperature;
  vector[1] = pressure_event.pressure;
  vector[2] = humidity_event.pressure;

  return true;
}


float bmeTemp (){
  float temp;
  sensors_event_t temp_event;
  bme_temp->getEvent(&temp_event);
  temp = temp_event.temperature;
  Serial.println(temp);
  return temp;
}

float bmePres (){
  float pres;
  sensors_event_t pressure_event;
  bme_pressure->getEvent(&pressure_event);
  pres = pressure_event.pressure;
  Serial.println(pres);
  return pres;
}

float bmeHumi (){
  float hume;
  sensors_event_t humidity_event;
  bme_humidity->getEvent(&humidity_event);
  hume = humidity_event.pressure;
  Serial.println(hume);
  return hume;
}

#include <ESP8266WiFi.h>
#include "EspMQTTClient.h"




/* Info WIFI -------------------------------------------------------- */
const char* ssid = "Mora";
const char* password = "moramora";


/* Datos del MQTT y WIFI -------------------------------------------- */
String NDISP = "2";

EspMQTTClient client(
  "Mora",
  "moramora",
  "192.168.0.27",   // MQTT Broker server ip
  "MQTTUsername",   // Can be omitted if not needed
  "MQTTPassword",   // Can be omitted if not needed
  "2"               // Client name that uniquely identify your device
);


/* Tiempos  --------------------------------------------------------- */
const int TON = 50;             //Tiempo que se mantiene encendido el LED
const int TOFF = 5000;          //Tiempo que se mantiene apagado el LED
const int TREPORTE = 300000;    //Tiempo entre envio de datos
const int TSENSADO = 30000;     //Tiempo entre toma de datos

const int MAXVECTOR = TREPORTE/TSENSADO;       //Elementos de los vectores de datos

/* Pines ------------------------------------------------------------ */
const int LED1 = 16;
const int OUT1 = 10;
const int OUT2 = 9;
uint8_t DHTPin = 2;



/*  ------------------------------- */


/* Vectores de mediciones -------------------------------------------- */
float Temperature[MAXVECTOR];
float Humidity[MAXVECTOR];
float Pressure[MAXVECTOR];

/* Variables auxiliares --------------------------------------------- */
long tledAct = 0;
long tledAnt = 0;
long tregistroAnt = 0;
long tregistroAct = 0;
long tsensorAnt = 0;
long tsensorAct = 0;
int indice = 0;
bool estado1 = false;
float estadoDisp = 0;



/* Librerias del BMP280  ---------------------------------------------- */
#include <Wire.h>
#include <SPI.h>
#include "Adafruit_BMP280.h"

Adafruit_BMP280 bmp;


/* Libreria y definiciones del DHT11 ----------------------------------- */
#include "DHT.h"

#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPin, DHTTYPE);     //Sensor de temperatura y humedad



/* Setup ------------------------------------------------------- */
void setup() {
  Serial.begin(115200);

  //Inicio el BME
  if (bmp.begin() == false)
  {
    estadoDisp = 0.4;       //Si no puedo comunicarme con el sensor indico que no esta operativo
    Serial.println(F("Could not find a valid BME280 sensor, check wiring!"));
  }
  else
  {
    estadoDisp = 1;         //Comuncacion existosa con el sensor
    Serial.println(F("Sensor BME280 encontrado y funcionando"));

    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,   /* Modo de operación */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Presion oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtrado. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Tiempo Standby. */
  }
  
  Serial.println("Conectando ");
  Serial.println(ssid);

  
  WiFi.begin(ssid, password);   // Intentamos la conexion a la WIFI
  while (WiFi.status() != WL_CONNECTED)
  {  
    delay(1000);
    Serial.print(".");      // Escribiendo puntitos hasta que conecte
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("IP: "); 
  Serial.println(WiFi.localIP());   // Imprimir nuestra IP al conectar


  digitalWrite(LED1, HIGH);
  digitalWrite(OUT1, HIGH);
  digitalWrite(OUT2, HIGH);
  
  pinMode(LED1, OUTPUT);
  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);


  postLastWill();
}



/* Loop ------------------------------------------------------- */
void loop()
{
  //Serial.println("Loop");
  client.loop();
  
  tledAct = millis();
  tsensorAct = tledAct;
  tregistroAct= tledAct;


  if(tledAct >= tledAnt+TOFF and estado1 == false)
  {
    digitalWrite(LED1, LOW);
    estado1 = true;  
  }
  if(tledAct >= tledAnt+TOFF+TON and estado1 == true)
  {
    digitalWrite(LED1, HIGH);
    estado1 = false;
    tledAnt = tledAct;
  }

  if(tregistroAct >= tregistroAnt + TSENSADO)
  {   
    tregistroAnt = tregistroAct;

    //Mediciones
    Temperature[indice] = bmp.readTemperature();        // Leemos la temperatura
    Pressure[indice] = bmp.readPressure()/100;          // y presion del BMP

    Humidity[indice] = dht.readHumidity();              // Leemos la humedad del DHT11
    

    indice++;
  }

  if(indice == round(TREPORTE/TSENSADO))
  {
    tsensorAnt = tsensorAct;

    String temperatura = "medicion/" + NDISP + "/1/";
    String humedad = "medicion/" + NDISP + "/2/";
    String presion = "medicion/" + NDISP + "/3/";
    char auxiliarString[100];

    temperatura.toCharArray(auxiliarString, 100);    
    client.publish(auxiliarString, String(promedio(Temperature, round(TREPORTE/TSENSADO))));
    humedad.toCharArray(auxiliarString, 100);
    client.publish(auxiliarString, String(promedio(Humidity, round(TREPORTE/TSENSADO))));
    presion.toCharArray(auxiliarString, 100);
    client.publish(auxiliarString, String(promedio(Pressure, round(TREPORTE/TSENSADO))));
  }

  //Serial.println(bmeTemp());
}

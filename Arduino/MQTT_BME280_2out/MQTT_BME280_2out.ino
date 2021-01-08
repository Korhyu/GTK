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
const int TON = 100;            //Tiempo que se mantiene encendido el LED
const int TOFF = 5000;          //Tiempo que se mantiene apagado el LED
const int TREPORTE = 300000;    //Tiempo entre envio de datos
const int TSENSADO = 60000;     //Tiempo entre toma de datos

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



/* Librerias del BME280  ---------------------------------------------- */
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BME280.h>


/* Variables del BME280  ---------------------------------------------- */
Adafruit_BME280 bme; // use I2C interface
Adafruit_Sensor *bme_temp = bme.getTemperatureSensor();
Adafruit_Sensor *bme_pressure = bme.getPressureSensor();
Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();








/* Setup ------------------------------------------------------- */
void setup() {
  Serial.begin(115200);
   
  
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
  Serial.print("Nuestra IP: "); 
  Serial.println(WiFi.localIP());   // Imprimir nuestra IP al conectar


  digitalWrite(LED1, HIGH);
  digitalWrite(OUT1, HIGH);
  digitalWrite(OUT2, HIGH);
  
  pinMode(LED1, OUTPUT);
  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);


  // Lastwill message, aviso que que el estado es "0"
  String estado = "estado/" + NDISP + "/";
  char auxiliarString[100];
  estado.toCharArray(auxiliarString, 100); 
  client.enableLastWillMessage(auxiliarString, "0", true);
  client.enableMQTTPersistence();
  Serial.println("Last will message sent"); 

  //Inicio el BME
  bmeInit();
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
    Temperature[indice] = bmeTemp();         // Leemos la temperatura
    Humidity[indice] = bmeHumi();            // Leemos la humedad
    Pressure[indice] = bmePres();

    indice++;
  }

  if(tsensorAct >= tsensorAnt + TREPORTE)
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
}

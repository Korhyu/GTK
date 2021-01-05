#include <ESP8266WiFi.h>
#include "EspMQTTClient.h"
#include "DHT.h"


#define DHTTYPE DHT11 // DHT 11

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


/* Objetos de los sensores ------------------------------------------ */
DHT dht(DHTPin, DHTTYPE);     //Sensor de temperatura y humedad



/*  ------------------------------- */


/* Vectores de mediciones -------------------------------------------- */
float Temperature[MAXVECTOR];
float Humidity[MAXVECTOR];

/* Variables auxiliares --------------------------------------------- */
long tledAct = 0;
long tledAnt = 0;
long tregistroAnt = 0;
long tregistroAct = 0;
long tsensorAnt = 0;
long tsensorAct = 0;
int indice = 0;
bool estado1 = false;


/* Funciones Auxiliares ---------------------------------------------- */
void accSalida(const String& message) {

  //Pasaje de Mensaje a Campos
  char auxiliarString[100];
  message.toCharArray(auxiliarString, 100); 
  String campos[6];
  
  for(int aux=0 ; aux < 6 ; aux++)
  {
    if (aux == 0)
      campos[aux] = strtok(auxiliarString, "/");
    else
      campos[aux] = strtok(NULL, "/");
    //Serial.println(campos[aux]);
  }
  
  //Salidas digitales
  if (campos[0] == "dig")
  {
    //Salida 1
    if (campos[1] == "1")
    {
      if (campos[2] == "ON")
        digitalWrite(OUT1, LOW);
      else
        digitalWrite(OUT1, HIGH);
    }
    //Salida 2
    if (campos[1] == "2")
    {
      if (campos[2] == "ON")
        digitalWrite(OUT2, LOW);
      else
        digitalWrite(OUT2, HIGH);
    }
  }
}





void onMessageReceived(const String& topic, const String& message) {
  Serial.println(topic + ": " + message);
}

float promedio (float* vector, int cantidad)
{
  int i;
  float suma=0;


  for (i=0 ; i<cantidad ; i++)
  {
    suma += vector[i];
    Serial.print(vector[i]);
    Serial.print("\t");
  }
  indice = 0;

  Serial.print("\nPromedio: ");
  Serial.println(suma/cantidad);
  
  return (suma/cantidad);
}

void onConnectionEstablished() {
  client.subscribe("RX/test", [] (const String &payload)
  {
    Serial.println(payload);
  });

  String acciones;


  // Subscripcion a topicos de interes
  char auxiliarString[100];
  acciones = "accion/" + NDISP + "/#";
  acciones.toCharArray(auxiliarString, 100); 
  client.subscribe(auxiliarString, accSalida);

  
  
  //Aviso que el dispositivo esta activo
  String estado = "estado/" + NDISP + "/";
  estado.toCharArray(auxiliarString, 100); 
  client.publish(auxiliarString, "1");
  Serial.println("Estoy vivo!!");
  
}




/* Setup ------------------------------------------------------- */
void setup() {
  Serial.begin(115200);
  pinMode(DHTPin, INPUT);   
  
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

  
  dht.begin();                  //Iniciamos el sensor 

  // Lastwill message, aviso que que el estado es "0"
  String estado = "estado/" + NDISP + "/";
  char auxiliarString[100];
  estado.toCharArray(auxiliarString, 100); 
  client.enableLastWillMessage(auxiliarString, "0", true);
  client.enableMQTTPersistence();
  Serial.println("Last will message sent"); 
  

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

    //Medicion de temperatura y humedad
    Temperature[indice] = dht.readTemperature();         // Leemos la temperatura
    Humidity[indice] = dht.readHumidity();               // Leemos la humedad

    //Serial.println(dht.readTemperature());              //Test de que funciona

    indice++;
  }

  if(tsensorAct >= tsensorAnt + TREPORTE)
  {
    tsensorAnt = tsensorAct;

    String temperatura = "medicion/" + NDISP + "/1/";
    String humedad = "medicion/" + NDISP + "/2/";
    char auxiliarString[100];
  

    temperatura.toCharArray(auxiliarString, 100);    
    client.publish(auxiliarString, String(promedio(Temperature, round(TREPORTE/TSENSADO))));
    humedad.toCharArray(auxiliarString, 100);
    client.publish(auxiliarString, String(promedio(Humidity, round(TREPORTE/TSENSADO))));
  }
}

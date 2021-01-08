/* Funciones MQTT ---------------------------------------------- */

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

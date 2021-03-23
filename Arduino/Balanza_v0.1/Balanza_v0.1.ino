/*
 * Programa para dispositivo para comunicarse con la balanza
 * SISTEL Clipse Croma por UART
 */

// Pines -------------------------------------------------------



// Codigos -----------------------------------------------------

#define ENQ 05          //Interrogacion de peso
#define ACK 06          //Recepcion correcta del peso
#define STX 02          //Comienzo de cadena de datos
#define ETX 03          //Fin de cadena de datos
#define NACK 15         //Recepcion incorrecta del peso
#define WACK 11         //Peticion de espera de la balanza

/*
 * Protocolo de comunicacion
 * STX + PESO + ETX + CRC
 * CRC = STX XOR DATO1 ... XOR DATOn XOR ETX
 * 
 * XOR: OR exclusiva
 */


void setup() {
  // Datos de la UART y RS-232
  int baudRate = 9600;
  int bitData = 8;
  int bitStop = 1;

}

void loop() {
  // put your main code here, to run repeatedly:

}

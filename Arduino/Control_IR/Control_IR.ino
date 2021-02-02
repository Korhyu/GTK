int incomingByte = 0;

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);


  Serial.println("Estoy vivo");
}

void loop() {
  if (Serial1.available() > 0) {
    incomingByte = Serial1.read();

  
    Serial.print("I received: ");
    Serial.println(incomingByte, HEX);
    }
}

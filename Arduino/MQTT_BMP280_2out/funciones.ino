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

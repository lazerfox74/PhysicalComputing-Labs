// the setup routine runs once when you press reset:
//previous value
float y0;
//interpolation amount
//0.5 = best
float interpamt = 0.5;

//interpolation algorithm
float onePole(float y1)
{
  return (y0 = interpamt * (y1 - y0));
}


void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  pinMode(11, OUTPUT);

}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = analogRead(A0);
  // print out the value you read:
  float value = onePole(voltage);
  Serial.println(value);
  Serial.print(",");



  // analogWrite(11, value/4.2);  // analogWrite values from 0 to 255


  // delay(1);

}

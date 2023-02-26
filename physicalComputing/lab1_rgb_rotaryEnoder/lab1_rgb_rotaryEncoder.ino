/* Rotary Encoder Setup Refernce taken from  Oleg Mazurov's code for rotary encoder interrupt service routines for AVR micros
    https://chome.nerpa.tech/mcu/reading-rotary-encoder-on-arduino/

*/
/* Neopixel Default Example sketch used as reference
https://github.com/adafruit/Adafruit_NeoPixel
*/
//buton setup
const int BUTTON_PIN = 7; // the number of the pushbutton pin

// Rotary Encoder Variables:
int lastState = HIGH; //  previous state from the input pin
int currentState;    //  current reading from the input pin
//colour state for the rgb leds
char colour = 0;

//rotary setup
// Define rotary encoder pins
#define ENC_A 2
#define ENC_B 3

unsigned long _lastIncReadTime = micros(); 
unsigned long _lastDecReadTime = micros(); 
int _pauseLength = 25000;
int _fastIncrement = 10;

volatile int counter = 0;

//neopixel
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
//neopixel pin
#define PIN        6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 12 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  //buton setup
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Set encoder pins and attach interrupts
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC_A), read_encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B), read_encoder, CHANGE);

  // Start the serial monitor to show output
  Serial.begin(9600);

  //neopixel
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

}

void loop() {
  static int lastCounter = 0;

  // If count has changed print the new value to serial
  if(counter != lastCounter){
    Serial.println(counter);
    lastCounter = counter;
  }

    pixels.clear(); // Set all pixel colors to 'off'

//detection for which colour the ledsshould be set too
    if(colour == 0)
    { 
          pixels.setPixelColor(counter, pixels.Color(0, 150, 0));

    }
    else if(colour == 1)
    {
          pixels.setPixelColor(counter, pixels.Color(150, 0, 0));
    }
    else
    {
          pixels.setPixelColor(counter, pixels.Color(0, 0, 150));

    }


    pixels.show();   // Send the updated pixel colors to the hardware.

    //buton read
  
    currentState = digitalRead(BUTTON_PIN);

  if(lastState == LOW && currentState == HIGH)
  {

    if(colour == 2)
    {
      colour = 0;
    }
    else
    {     colour ++;

    }
    Serial.println(colour);

  }
    

  // save the last state
  lastState = currentState;
  counter = counter%12;
  currentState = currentState%12;



  delay(20);


}

void read_encoder() {
 
  static uint8_t old_AB = 3;  // Lookup table index
  static int8_t encval = 0;   // Encoder value  
  static const int8_t enc_states[]  = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0}; // Lookup table

  old_AB <<=2;  // Remember previous state

  if (digitalRead(ENC_A)) old_AB |= 0x02; // Add current state of pin A
  if (digitalRead(ENC_B)) old_AB |= 0x01; // Add current state of pin B
  
  encval += enc_states[( old_AB & 0x0f )];

  // Update counter if encoder has rotated a full indent, that is at least 4 steps
  if( encval > 3 ) {        // Four steps forward
    int changevalue = -1;
    if((micros() - _lastIncReadTime) < _pauseLength) {
      changevalue = _fastIncrement * changevalue; 
    }
    _lastIncReadTime = micros();
    counter = counter + changevalue;              // Update counter
    encval = 0;
  }
  else if( encval < -3 ) {        // Four steps backward
    int changevalue = 1;
    if((micros() - _lastDecReadTime) < _pauseLength) {
      changevalue = _fastIncrement * changevalue; 
    }
    _lastDecReadTime = micros();
    counter = counter + changevalue;              // Update counter
    encval = 0;
  }

} 
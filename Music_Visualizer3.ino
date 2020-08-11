#define LIN_OUT 1 // turns linear function on of fht
#define FHT_N 64 // set to 64 point fht
#include <FHT.h> // include the library
#include <FastLED.h>

#define NUM_STRIPS 16 //define the number of strips here
#define NUM_LEDS_PER_STRIP 16 // define the number of LEDs per strip here
CRGB leds[NUM_STRIPS][NUM_LEDS_PER_STRIP];


float gain = 600.0;// changes how much the magnitude of the frequency will show up on the display

void setup() {
    Serial.begin(115200); // use the serial port
    ADCSRA = 0xe5; // set the adc to free running mode
    ADMUX = 0x40; // use adc0
    DIDR0 = 0x01; // turn off the digital input for adc0
    
    // Sets up LEDs in an array 
    FastLED.addLeds<NEOPIXEL, 23>(leds[0], NUM_LEDS_PER_STRIP);//<type of LED, pin of dataout>(LEDstrip #,# of LEDs)
    FastLED.addLeds<NEOPIXEL, 25>(leds[1], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<NEOPIXEL, 27>(leds[2], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<NEOPIXEL, 29>(leds[3], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<NEOPIXEL, 31>(leds[4], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<NEOPIXEL, 33>(leds[5], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<NEOPIXEL, 35>(leds[6], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<NEOPIXEL, 37>(leds[8], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<NEOPIXEL, 39>(leds[9], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<NEOPIXEL, 41>(leds[10], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<NEOPIXEL, 43>(leds[11], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<NEOPIXEL, 45>(leds[12], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<NEOPIXEL, 47>(leds[7], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<NEOPIXEL, 49>(leds[13], NUM_LEDS_PER_STRIP);  
    FastLED.addLeds<NEOPIXEL, 51>(leds[14], NUM_LEDS_PER_STRIP);
    FastLED.addLeds<NEOPIXEL, 53>(leds[15], NUM_LEDS_PER_STRIP);
    
    pinMode(A1,OUTPUT);//Something for my specific mic
    digitalWrite(A1,1);//Something for my specific mic
   }
void loop() {
      while(1){// reduces jitter
       cli();  // UDRE interrupt slows this way down on arduino1.0
       for (int i = 0 ; i < FHT_N ; i++) { // save 64 samples
         while(!(ADCSRA & 0x10)); // wait for adc to be ready
         ADCSRA = 0xf6; // restart adc with a prescaler of 64 
         byte m = ADCL; // fetch adc data
         byte j = ADCH;
         int k = (j << 8) | m; // form into an int
         k -= 0x0200; // form into a signed int
         k <<= 6; // form into a 16b signed int
         fht_input[i] = k; // put real data into bins
       }
      fht_reorder(); // reorder the data before doing the fht
      fht_run(); // process the data in the fht
      fht_mag_lin(); // take the output of the fht
      sei();
      
      // prints off fht values to the serial monitor 
      for( int i = 0; i < (FHT_N/2) ; i++){
        Serial.print(i);
        Serial.print(" ");
        Serial.println(fht_lin_out[i]);
      }
       
      for( int i = 0; i < NUM_STRIPS; i++){
            int x = (((float)fht_lin_out[i+1]+(float)fht_lin_out[i+2])/2)*(16.0/gain);// maps 16-bit value to an integer between 0-16ish
            if(x > 16) x = 16; // makes sure x isn't greater than 16;
            // only needed for debugging
//            Serial.print(i);
//            Serial.print(" ");
//            Serial.println(x); 
            for(int m = 0; m < x; m++){
              fireLEDs(m,i);// sets LED to certain color defined by the function 
            }
          } 
       
       FastLED.show();
     

         for( int l = 0; l < NUM_STRIPS; l++){
              for(int m = 0; m < NUM_LEDS_PER_STRIP; m++){
                leds[l][m] = CRGB(0,0,0);
              }
         }
      }
}

void gbpLEDs(int data, int strip){
  
  int green = data;
  int blue = (float)data*0.6;
  int purple = (float)data*0.3;

  leds[strip][green] = CRGB(0,220,0);
  leds[strip][blue] = CRGB::Blue;
  leds[strip][purple] = CRGB::Purple;

}
  
void rainbowLEDs(int data, int strip){
  int red = data;
  int yellow = (float)data*0.8;
  int green = (float)data*0.6;
  int blue = (float)data*0.4;
  int purple = (float)data*0.2;
  leds[strip][red] = CRGB::Red;
  leds[strip][yellow] = CRGB:: Yellow;
  leds[strip][green] = CRGB(0,220,0);
  leds[strip][blue] = CRGB::Blue;
  leds[strip][purple] = CRGB::Purple;

}
void fireLEDs(int data, int strip){
  int red = data;
  int orange= (float)data*0.90;
  int yellow = (float)data*0.30;
  int white = (float)data*0.1;
  
  leds[strip][red] = CRGB(220,30,3);
  leds[strip][orange] = CRGB(136,45,1);
  leds[strip][yellow] = CRGB::DarkOrange;
  leds[strip][white] = CRGB(163,82,37);
}

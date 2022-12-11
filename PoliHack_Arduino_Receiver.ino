//Receiver Arduino Board Code
#include <Servo.h>
#include <Adafruit_NeoPixel.h>

#define RELAY_PIN 7
#define NUM_LEDS 29

/* 0 - relay_pin  1 - servo  2 - led_band  */
char mystring[4];
char link = '1';  //Initialized variable to store receive
int state=0;
Servo RightDoor;
Servo LeftDoor;

/*Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_BAND, NEO_GRB + NEO_KHZ800); */

void setup() {
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);  // Begin the Serial at 9600 Baud
  pinMode(6,INPUT);
  pinMode(5, INPUT);
  pinMode(4, INPUT);  
  RightDoor.attach(9);
  LeftDoor.attach(10);
  RightDoor.write(0);
  LeftDoor.write(90);
 // strip.begin();
  //strip.show(); 
}
void loop() {

  if (digitalRead(6) == 1 && digitalRead(5) == 1 && digitalRead(4) == 1) {
    link = '7';
  } else {
    if (digitalRead(6) == 1 && digitalRead(5) == 1 && digitalRead(4) == 0) {
      link = '6';
    } else {
      if (digitalRead(6) == 1 && digitalRead(5) == 0 && digitalRead(4) == 1) {
        link = '5';
      } else {
        if (digitalRead(6) == 1 && digitalRead(5) == 0 && digitalRead(4) == 0) {
          link = '4';
        } else {
          if (digitalRead(6) == 0 && digitalRead(5) == 1 && digitalRead(4) == 1) {
            link = '3';
          } else {
            if (digitalRead(6) == 0 && digitalRead(5) == 1 && digitalRead(4) == 0) {
              link = '2';
            } else {
              if (digitalRead(6) == 0 && digitalRead(5) == 0 && digitalRead(4) == 1) {
                link = '1';
              } else {
                link = '0';
              }
            }
          }
        }
      }
    }
  }
  Serial.readBytes(mystring, 3);  //Read the serial data
  Serial.println(link);           //Print data on Serial Monitor


  if (mystring[0] == '1') {
    digitalWrite(RELAY_PIN, LOW);
  } else {
    digitalWrite(RELAY_PIN, HIGH);
  }

  if (mystring[1] == '1') {
    if(state==0){
       for (uint8_t i = 0; i <= 90; i++) {
      RightDoor.write(i);
      LeftDoor.write(90 - i);
      delay(10);
      
    }
     state=1;
    }

  } else {
    if(state==1){
      for (uint8_t i = 0; i <= 90; i++) {
      LeftDoor.write(i);
      RightDoor.write(90 - i);
      delay(10);
    }
    state=0;
    }
    
  }

    /*if(mystring[2] == '1')
  {
    uint8_t val = random(0,6);
    
    switch(val){
      case 0 : CylonBounce(0xff, 0, 0, 4, 10, 10);break;
      case 1 : TwinkleRandom(20, 40, false);break;
      case 2 : Sparkle(0xff, 0xff, 0xff, 0);break;
      case 3 : RunningLights(0xff,0xff,0x00, 15);break;
      case 4 : colorWipe(0x00,0xff,0x00, 20);
               colorWipe(0x00,0x00,0x00, 20);break;

      case 5 : rainbowCycle(3);break;
      
      default:break;
    }
  }
  else
  {
    
    setAll(0,0,0);

  } */
    //delay(200);
  }



 /* void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}
void CylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){

  for(int i = 0; i < NUM_LEDS-EyeSize-2; i++) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);

  for(int i = NUM_LEDS-EyeSize-2; i > 0; i--) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }
 
  delay(ReturnDelay);
}

void TwinkleRandom(int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(0,0,0);
 
  for (int i=0; i<Count; i++) {
     setPixel(random(NUM_LEDS),random(0,255),random(0,255),random(0,255));
     showStrip();
     delay(SpeedDelay);
     if(OnlyOne) {
       setAll(0,0,0);
     }
   }
 
  delay(SpeedDelay);
}

void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  int Pixel = random(NUM_LEDS);
  setPixel(Pixel,red,green,blue);
  showStrip();
  delay(SpeedDelay);
  setPixel(Pixel,0,0,0);
}

void RunningLights(byte red, byte green, byte blue, int WaveDelay) {
  int Position=0;
 
  for(int j=0; j<NUM_LEDS*2; j++)
  {
      Position++; // = 0; //Position + Rate;
      for(int i=0; i<NUM_LEDS; i++) {
        // sine wave, 3 offset waves make a rainbow!
        //float level = sin(i+Position) * 127 + 128;
        //setPixel(i,level,0,0);
        //float level = sin(i+Position) * 127 + 128;
        setPixel(i,((sin(i+Position) * 127 + 128)/255)*red,
                   ((sin(i+Position) * 127 + 128)/255)*green,
                   ((sin(i+Position) * 127 + 128)/255)*blue);
      }
     
      showStrip();
      delay(WaveDelay);
  }
}

void colorWipe(byte red, byte green, byte blue, int SpeedDelay) {
  for(uint16_t i=0; i<NUM_LEDS; i++) {
      setPixel(i, red, green, blue);
      showStrip();
      delay(SpeedDelay);
  }
}


void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< NUM_LEDS; i++) {
      c=Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      setPixel(i, *c, *(c+1), *(c+2));
    }
    showStrip();
    delay(SpeedDelay);
  }
}

byte * Wheel(byte WheelPos) {
  static byte c[3];
 
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  return c;
}  */
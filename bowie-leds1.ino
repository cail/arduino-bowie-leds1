// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

/////////////////////////
// Arduino NANO
/////////////////////////

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PINF  2
#define PINB  6
#define PINT  4

#define PINBUTTON  8


#define ANALOG_PIN A1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELSF      9
#define NUMPIXELSB      10
#define NUMPIXELST      10

#define STATES 10
int g_state = 0;

Adafruit_NeoPixel pixels_f = Adafruit_NeoPixel(NUMPIXELSF, PINF, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel pixels_b = Adafruit_NeoPixel(NUMPIXELSB, PINB, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel pixels_t = Adafruit_NeoPixel(NUMPIXELST, PINT, NEO_GRB + NEO_KHZ800);

int delayval = 100;
int tick = 0;

void setup() {
  Serial.begin(9600);
  pixels_f.begin();
  pixels_b.begin();
  pixels_t.begin();

  pinMode(PINBUTTON, INPUT);  
}

int strobe_500ms = 1;

int turns = 0;

int police_lights = 1;

int button_clicked = 0;


void police_loop() {
  
  for(int i=0; i<NUMPIXELSF; i++)
  {
    pixels_f.setPixelColor(i, pixels_f.Color(255,255,255)); // Moderately bright green color.
  }

  uint32_t blinkc = pixels_f.Color(255,128,0);
  if (strobe_500ms) {
     blinkc = 0;
  }
  if (turns) {
    pixels_f.setPixelColor(0, blinkc);
    pixels_f.setPixelColor(1, blinkc);
    pixels_f.setPixelColor(NUMPIXELSF-1, blinkc);
    pixels_f.setPixelColor(NUMPIXELSF-2, blinkc);
  }
  
  pixels_f.show(); // This sends the updated pixel color to the hardware.

  for(int i=0; i<NUMPIXELSB; i++)
  {
    pixels_b.setPixelColor(i, pixels_b.Color(255,0,0));
  }
  
  if (turns) {
    pixels_b.setPixelColor(0, blinkc);
    pixels_b.setPixelColor(1, blinkc);
    pixels_b.setPixelColor(NUMPIXELSB-1, blinkc);
    pixels_b.setPixelColor(NUMPIXELSB-2, blinkc);
  }
  
  pixels_b.show();

  uint32_t topcolor = pixels_t.Color(255,0,0);
  uint32_t topcolor2 = pixels_t.Color(0,0,255);

 // if ((tick / (1000/delayval))  % 3 != 0)
  {
  
    if (tick / (500/delayval) % 2) {
      uint32_t swap = topcolor;
      topcolor = topcolor2;
      topcolor2 = swap;
    }
    
  }
  pixels_t.setPixelColor(0, topcolor);
  pixels_t.setPixelColor(1, topcolor);
  pixels_t.setPixelColor(2, topcolor2);
  pixels_t.setPixelColor(3, topcolor2);

  pixels_t.show();
}

void allwhite_loop()
{
  
}

void update_button()
{
  static int analogReads = 0;
  static uint32_t analog_accum = 0;
  static int button_latch = 0;
  
  analog_accum = analog_accum + analogRead(ANALOG_PIN);
  analogReads = analogReads+1;
  if (analogReads > 10) {
    uint32_t button_voltage = (analog_accum / analogReads) * 5; 
    Serial.print("Voltage: ");
    Serial.println(button_voltage);
    analogReads = 0;
    analog_accum = 0;

    if (button_voltage > 4000 && !button_latch) {
      button_clicked = 1;
      button_latch = 1;
      Serial.println("Button clicked");
    }
    if (button_voltage < 4000 && button_latch) {
      button_latch = 0;
      button_clicked = 0;
    }

    int vv = button_voltage / 100;
    
    for(int i=0; i<NUMPIXELSF; i++)
    {
      if (i < vv)
        pixels_f.setPixelColor(i, pixels_f.Color(255,255,255));
      else
        pixels_f.setPixelColor(i, pixels_f.Color(0,0,0));
    }
    //pixels_f.show();
  }  

  int buttonState = digitalRead(PINBUTTON);
  if (buttonState == HIGH) {
      Serial.println("Button2 clicked");    
  }else
      Serial.println("Button2 clicked LOW");    
  
}

void loop() {

  if (g_state == 0) {
    police_loop();
  } else if (g_state == 1) {
    allwhite_loop();
  } else {
    
  }

  if ((0 == tick % (500/delayval))) {
     strobe_500ms = !strobe_500ms;
  }

  update_button();

//  if (button_clicked)
//    g_state = (g_state+1) % STATES;

  delay(delayval);
  tick++;
}

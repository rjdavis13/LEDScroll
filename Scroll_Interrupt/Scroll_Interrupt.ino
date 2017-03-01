#include <Adafruit_NeoPixel.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define LEDPIN 17
#define MODEPIN 3
#define COLORPIN 10

//*** color constants
/*
const uint32_t bastardAmber = strip.Color(255,206,143);
const uint32_t mediumAmber = strip.Color(254,177,153);
const uint32_t paleAmberGold = strip.Color(254,192,138);
const uint32_t galloGold = strip.Color(254,165,98);
const uint32_t goldenAbmer = strip.Color(254,121,0);
const uint32_t lightRed = strip.Color(176,17,0);
const uint32_t mediumPink = strip.Color(234,139,171);
const uint32_t broadwayPink = strip.Color(224,5,97);
const uint32_t folliesPink = strip.Color(175,77,173);
const uint32_t lightLavender = strip.Color(119,130,199);
const uint32_t specialLavender = strip.Color(147,164,212);
const uint32_t lavender = strip.Color(88,2,163);
const uint32_t indigo = strip.Color(0,38,86);
const uint32_t hemsleyBlue = strip.Color(0,142,208);
const uint32_t tiptonBlue = strip.Color(52,148,209);
const uint32_t lightSteelBlue = strip.Color(1,134,201);
const uint32_t lightSkyBlue = strip.Color(0,145,212);
const uint32_t skyBlue = strip.Color(0,121,192);
const uint32_t brilliantBlue = strip.Color(0,129,184);
const uint32_t lightGreenBlue = strip.Color(0,83,115);
const uint32_t brightBlue = strip.Color(0,97,166);
const uint32_t primaryBlue = strip.Color(1,100,167);
const uint32_t congoBlue = strip.Color(0,40,86);
const uint32_t paleYellowGreen = strip.Color(209,219,182);
const uint32_t mossGreen = strip.Color(42,165,85);
const uint32_t primaryGreen = strip.Color(0,46,35);
const uint32_t doubleCTB = strip.Color(8,107,222);
const uint32_t fullCTB = strip.Color(107,158,231);
const uint32_t halfCTB = strip.Color(165,198,247);
const uint32_t darkBlue = strip.Color(0,0,189);
const uint32_t white = strip.Color(255,255,255);
*/
const uint32_t colorArray[30] = {
strip.Color(255,206,143),
strip.Color(254,177,153),
strip.Color(254,192,138),
strip.Color(254,165,98),
strip.Color(254,121,0),
strip.Color(176,17,0),
strip.Color(234,139,171),
strip.Color(224,5,97),
strip.Color(175,77,173),
strip.Color(119,130,199),
strip.Color(147,164,212),
strip.Color(88,2,163),
strip.Color(0,38,86),
strip.Color(0,142,208),
strip.Color(52,148,209),
strip.Color(1,134,201),
strip.Color(0,145,212),
strip.Color(0,121,192),
strip.Color(0,129,184),
strip.Color(0,83,115),
strip.Color(0,97,166),
strip.Color(1,100,167),
strip.Color(0,40,86),
strip.Color(209,219,182),
strip.Color(42,165,85),
strip.Color(0,46,35),
strip.Color(8,107,222),
strip.Color(107,158,231),
strip.Color(165,198,247),
strip.Color(0,0,189),
strip.Color(255,255,255)};
//*** variables will change
  uint8_t modeIndex = 0;
  
  uint8_t colorIndex = 0;
  uint32_t color1Select = colorArray[0];

 
  /*  int c1seed[] = {255,0,0};
  uint8_t c1count = 0;
*/

  // Debounce stuff (note! Debouncer will fail after 50 days continous operation)
  unsigned long lastDBTime = 0;  // the last time the output pin was toggled
  unsigned long debounceDelay = 75;    // the debounce time
  unsigned long interruptTime = 0;  //time interrupt occures

  
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino LED data pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
  Adafruit_NeoPixel strip = Adafruit_NeoPixel(99, LEDPIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  //* This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
//  #if defined (__AVR_ATtiny85__)
//    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
//  #endif
  // End of trinket special code 


//*** the mode button stuff
  // initialize the mode button pin as an input:
  pinMode(MODEPIN, INPUT_PULLUP);
  // Attach an interrupt to the ISR vector
  attachInterrupt(digitalPinToInterrupt(MODEPIN), Mode_ISR, FALLING);

  // initialize the color button pin as an input:
  pinMode(COLORPIN, INPUT_PULLUP);
  // Attach an interrupt to the ISR vector
  attachInterrupt(digitalPinToInterrupt(COLORPIN), Color_ISR, FALLING);
  

//*** the led strip stuff  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  // Some example procedures showing how to display to the pixels:
  while (modeIndex == 0){
    colorWipe(color1Select, 25); // color1Select
	colorWipe(strip.Color(0, 0, 0), 50); // Clear
  }
  while (modeIndex == 1){
    colorWipe(strip.Color(255,255,255), 25); // Red
	colorWipe(strip.Color(0, 0, 0), 25); // Clear
  }
  while (modeIndex == 2){
    colorWipe(strip.Color(0, 255, 0), 25); // Green
	colorWipe(strip.Color(0, 0, 0), 25); // Clear
  }
  while (modeIndex == 3){
    colorWipe(strip.Color(0, 0, 255), 25); // Blue
	colorWipe(strip.Color(0, 0, 0), 25); // Clear
  }
  while (modeIndex == 4){
    rainbowCycle(20);
  }
  while(modeIndex == 5){
	triColorWipe(strip.Color(255,0,0), strip.Color(255,255,255), strip.Color(0,0,255), 50); // you patriot you
  colorWipe(strip.Color(0, 0, 0), 16); // Clear
  }
  

/*colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
  // Send a theater pixel chase in...
  theaterChase(strip.Color(127, 127, 127), 50); // White
  theaterChase(strip.Color(127, 0, 0), 50); // Red
  theaterChase(strip.Color(0, 0, 127), 50); // Blue

  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);*/
}


//interrupt action for mode button
void Mode_ISR() {
  interruptTime = millis();
  if( (interruptTime - lastDBTime) > debounceDelay){
	lastDBTime = interruptTime;
	modeIndex++;
	  if (modeIndex > 5){
		modeIndex = 0;
	  }
	}
}

//interrupt action for color button
// ! This is probably excessive, just list colors you like and put into an array
//   then cycle through them in this ISR
void Color_ISR() {
  interruptTime = millis();
  if( (interruptTime - lastDBTime) > debounceDelay){
	lastDBTime = interruptTime;
	colorIndex++;
	  if (colorIndex > 30){
		colorIndex = 0;
	color1Select = colorArray[colorIndex];
	/*if(c1seed[0] == 0){
	  c1seed[0] = 128;
	}
	else if(c1seed[0] == 128){
	  c1seed[0]=255;
	}
    else if(c1seed[0] == 255){
	  c1seed[0] = 0;
	  if(c1seed[1] == 0){
		c1seed[1] = 255;
	  }
	  else if(c1seed[1] == 255){
		c1seed[1] = 128;
	  }
	  else if(c1seed[1] == 128){
		c1seed[1] = 0;
		if(c1seed[2] == 0){
		  c1seed[2] = 255;
		}
		else if(c1seed[2] == 255){
		c1seed[2] = 128;
	    }
	    else if(c1seed[2] == 128){
		  c1seed[2] = 0;
	    }
	  }
	}
	if ((c1seed[0] + c1seed[1] + c1seed[2]) <= 128){
	  c1seed[0] = 128;
	}
	if ((c1seed[0] + c1seed[1] + c1seed[2]) < 255){
	  c1seed[0] = 255;
	}*/
  }
}

// Fill the dots one after the other with a color
void triColorWipe(uint32_t c1, uint32_t c2, uint32_t c3, uint8_t wait) {
  for(uint16_t i=0; i+2<strip.numPixels(); i=i+3) {
    strip.setPixelColor(i, c1);
    strip.setPixelColor(i+1, c2);
	strip.setPixelColor(i+2, c3);
	strip.show();
    delay(wait);
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}


void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}


//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

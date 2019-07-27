bool sunriseStarted;
uint32_t startTime;


void preSunrise() { 
  uint8_t r = 3; // min 3
  uint8_t g = 7; // min 7
  uint8_t b = 5; // min 5

  // reset for next time
  sunriseStarted = false;
  CRGB morning = CRGB(r, g, b);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  leds[0] = morning;
};

void drawStaticPalette(CRGBPalette16 palette){
    
    uint8_t i;
    float j;
    for (i = 0, j = 0.0; i <=NUM_LEDS; i++, j++) {
        uint8_t p = (j / NUM_LEDS) * 255;
        leds[i] = ColorFromPalette(palette, p, 255, LINEARBLEND);
    }
    
};


void drawExpandingSunrise(CRGBPalette16 startColour, CRGBPalette16 endColours,
    float width, float interPixelBlend)
{
    
  uint8_t midpoint = NUM_LEDS / 2;  
  

  uint8_t i;
  float j;
  
  
  
  // Fill the leds with the starting colour
  drawStaticPalette(startColour);
  
  // fill the width specified with the new colour, starting from the midpoint. 
    for (i = midpoint - ((uint8_t)width / 2), j = 0.0; i < midpoint + ((uint8_t)width / 2) + 1; i++, j++) {
    uint8_t p = (j / width) * 255;

      CRGB old = leds[i];
      CRGB newCol = ColorFromPalette(endColours, p, 255, LINEARBLEND);

      if (j < 10 || j > width - 10) {
          leds[i] = blend(old, newCol, interPixelBlend);
      } else {
        leds[i] = newCol;
      }
   };
};

void drawContractingSunrise(CRGBPalette16 startColour, CRGBPalette16 endColours,
    uint8_t maxWidth, uint16_t timeToUse, uint16_t actualSpeed)
{
    
  uint8_t midpoint = NUM_LEDS / 2;  
  
  float div = 1.0;
  float f_slowdown = actualSpeed/div;

  uint8_t width = min( timeToUse / f_slowdown, (float) maxWidth);
  float remainder = timeToUse % (uint8_t) f_slowdown;
  uint8_t blendAmt = (remainder / f_slowdown)* 255;
  

  uint8_t i;
  float j;
  
  
  // Fill the leds with the starting colour
  drawStaticPalette(startColour);
  
  // fill the width specified with the new colour, starting from the midpoint. 
      for (i = 0, j = 0.0; i < midpoint - (width / 2); i++, j++) {
        uint8_t p = (j / width) * 255;

      CRGB old = leds[i];
      CRGB newCol = ColorFromPalette(endColours, p, 255, LINEARBLEND);

      leds[i] = newCol;
      
   }; 
   
};




void sunrise(){
    
    
    if (!sunriseStarted)
    {
        startTime = millis();
        sunriseStarted = true;

    } else {
    // setup
        
        // Look at making width a float again and turning the fractional part
        // into the blend amount. 
    
        // speed slider, remapped to 0
        uint16_t speedMax = 100; // min 3 before crash, 50 good.
        float s = ((float)speed / 255.0);
        uint16_t remappedSpeed = max(s * speedMax, (float) 1 );
        
        uint16_t elapsedSecs = (millis() - startTime) / 10;
        
        uint16_t timeToUse = elapsedSecs*remappedSpeed;
        
        float baseWidth = float(elapsedSecs) / (float) (speedMax - remappedSpeed);
        float overallWidth = baseWidth;
        uint8_t intraWidth = ((uint8_t)baseWidth % 1) * 255;
        
        uint8_t width;
        
        
        
        Serial.print("w: ");
        Serial.print(overallWidth);
        

        if (overallWidth < NUM_LEDS) {
            Serial.print(" Phase1 ");
            width = overallWidth;
            drawExpandingSunrise(black_gp, presunrise_gp, width, intraWidth);
            
        } else if (overallWidth < NUM_LEDS*2) {
            Serial.print(" Phase2 ");
            
            width = overallWidth - (float) NUM_LEDS;
            drawExpandingSunrise(presunrise_gp, sunrise_gp, width, intraWidth);
            
        } else {
            width = overallWidth - ((float)NUM_LEDS*2);
            //drawContractingSunrise (sunrise_gp, white_gp, width, intraWidth);
        }
        Serial.println("");
  }

};
  




uint32_t seconds(uint32_t inputMillis) {
  return inputMillis * 1000;
}

uint32_t minutes(uint32_t inputMillis) {
  return inputMillis * 60 * 1000;
}

uint8_t scale(uint8_t value, uint8_t srcMin, uint8_t srcMax, uint8_t dstMin, uint8_t dstMax) {
  return ((value - srcMin) / (srcMax - srcMin)) * (dstMax - dstMin) + dstMin;
}

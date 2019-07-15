
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
    for (i = midpoint - (width / 2), j = 0.0; i < midpoint + (width / 2) + 1; i++, j++) {
    uint8_t p = (j / width) * 255;

      CRGB old = leds[i];
      CRGB newCol = ColorFromPalette(endColours, p, 255, LINEARBLEND);

      if (j < 2 || j > width - 2) {
        leds[i] = blend(old, newCol, blendAmt);
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
        float phase1duration = 60 * 3;
        uint16_t elapsedSecs = (millis() - startTime) / 100;
        uint16_t phase2_secs = elapsedSecs - (uint16_t) phase1duration;
        uint16_t phase3_secs = phase2_secs - (uint16_t)phase1duration*2;
        
        // speed slider, remapped to 0
        uint16_t speedMax = 50; // min 3 before crash, 50 good.
        uint16_t remappedSpeed = min( ((float)speed / 255) * speedMax, (float) speedMax);
        
        // inverted
        uint8_t actualSpeed = speedMax - max(remappedSpeed, (uint16_t) 1) + 1;


        if (elapsedSecs <= phase1duration) {
            drawExpandingSunrise(black_gp, presunrise_gp, NUM_LEDS, elapsedSecs*remappedSpeed, actualSpeed);
        } else if (elapsedSecs <= phase1duration*2 ) {
            drawExpandingSunrise(presunrise_gp, sunrise_gp, NUM_LEDS, phase2_secs*remappedSpeed, actualSpeed);
        } else {
            drawContractingSunrise (sunrise_gp, white_gp, NUM_LEDS, phase2_secs*remappedSpeed, actualSpeed);
        }
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

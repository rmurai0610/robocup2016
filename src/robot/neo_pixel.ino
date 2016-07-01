void neo_pixel_clear(void) {
  for(uint16_t i=0; i<neo_pixel.numPixels(); i++) {
    neo_pixel.setPixelColor(i, 0);
  }
  neo_pixel.show();
}

void flash_all(uint32_t c, uint16_t wait) {
    for(uint16_t i=0; i<neo_pixel.numPixels(); i++) {
    neo_pixel.setPixelColor(i, c);
  }
  neo_pixel.show();
  delay(wait);
  neo_pixel_clear();
}

// Fill the dots one after the other with a color
void color_wipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<neo_pixel.numPixels(); i++) {
    neo_pixel.setPixelColor(i, c);
    neo_pixel.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<neo_pixel.numPixels(); i++) {
      neo_pixel.setPixelColor(i, wheel((i+j) & 255));
    }
    neo_pixel.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t wheel(byte wheel_pos) {
  wheel_pos = 255 - wheel_pos;
  if(wheel_pos < 85) {
    return neo_pixel.Color(255 - wheel_pos * 3, 0, wheel_pos * 3);
  }
  if(wheel_pos < 170) {
    wheel_pos -= 85;
    return neo_pixel.Color(0, wheel_pos * 3, 255 - wheel_pos * 3);
  }
  wheel_pos -= 170;
  return neo_pixel.Color(wheel_pos * 3, 255 - wheel_pos * 3, 0);
}

void show_coord(uint8 x, uint8 y) {
  for(uint16_t i=0; i<x; i++) {
    flash_all(neo_pixel.Color(255, 0, 0), 100);
    delay(200);
  }
  delay(500);
  for(uint16_t i=0; i<y; i++) {
    flash_all(neo_pixel.Color(0, 255, 0), 100);
    delay(200);
  }

}


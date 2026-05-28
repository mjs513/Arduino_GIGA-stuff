#include "Arduino.h"
#include "Arduino_GigaDisplay.h"
#include "SDRAM.h"

Display display;

uint16_t color_table[] = {
  RGB565_RED, RGB565_MAGENTA, RGB565_YELLOW, RGB565_BLACK, RGB565_GREEN,
  RGB565_WHITE, RGB565_ORANGE, RGB565_GREENYELLOW, RGB565_PINK
};
uint8_t color_index = 0;



void fillScreen(uint16_t color) {
  uint32_t sizeof_framebuffer2 = 2 * 160 * 160;
  uint8_t* frameBuffer2 =  (uint8_t*)  malloc(sizeof_framebuffer2);
  for (int x = 0; x < 480; x += 160) {
    for (int y = 0; y < 800; y += 160) {
      uint16_t *pb = (uint16_t *)frameBuffer2;
      uint32_t count = sizeof_framebuffer2 / 2;
      while(count--) *pb++ = color;
      display.write8(x, y, frameBuffer2);
    }
  }
  free(frameBuffer2);
}

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  uint32_t sizeof_framebuffer2 = 2 * 160 * 160;
  uint8_t* frameBuffer2 =  (uint8_t*)  malloc(sizeof_framebuffer2);
 
  uint16_t *pb = (uint16_t *)frameBuffer2;
  uint32_t count = sizeof_framebuffer2 / 2;

  for (int x1 = 0; x1 < w; x1 += w) {
    for (int y1 = 0; y1 < w; y1 += h) {
      while(count--) *pb++ = color;
      display.write8(x+x1, y+y1, frameBuffer2);
    }
  }
  free(frameBuffer2);
}

void setup() {
  printk("setup called\n");
  while (!Serial && millis() < 5000)
    ;
  Serial.begin(115200);

  bool display_started; 
  if (!(display_started = display.begin())) {
    Serial.println("Failed to start display");
  };
  printk("After display.begin() %x\n", display_started);

  Serial.println("Display configured!!");
  pinMode(LED_BUILTIN, OUTPUT);

  printk("Before getrameBuffer\n");
  void* FB =  display.getFrameBuffer();
  printk("\tFB: %p\n", FB);
  if (FB == nullptr){
    Serial.println("Memory not allocated successfully." );
    while(1){}
  }
  //Allocate memory to the framebuffer
  uint32_t sizeof_framebuffer = 2 * 160 * 160;
  void* ptrFB = malloc(sizeof_framebuffer);
  printk("\tptrFB: %p %u\n", ptrFB, sizeof_framebuffer);
  if (ptrFB == nullptr) {
      SDRAM.begin();
      ptrFB = (uint16_t*)SDRAM.malloc(sizeof_framebuffer);

      printk("\tptrFB SDRAM: %p\n", ptrFB);
  }
  // Cast the void pointer to an int pointer to use it
  uint8_t* frameBuffer = static_cast<uint8_t*>(ptrFB);

  printk("Before setFrameDesc\n");
  display.setFrameDesc(160, 160, 160, sizeof_framebuffer);
  printk("After setFrameDesc\n");
  display.startFrameBuffering();
  printk("After startFrameBuffering\n");
  fillScreen(RGB565_DARKGREY);

  for (int x = 0; x < 480; x += 160) {
    for (int y = 0; y < 800; y += 160) {
      uint16_t *pb = (uint16_t *)frameBuffer;
      uint32_t count = sizeof_framebuffer / 2;
      while(count--) *pb++ = color_table[color_index];
      display.write8(x, y, frameBuffer);
      color_index++;
      if (color_index == (sizeof(color_table)/sizeof(color_table[0]))) color_index = 0;
    }
  }

  delay(2000);
  fillScreen(RGB565_DARKGREY);
  fillRect(160, 160, 160, 160, RGB565_RED);

  display.endFrameBuffering();
  free(ptrFB);

  delay(2000);
  display.startFrameBuffering();
  fillScreen(RGB565_BLACK);
  display.endFrameBuffering();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
}

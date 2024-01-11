#define USE_ADAFRUIT_GFX_LAYERS

#include <inttypes.h>
#include "driver/gpio.h"
#define gpio_pad_select_gpio esp_rom_gpio_pad_select_gpio
#include <MatrixHardware_ESP32_V0.h>
#include <SmartMatrix.h>

#define COLOR_DEPTH 24
const uint16_t kMatrixWidth = 128;
const uint16_t kMatrixHeight = 32;// HUB12 panels have limited support, the height needs to be doubled to account for the fact they only use one RGB channel, though SmartMatrix Library will fill the second channel
const uint8_t kRefreshDepth = 36;
const uint8_t kDmaBufferRows = 4;
const uint8_t kPanelType = SM_PANELTYPE_HUB75_32ROW_MOD16SCAN;
const uint32_t kMatrixOptions = (SMARTMATRIX_OPTIONS_INVERT_R1R2);

SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, kMatrixOptions);

#if 1
  const uint8_t kBackgroundLayerOptions = (SM_BACKGROUND_GFX_OPTIONS_NONE);
  SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(backgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kBackgroundLayerOptions);
#else
  const uint8_t kGFXMonoLayerOptions = (SM_GFX_MONO_OPTIONS_NONE);
  SMARTMATRIX_ALLOCATE_GFX_MONO_LAYER(backgroundLayer, kMatrixWidth, kMatrixHeight, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kGFXMonoLayerOptions);
#endif

#define BLACK   0x0000
#define RED     0xF800



void testText() {
	backgroundLayer.fillScreen(BLACK);
	backgroundLayer.setCursor(0, 0);
	backgroundLayer.setTextColor(RED);
	backgroundLayer.setTextSize(1);
	backgroundLayer.println("Hello World!");
}

void testText2() {
	backgroundLayer.fillScreen(BLACK);
	backgroundLayer.setCursor(0, 0);
	backgroundLayer.setTextColor(RED);
	backgroundLayer.setTextSize(2);
	backgroundLayer.println("Hello World! larger");
}


unsigned long testFastLines(uint16_t color1, uint16_t color2) {
  unsigned long start;
  int           x, y, w = backgroundLayer.width(), h = backgroundLayer.height();

  backgroundLayer.fillScreen(BLACK);
  start = micros();
  for(y=0; y<h; y+=5) {
    backgroundLayer.drawFastHLine(0, y, w, color1);
    backgroundLayer.swapBuffers();
  }
  for(x=0; x<w; x+=5) {
    backgroundLayer.drawFastVLine(x, 0, h, color2);
    backgroundLayer.swapBuffers();
  }

  return micros() - start;
}



void setup() {
  matrix.addLayer(&backgroundLayer);
  matrix.begin();
  matrix.setBrightness(128);

  backgroundLayer.setTextSize(1);
  backgroundLayer.setTextColor(RED);
  backgroundLayer.swapBuffers();
}

void loop() {
  testText();
  backgroundLayer.swapBuffers();
  delay(3000);
  testText2();
  backgroundLayer.swapBuffers();
  delay(3000);
  testFastLines(RED, RED);
  delay(3000);
}




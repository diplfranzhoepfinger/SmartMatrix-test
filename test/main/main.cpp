#define USE_ADAFRUIT_GFX_LAYERS

#include <inttypes.h>
#include "driver/gpio.h"
#define gpio_pad_select_gpio esp_rom_gpio_pad_select_gpio
#include <MatrixHardware_ESP32_V0.h>
#include <SmartMatrix.h>

#define COLOR_DEPTH 24
const uint16_t kMatrixWidth = 64;
const uint16_t kMatrixHeight = 32;// HUB12 panels have limited support, the height needs to be doubled to account for the fact they only use one RGB channel, though SmartMatrix Library will fill the second channel
const uint8_t kRefreshDepth = 36;
const uint8_t kDmaBufferRows = 4;
const uint8_t kPanelType = SMARTMATRIX_HUB12_16ROW_32COL_MOD4SCAN;
const uint32_t kMatrixOptions = (SMARTMATRIX_OPTIONS_HUB12_MODE);

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
	backgroundLayer.setTextColor(RED);  backgroundLayer.setTextSize(1);
	backgroundLayer.println("Hello World!");
}



void setup() {
  matrix.addLayer(&backgroundLayer);
  matrix.begin();
  matrix.setBrightness(128);

  backgroundLayer.setTextSize(1);
  backgroundLayer.setTextColor(RED);
  backgroundLayer.println("Test");
  backgroundLayer.swapBuffers();
}

void loop() {
  for(uint8_t rotation=0; rotation<4; rotation++) {
    backgroundLayer.setRotation(rotation);
    testText();
    backgroundLayer.swapBuffers();
    delay(1000);
  }
}




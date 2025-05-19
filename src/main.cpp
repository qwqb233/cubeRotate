/*
 * @Author: qwqb233 57853293+qwqb233@users.noreply.github.com
 * @Date: 2025-04-30 16:24:04
 * @LastEditors: qwqb233 57853293+qwqb233@users.noreply.github.com
 * @FilePath: \C3_TFT\src\main.cpp
 * @Description: 
 */
#include <Arduino.h>

#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include <cmath>
#include <Wire.h>

#include "esp_system.h"

#include "../include/Matrix.h"
#include "../include/solid.h"
 
TFT_eSPI tft = TFT_eSPI(); // Invoke library, pins defined in User_Setup.h
 
uint32_t targetTime = 0; // for next 1 second timeout
 
byte omm = 99;
bool initial = 1;
byte xcolon = 0;
unsigned int colour = 0;

#define TOTAL_HEAP_SIZE (400 * 1024) // 根据实际芯片型号调整

void print_memory_usage() {
    size_t free_heap = esp_get_free_heap_size();
    size_t used_heap = TOTAL_HEAP_SIZE - free_heap;
    float usage_percent = (float)used_heap / TOTAL_HEAP_SIZE * 100;

    printf("memory usage: %.2f%% (used %d bit / total %d bit)\r\n", 
           usage_percent, used_heap, TOTAL_HEAP_SIZE);
}

void print_memory_info() {
  // 获取内部堆的空闲内存
  size_t free_internal = esp_get_free_internal_heap_size();
  Serial.printf("free_memory: %d bit\r\n", free_internal);

  // 获取总空闲内存（如果启用了外部SPIRAM）
  size_t free_total = esp_get_free_heap_size();
  Serial.printf("total_memory: %d bit\r\n", free_total);
}

static uint8_t conv2d(const char *p)
{
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}
 
uint8_t hh = conv2d(__TIME__), mm = conv2d(__TIME__ + 3), ss = conv2d(__TIME__ + 6); // Get H, M, S from compile time
/* rotation_3d rotation_main = {
  .x = PI/4,
  .y = PI/4,
  .z = PI/4
}; */
//Cube cube_main2(20, 20, 20, 40, 30,rotation_main, TFT_GREEN);

Sphere test(30,30,30,2);
double pi = 3.14159265358979323846;
void setup(void)
{
  tft.init();
  //tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
 
  tft.setTextColor(TFT_YELLOW, TFT_BLACK); // Note: the new fonts do not draw the background colour
 
  targetTime = millis() + 1000;
  
  test.rotation_to(0,0,pi/3);

  Serial.begin(115200);

  print_memory_info();
}

void loop()
{

  test.rotation_to(0,pi/12,0);
  test.tft_draw(tft);
  print_memory_usage();
  delay(30);

  //Serial.printf(""); 
}
 
/* void loop()
{
  if (targetTime < millis())
  {
    targetTime = millis() + 1000;
    ss++; // Advance second
    if (ss == 60)
    {
      ss = 0;
      omm = mm;
      mm++; // Advance minute
      if (mm > 59)
      {
        mm = 0;
        hh++; // Advance hour
        if (hh > 23)
        {
          hh = 0;
        }
      }
    }
 
    if (ss == 0 || initial)
    {
      initial = 0;
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
      tft.setCursor(8, 52);
      tft.print(__DATE__); // This uses the standard ADAFruit small font
 
      tft.setTextColor(TFT_BLUE, TFT_BLACK);
      tft.drawCentreString("It is windy", 120, 48, 2); // Next size up font 2
 
      // tft.setTextColor(0xF81F, TFT_BLACK); // Pink
      // tft.drawCentreString("12.34",80,100,6); // Large font 6 only contains characters [space] 0 1 2 3 4 5 6 7 8 9 . : a p m
    }
 
    // Update digital time
    byte xpos = 6;
    byte ypos = 0;
    if (omm != mm)
    { // Only redraw every minute to minimise flicker
      // Uncomment ONE of the next 2 lines, using the ghost image demonstrates text overlay as time is drawn over it
      tft.setTextColor(0x39C4, TFT_BLACK); // Leave a 7 segment ghost image, comment out next line!
      // tft.setTextColor(TFT_BLACK, TFT_BLACK); // Set font colour to black to wipe image
      //  Font 7 is to show a pseudo 7 segment display.
      //  Font 7 only contains characters [space] 0 1 2 3 4 5 6 7 8 9 0 : .
      tft.drawString("88:88", xpos, ypos, 7); // Overwrite the text to clear it
      tft.setTextColor(0xFBE0);               // Orange
      omm = mm;
 
      if (hh < 10)
        xpos += tft.drawChar('0', xpos, ypos, 7);
      xpos += tft.drawNumber(hh, xpos, ypos, 7);
      xcolon = xpos;
      xpos += tft.drawChar(':', xpos, ypos, 7);
      if (mm < 10)
        xpos += tft.drawChar('0', xpos, ypos, 7);
      tft.drawNumber(mm, xpos, ypos, 7);
    }
 
    if (ss % 2)
    { // Flash the colon
      tft.setTextColor(0x39C4, TFT_BLACK);
      xpos += tft.drawChar(':', xcolon, ypos, 7);
      tft.setTextColor(0xFBE0, TFT_BLACK);
    }
    else
    {
      tft.drawChar(':', xcolon, ypos, 7);
      colour = random(0xFFFF);
      // Erase the old text with a rectangle, the disadvantage of this method is increased display flicker
      tft.fillRect(0, 64, 160, 20, TFT_BLACK);
      tft.setTextColor(colour);
      tft.drawRightString("Colour", 75, 64, 4); // Right justified string drawing to x position 75
      String scolour = String(colour, HEX);
      scolour.toUpperCase();
      char buffer[20];
      scolour.toCharArray(buffer, 20);
      tft.drawString(buffer, 82, 64, 4);
    }
  }
} */
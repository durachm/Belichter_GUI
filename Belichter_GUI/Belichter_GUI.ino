//This example implements a simple sliding On/Off button. The example
// demonstrates drawing and touch operations.
//
//Thanks to Adafruit forums member Asteroid for the original sketch!
//
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000

#define STMPE_CS 8
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);
#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

int time_0 = 10;
int time_1 = 50;
int time_2 = 400;
bool relais_on = 0;
bool dark_on = 0;

bool start_0 = 0;
bool start_1 = 0;
bool start_2 = 0;

unsigned long start_time = 0;

class Button {
public:
  int _x = 0;
  int _y = 0;
  Button(int x, int y) {  // Constructor
    _x = x;
    _y = y;
  }

  void drawButton(const char* text) {
    tft.fillRect(_x, _y, 100, 50, ILI9341_BLACK);
    tft.drawRect(_x, _y, 100, 50, ILI9341_DARKGREY);
    tft.setCursor(_x + 20, _y + 15);
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_WHITE);
    tft.println(text);
    // writeText(100,200, "hallo");
  }
  void drawButton(int text) {
    tft.fillRect(_x, _y, 100, 50, ILI9341_BLACK);
    tft.drawRect(_x, _y, 100, 50, ILI9341_DARKGREY);
    tft.setCursor(_x + 30, _y + 15);
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_WHITE);
    tft.println(text);
    // writeText(100,200, "hallo");
  }
  bool justPressed(int px, int py) {
    //tft.drawLine(_x, _y, _x+100, _y+50, ILI9341_DARKCYAN);
    if ((px > _x) && (px < (_x + 100)) && (py > _y) && (py < (_y + 50))) {
      return 1;
    } else {
      return 0;
    }
  }
  int numberfield(void) {
    tft.fillScreen(ILI9341_BLACK);

    for (int x = 0; x < 3; x++) {
      for (int y = 0; y < 4; y++) {
        tft.drawRect(x * 70 + 10 * x + 5, y * 50 + 10 * y + 85, 70, 50, ILI9341_DARKGREY);
      }
    }
    int number = 1;
    tft.setTextSize(2);
    for (int y = 0; y < 3; y++) {
      for (int x = 0; x < 3; x++) {
        tft.setCursor(x * 70 + 10 * x + 35, y * 50 + 10 * y + 105);
        tft.println(number);
        number++;
      }
      tft.setCursor(115, 280);
      tft.println(0);
      tft.drawFastHLine(185, 290, 30, ILI9341_WHITE);
      tft.drawFastVLine(215, 280, 10, ILI9341_WHITE);
      tft.drawLine(185, 290, 190, 295, ILI9341_WHITE);
      tft.drawLine(185, 290, 190, 285, ILI9341_WHITE);

      tft.drawFastHLine(25, 290, 30, ILI9341_WHITE);
      tft.drawLine(25, 290, 30, 295, ILI9341_WHITE);
      tft.drawLine(25, 290, 30, 285, ILI9341_WHITE);
    }

    int result = 0;
    int last_result = 0;

    while (1) {

      if (!ts.touched()) {


        TS_Point p = ts.getPoint();
        // Scale using the calibration #'s
        // and rotate coordinate system
        int px = map(p.x, TS_MINY, TS_MAXY, 0, tft.width());
        int py = map(p.y, TS_MINX, TS_MAXX, 0, tft.height());

        number = 1;

        for (int y = 0; y < 3; y++) {
          for (int x = 0; x < 3; x++) {
            //tft.drawLine(x * 70 + 10 * x + 5, y * 50 + 10 * y + 85, x * 70 + 10 * x + 5 + 70, y * 50 + 10 * y + 85 + 50, ILI9341_MAGENTA);
            if ((px > x * 70 + 10 * x + 5) && (px < (x * 70 + 10 * x + 5 + 70)) && (py > y * 50 + 10 * y + 85) && (py < (y * 50 + 10 * y + 85 + 50)) && result < 9999) {
              result = result * 10 + number;
            }
            number++;
          }
        }
        if ((px > 5) && (px < (5 + 70)) && (py > 265) && (py < (265 + 50))) {  //return
          result = 0;
        }
        if ((px > 85) && (px < (85 + 70)) && (py > 265) && (py < (265 + 50)) && result < 9999) {  //0
          result = result * 10;
        }
        if ((px > 165) && (px < (165 + 70)) && (py > 265) && (py < (265 + 50))) {  //enter
          tft.fillScreen(ILI9341_BLACK);
          return result;
        }


        if (last_result != result) {
          tft.fillRect(0, 0, 240, 50, ILI9341_BLACK);
          tft.setCursor(115, 20);
          tft.setTextSize(3);
          tft.println(result);
          last_result = result;
        }
      }
    }
  }
};





void setup(void) {
  tft.begin();
  ts.begin();


  tft.fillScreen(ILI9341_BLACK);
  // origin = left,top landscape (USB left upper)
  // tft.setRotation(0);
}

void loop() {
  Button Time0(10, 10);
  Button Time1(10, 80);
  Button Time2(10, 150);
  Button Start0(130, 10);
  Button Start1(130, 80);
  Button Start2(130, 150);
  Button OnOff(130, 220);
  Button Darkmode(10, 220);

  Time0.drawButton(time_0);
  Start0.drawButton("Start");
  Time1.drawButton(time_1);
  Start1.drawButton("Start");
  Time2.drawButton(time_2);
  Start2.drawButton("Start");
  OnOff.drawButton("On");
  Darkmode.drawButton("Dark");

  bool refresh = 0;
  int time_counter = 0;

  while (1) {  // Gui

    if (!ts.touched()) {
      TS_Point p = ts.getPoint();
      // Scale using the calibration #'s
      // and rotate coordinate system
      int x = map(p.x, TS_MINY, TS_MAXY, 0, tft.width());
      int y = map(p.y, TS_MINX, TS_MAXX, 0, tft.height());

      if (Time0.justPressed(x, y)) {
        time_0 = Time0.numberfield();
        refresh = 1;
      } else if (Time1.justPressed(x, y)) {
        time_1 = Time1.numberfield();
        refresh = 1;
      } else if (Time2.justPressed(x, y)) {
        time_2 = Time2.numberfield();
        refresh = 1;
      }

      else if (Start0.justPressed(x, y)) {
        start_0 = !start_0;
        refresh = 1;
        start_time = millis();
      } else if (Start1.justPressed(x, y)) {
        start_1 = !start_1;
        refresh = 1;
        start_time = millis();
      } else if (Start2.justPressed(x, y)) {
        start_2 = !start_2;
        refresh = 1;
        start_time = millis();
      }

      else if (Darkmode.justPressed(x, y)) {
        dark_on = !dark_on;
        refresh = 1;
      } else if (OnOff.justPressed(x, y)) {
        relais_on = !relais_on;
        refresh = 1;
      }

      if (refresh == 1) {
        refresh = 0;
        Time0.drawButton(time_0);
        Time1.drawButton(time_1);
        Time2.drawButton(time_2);
        if (relais_on) {
          OnOff.drawButton("Off");
          tft.fillRect(5, 280, 230, 35, ILI9341_RED);
        } else {
          OnOff.drawButton("On");
        }
        if (dark_on) {
          Darkmode.drawButton("Light");
        } else {
          Darkmode.drawButton("Dark");
        }
        if (start_0) {
          Start0.drawButton("Stop");
          tft.fillRect(5, 280, 230, 35, ILI9341_RED);
        } else {
          Start0.drawButton("Start");
        }
        if (start_1) {
          Start1.drawButton("Stop");
          tft.fillRect(5, 280, 230, 35, ILI9341_RED);
        } else {
          Start1.drawButton("Start");
        }
        if (start_2) {
          Start2.drawButton("Stop");
          tft.fillRect(5, 280, 230, 35, ILI9341_RED);
        } else {
          Start2.drawButton("Start");
        }
      }
    }

    if (start_0 || start_1 || start_2 || relais_on) {


      if (start_0)
        time_counter = time_0;
      if (start_1)
        time_counter = time_1;
      if (start_2)
        time_counter = time_2;

      if (!relais_on) {

        int width = map((millis() - start_time) / 1000, 0, time_counter, 0, 230);

        tft.fillRect(235 - width, 280, width, 35, ILI9341_BLACK);

        if (time_counter < (millis() - start_time) / 1000) {
          start_0 = 0;
          start_1 = 0;
          start_2 = 0;
          start_time = 0;
          refresh = 1;
        }
      }
    }
    if (!start_0 && !start_1 && !start_2 && !relais_on) {
      tft.fillRect(5, 280, 230, 35, ILI9341_BLACK);
    }
  }
}
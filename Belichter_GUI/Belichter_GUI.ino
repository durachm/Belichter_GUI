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

int backlight = 3;
int brightness = 255;
int time_0 = 5;
int time_1 = 30;
int time_2 = 60;
bool relais_on = 0;
bool dark_on = 0;


int relais_pin = 7;

bool start_0 = 0;
bool start_1 = 0;
bool start_2 = 0;

unsigned long start_time = 0;

unsigned long lastMillis = 0;

uint16_t background_colour = 0x0000;  ///<   0,   0,   0
uint16_t foreground_colour = 0xF800;  ///< 255,   0,   0


class Button {
public:
  int _x = 0;
  int _y = 0;
  Button(int x, int y) {  // Constructor
    _x = x;
    _y = y;
  }

  void drawButton(const char* text) {
    tft.fillRect(_x, _y, 100, 50, background_colour);
    tft.drawRect(_x, _y, 100, 50, foreground_colour);
    tft.setCursor(_x + 20, _y + 15);
    tft.setTextSize(2);
    tft.setTextColor(foreground_colour);
    tft.println(text);
    // writeText(100,200, "hallo");
  }
  void drawButton(int text) {
    tft.fillRect(_x, _y, 100, 50, background_colour);
    tft.drawRect(_x, _y, 100, 50, foreground_colour);
    tft.setCursor(_x + 30, _y + 15);
    tft.setTextSize(2);
    tft.setTextColor(foreground_colour);
    tft.println(text);
    // writeText(100,200, "hallo");
  }
  void drawSun() {
    tft.fillRect(_x, _y, 100, 50, background_colour);
    tft.drawRect(_x, _y, 100, 50, foreground_colour);
    tft.drawCircle(_x + 50, _y + 25, 4, foreground_colour);

    tft.drawFastHLine(_x + 58, _y + 25, 8, foreground_colour);
    tft.drawFastHLine(_x + 35, _y + 25, 8, foreground_colour);
    tft.drawFastVLine(_x + 50, _y + 33, 8, foreground_colour);
    tft.drawFastVLine(_x + 50, _y + 10, 8, foreground_colour);

    tft.drawLine(_x + 56, _y + 31, _x + 61, _y + 36, foreground_colour);
    tft.drawLine(_x + 56, _y + 19, _x + 61, _y + 14, foreground_colour);
    tft.drawLine(_x + 44, _y + 31, _x + 39, _y + 36, foreground_colour);
    tft.drawLine(_x + 44, _y + 19, _x + 39, _y + 14, foreground_colour);
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
    tft.fillScreen(background_colour);

    for (int x = 0; x < 3; x++) {
      for (int y = 0; y < 4; y++) {
        tft.drawRect(x * 70 + 10 * x + 5, y * 50 + 10 * y + 85, 70, 50, foreground_colour);
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
      tft.drawFastHLine(185, 290, 30, foreground_colour);
      tft.drawFastVLine(215, 280, 10, foreground_colour);
      tft.drawLine(185, 290, 190, 295, foreground_colour);
      tft.drawLine(185, 290, 190, 285, foreground_colour);

      tft.drawFastHLine(25, 290, 30, foreground_colour);
      tft.drawLine(25, 290, 30, 295, foreground_colour);
      tft.drawLine(25, 290, 30, 285, foreground_colour);
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
          tft.fillScreen(background_colour);
          return result;
        }


        if (last_result != result) {
          tft.fillRect(0, 0, 240, 50, background_colour);
          tft.setCursor(115, 20);
          tft.setTextSize(3);
          tft.println(result);
          last_result = result;
        }
      }
    }
  }

  void brightnessControl(void) {
    tft.fillScreen(background_colour);
    tft.drawFastHLine(20, 280, 200, foreground_colour);
    tft.drawFastVLine(20, 265, 30, foreground_colour);
    tft.drawFastVLine(220, 265, 30, foreground_colour);
    tft.drawRect(5, 5, 230, 100, foreground_colour);

    tft.drawRect(5, 110, 230, 100, foreground_colour);

    //tft.fillRect(10, 70, 220, 80, foreground_colour);
    tft.setCursor(35, 35);
    tft.setTextSize(2);
    tft.println("Set brightness");

    tft.setCursor(25, 145);
    tft.setTextSize(2);
    tft.println("Inverted Colors");

    int last_px = 0;

    while (1) {
      if (ts.touched()) {


        TS_Point p = ts.getPoint();
        // Scale using the calibration #'s
        // and rotate coordinate system
        int px = map(p.x, TS_MINY, TS_MAXY, 0, tft.width());
        int py = map(p.y, TS_MINX, TS_MAXX, 0, tft.height());

        if ((px > 5) && (px < (5 + 230)) && (py > 5) && (py < (5 + 100))) {  // Check if Enter is pressed
          tft.fillScreen(background_colour);
          delay(400);
          break;
        }

        if ((px > 5) && (px < (5 + 230)) && (py > 110) && (py < (110 + 100))) {  // Check if Enter is pressed
          if (background_colour == 0x0000) {
            background_colour = 0xF800;  ///<   0,   0,   0
            foreground_colour = 0x0000;  ///< 255,   0,   0
          } else {
            background_colour = 0x0000;  ///<   0,   0,   0
            foreground_colour = 0xF800;  ///< 255,   0,   0
          }
          tft.fillScreen(background_colour);
          delay(400);
          break;
        }

        if (px > 0 && (px < (240)) && (py > 240) && (py < 315)) {

          if (px < 20)
            px = 20;
          if (px > 220)
            px = 220;

          tft.fillCircle(last_px, 280, 12, background_colour);
          tft.fillCircle(px, 280, 12, foreground_colour);
          tft.drawFastHLine(20, 280, 200, foreground_colour);
          tft.drawFastVLine(20, 265, 30, foreground_colour);
          tft.drawFastVLine(220, 265, 30, foreground_colour);
          last_px = px;

          if (px <= 140)
            brightness = map(px, 20, 140, 1, 15);
          else
            brightness = map(px, 140, 220, 16, 255);
          analogWrite(backlight, brightness);  // Set brightness

          tft.fillRect(10, 70, 220, 20, background_colour);
          tft.setCursor(110, 75);
          tft.setTextSize(2);
          tft.println(brightness);
          // tft.setCursor(110, 100);
          // tft.println(px);
        }
      }
    }
    while (1) {
      if (!ts.touched()) {
        TS_Point p = ts.getPoint();
        delay(200);
        return;
      }
    }
  }
};





void setup(void) {

  pinMode(backlight, OUTPUT);  // sets the pin as output
  pinMode(relais_pin, OUTPUT);
  digitalWrite(relais_pin, 1);
  tft.begin();
  ts.begin();
  tft.fillScreen(background_colour);
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
  Darkmode.drawSun();

  bool refresh = 0;
  int time_counter = 0;
  analogWrite(backlight, brightness);  // Set brightness
  //analogWrite(backlight, 0);  // Set brightness
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
        Darkmode.brightnessControl();
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
        Darkmode.drawSun();
        if (relais_on) {
          OnOff.drawButton("Off");
          tft.fillRect(5, 280, 230, 35, foreground_colour);
        } else {
          OnOff.drawButton("On");
        }

        if (start_0) {
          Start0.drawButton("Stop");
          tft.fillRect(5, 280, 230, 35, foreground_colour);
        } else {
          Start0.drawButton("Start");
        }
        if (start_1) {
          Start1.drawButton("Stop");
          tft.fillRect(5, 280, 230, 35, foreground_colour);
        } else {
          Start1.drawButton("Start");
        }
        if (start_2) {
          Start2.drawButton("Stop");
          tft.fillRect(5, 280, 230, 35, foreground_colour);
        } else {
          Start2.drawButton("Start");
        }
      }
    }

    if (start_0 || start_1 || start_2 || relais_on) {  // turn relais on

      digitalWrite(relais_pin, 0);
      if (start_0)
        time_counter = time_0;
      if (start_1)
        time_counter = time_1;
      if (start_2)
        time_counter = time_2;

      if (!relais_on) {

        int width = map((millis() - start_time) / 1000, 0, time_counter, 0, 230);

        tft.fillRect(235 - width, 280, width, 35, background_colour);

        if (millis() - lastMillis >= 1000) {
          lastMillis = millis();
          if (start_0)
            Time0.drawButton(time_0 - (millis() - start_time) / 1000);
          if (start_1)
            Time1.drawButton(time_1 - (millis() - start_time) / 1000);
          if (start_2)
            Time2.drawButton(time_2 - (millis() - start_time) / 1000);
        }

        if (time_counter <= (millis() - start_time) / 1000) {  // timer
          start_0 = 0;
          start_1 = 0;
          start_2 = 0;
          start_time = 0;
          refresh = 1;
        }
      }
    }
    if (!start_0 && !start_1 && !start_2 && !relais_on) {  // clear bar / turn relais off
      tft.fillRect(5, 280, 230, 35, background_colour);
      digitalWrite(relais_pin, 1);
    }
  }
}
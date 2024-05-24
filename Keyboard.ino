#include "FS.h"
#include <SPI.h>
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite pwdField = TFT_eSprite(&tft);
//if anyone wants turn the keyboard into Sprite and stop flickering when pressing Caps

uint16_t calData[5] = { 396, 3350, 270, 3369, 7 };

unsigned long milliStart = 0;

String symbol[8][12] = {
  { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=" },
  { "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]" },
  { "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "'", "\\" },
  { "z", "x", "c", "v", "b", "n", "m", ",", ".", "/", "`", " " },

  { "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_", "+" },
  { "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "{", "}" },
  { "A", "S", "D", "F", "G", "H", "J", "K", "L", ":", "\"", "|" },
  { "Z", "X", "C", "V", "B", "N", "M", "<", ">", "?", "~", " " }
};

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setTouch(calData);
  tft.setRotation(1);

  String pwd = getKeyboardOut();
  Serial.print("Final Password is : ");
  Serial.println(pwd);

  tft.fillScreen(TFT_BLACK);
}

void loop() {
}



String getKeyboardOut() {
  uint16_t x = 0, y = 0;
  String pwd ="";
  String pressedKey;

  byte xIndex, yIndex;
  bool enterPressed = false;
  bool capsOn = false;

  pwdField.createSprite(315, 40);

  drawButtons(capsOn);

  milliStart = millis();

  while (!enterPressed) {
    bool pressed = tft.getTouch(&x, &y);
    if (pressed && (millis() - milliStart) > 500) {
      if (x < 55 && y > 201) {
        if (capsOn) {
          capsOn = false;
        } else {
          capsOn = true;
        }
        drawButtons(capsOn);
      }

      if (x > 55 && x < 193 && y > 201) {
        pressedKey = " ";
      }

      if (x>193 && x < 249 && y >201){
        if (pwd != ""){
          pwd.remove(pwd.length() - 1);
          pwdField.fillSprite(TFT_WHITE);
        }
      }

      if ((x > 249 && y > 201) || (x > 287 && y > 172)) {
        if (enterPressed) {
          enterPressed = false;
        } else {
          enterPressed = true;
        }
      }

      if (y > 85 && y < 201) {
        if (capsOn) {
          xIndex = (int)(x - 1) / 26;
          yIndex = (int)(((y - 85)) / 29 + 4);
        } else {
          xIndex = (int)(x - 1) / 26;
          yIndex = (int)(((y - 85)) / 29);
        }
        pressedKey = String(symbol[yIndex][xIndex]);
      }

      milliStart = millis();

      pwd += pressedKey;

      Serial.println(pressedKey);
      Serial.println(pwd);

      pwdField.drawString(pwd , 10 , 35 , 4);
      pwdField.pushSprite(0, 40);

      pressedKey = "";
    }
  }

  return pwd;
}

void drawButtons(bool isCapsOn) {
  tft.fillScreen(TFT_BLACK);

  pwdField.fillSprite(TFT_WHITE);
  pwdField.setRotation(1);
  pwdField.setTextDatum(BL_DATUM);
  pwdField.setTextColor(TFT_BLACK,TFT_WHITE);
  pwdField.pushSprite(0, 40);

  tft.setTextSize(2);
  tft.setTextColor(TFT_RED);
  if (!isCapsOn) {
    for (int j = 0; j < 4; j++) {
      for (int i = 0; i < 12; i++) {
        tft.fillRoundRect((i * 26) + 1, j * 29 + 85, 25, 28, 3, TFT_YELLOW);
        tft.setCursor(i * 26 + 6, j * 29 + 90);
        tft.print(symbol[j][i]);
      }
    }
  } else {
    for (int j = 4; j < 8; j++) {
      for (int i = 0; i < 12; i++) {
        tft.fillRoundRect((i * 26) + 1, (j - 4) * 29 + 85, 25, 28, 3, TFT_YELLOW);
        tft.setCursor(i * 26 + 6, (j - 4) * 29 + 90);
        tft.print(symbol[j][i]);
      }
    }
  }
  tft.fillRoundRect(1, 201, 55, 28, 3, TFT_CYAN);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(5, 211);
  tft.print("Caps");

  tft.fillRoundRect(57, 201, 135, 28, 3, TFT_CYAN);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(100, 211);
  tft.print("Space");

  tft.fillRoundRect(193, 201, 55, 28, 3, TFT_CYAN);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(198, 211);
  tft.print("Back");

  tft.fillRoundRect((11 * 26) + 1, (3) * 29 + 85, 25, 33, 3, TFT_CYAN);
  tft.fillRoundRect(249, 201, 63, 28, 3, TFT_CYAN);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(253, 211);
  tft.print("Enter");
}

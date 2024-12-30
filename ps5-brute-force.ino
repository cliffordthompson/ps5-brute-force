//***************************************************************************//
//*                                                                         *//
//*  Copyright (c) 2024 Clifford Thompson                                   *//
//*                                                                         *//
//*  All code in this file is released under Creative Commons Attribution   *//
//*  (CC-BY) license : https://creativecommons.org/licenses/by/4.0/.        *//
//*                                                                         *//
//***************************************************************************//

#include <Keyboard.h>
#include <ArduinoGraphics.h>
#include <Arduino_LED_Matrix.h>

#include <array>

const uint8_t BUTTON_PIN = 4;
const uint8_t KEYPRESS_DELAY_MS = 500;
const uint8_t ATTEMPT_DELAY_MS = 1500;
const size_t  NUM_CHARACTERS = 10;
const uint8_t ATTEMPTS_PER_BUTTON_PRESS = 200; // The Arduino does like us lockin up the loop() for too long.
const std::array CHARACTERS = {'0','1','2','3','4','5','6','7','8','9'};
const size_t PASSCODE_SIZE = 4;

typedef std::array<char, PASSCODE_SIZE> Passcode;

// Array to hold the current 4-character combination. Change the
// array indices to start at a different point.
Passcode combination = {CHARACTERS[0],CHARACTERS[0],CHARACTERS[0],CHARACTERS[0]}; 

int previousButtonState = HIGH;  // For tracking the state of a pushButton
ArduinoLEDMatrix matrix;

void setup() 
{
  pinMode(BUTTON_PIN, INPUT);
  // initialize control over the keyboard:
  Keyboard.begin();
  delay(1000);

  Serial.begin(115200);
  matrix.begin();

  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  // add some static text
  // will only show "UNO" (not enough space on the display)
  const char text[] = "UNO r4";
  matrix.textFont(Font_4x6);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(text);
  matrix.endText();

  matrix.endDraw();

  delay(2000);

}

void loop() 
{
  int buttonState = digitalRead(BUTTON_PIN);

  if((buttonState != previousButtonState) && (buttonState == HIGH))
  {
    for(int i = 0; i < ATTEMPTS_PER_BUTTON_PRESS; ++i)
    {
      displayPasscodeOnLEDMatrix(combination);
      tryPasscode(combination);
      incrementCombination();
      delay(ATTEMPT_DELAY_MS);
    }  
  }
  previousButtonState = buttonState;
}

// Function to increment the combination
void incrementCombination() {
  for (int i = 3; i >= 0; i--) {  // Start from the least significant character
    // Find the next character
    int currentIndex = indexOf(combination[i]);

    // If it's not the last character, we increment to the next character
    if (currentIndex < NUM_CHARACTERS - 1) {
      combination[i] = CHARACTERS[currentIndex + 1];
      break;
    } else {  // If it's the last character, reset it to the first character and move to the next higher place
      combination[i] = CHARACTERS[0];
    }
  }
}

// Function to find the index of a character in the characters array
int indexOf(char ch) {
  for (int i = 0; i < NUM_CHARACTERS; i++) {
    if (CHARACTERS[i] == ch) {
      return i;
    }
  }
  return -1;  // Return -1 if the character is not found (this shouldn't happen)
}

void tryPasscode(const Passcode& passcode)
{
  for(const auto& number: passcode)
  {
    Keyboard.press(number);
    delay(KEYPRESS_DELAY_MS);
    Keyboard.releaseAll();
    delay(KEYPRESS_DELAY_MS);
  }
  delay(1500);

  Keyboard.press(KEY_RETURN);
  delay(KEYPRESS_DELAY_MS);
  Keyboard.releaseAll();
  delay(KEYPRESS_DELAY_MS);
}

void displayPasscodeOnLEDMatrix(const Passcode& passcode)
{
  std::string displayString;

  for(const auto& character: passcode)
  {
    displayString.push_back(character);
  }

  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  matrix.textScrollSpeed(200);
  matrix.textFont(Font_4x6);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(displayString.c_str());
  matrix.endText(/*SCROLL_LEFT*/);
  matrix.endDraw();
}



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
const uint8_t ATTEMPTS_PER_BUTTON_PRESS = 200; // The Arduino does like us locking up the loop() for too long.
const std::array CHARACTERS = {'0','1','2','3','4','5','6','7','8','9'};
constexpr size_t  NUM_CHARACTERS = CHARACTERS.size();
const size_t PASSCODE_SIZE = 4; // This could be changed for a longer passcode

typedef std::array<char, PASSCODE_SIZE> Passcode;

// Array to hold the current 4-character passcode. Change the array indices to start
// at a different point. The size of the array could be changed here for longer, or
// shorter passcodes.
Passcode g_passcode = {CHARACTERS[0],CHARACTERS[0],CHARACTERS[0],CHARACTERS[0]};

int g_previousButtonState = HIGH;  // For tracking the state of a pushButton
ArduinoLEDMatrix g_matrix;

// ***************************************************************************
// Description:
//   The standard Arduino startup function.
//
// Inputs:
//   None
// Outputs:
//   None
// Returns:
//   None
// ***************************************************************************
//
void
setup()
{
  pinMode(BUTTON_PIN, INPUT);
  // initialize control over the keyboard:
  Keyboard.begin();
  delay(1000);
  displaySplashScreenOnLEDMatrix(g_matrix);

  return;
}

// ***************************************************************************
// Description:
//   The standard Arduino event loop.
//
// Inputs:
//   None
// Outputs:
//   None
// Returns:
//   None
// ***************************************************************************
//
void
loop()
{
  const int buttonState = digitalRead(BUTTON_PIN);

  if((buttonState != g_previousButtonState) && (buttonState == HIGH))
  {
    for(int i = 0; i < ATTEMPTS_PER_BUTTON_PRESS; ++i)
    {
      displayPasscodeOnLEDMatrix(g_matrix, g_passcode);
      tryPasscode(g_passcode);
      incrementPasscode(g_passcode);
      delay(ATTEMPT_DELAY_MS);
    }
  }
  g_previousButtonState = buttonState;

  return;
}

// ***************************************************************************
// Description:
//   This function increments the current passcode to the next one
//   in the sequence.
//
// Inputs:
//   passcode: The passcode to increment
// Outputs:
//   None
// Returns:
//   None
// ***************************************************************************
//
void
incrementPasscode(Passcode& passcode)
{
  // Start from the least significant character
  for (int i = passcode.size() - 1; i >= 0; --i)
  {
    // Find the next character
    const int currentIndex = indexOf(passcode[i]);

    // If it's not the last character, we increment to the next character
    // and stop processing further characters.
    if (currentIndex < NUM_CHARACTERS - 1)
    {
      passcode[i] = CHARACTERS[currentIndex + 1];
      break;
    }
    else
    {  // If it's the last character, reset it to the first character and move to the next higher place
      passcode[i] = CHARACTERS[0];
    }
  }
  return;
}

// ***************************************************************************
// Description:
//   This function attempts to find the index of a character in the
//   characters array
//
// Inputs:
//   ch: The character top find the index of.
// Outputs:
//   None
// Returns:
//   The index of 'ch' in the characters array.
// ***************************************************************************
//
int
indexOf(char ch)
{
  for (int i = 0; i < NUM_CHARACTERS; i++)
  {
    if (CHARACTERS[i] == ch)
    {
      return i;
    }
  }
  return -1;  // Return -1 if the character is not found (this shouldn't happen)
}

// ***************************************************************************
// Description:
//   This function attempts to input the give passcode. The Arduino is acting
//   as a keyboard.
//
// Inputs:
//   passcode: The passcode to try.
// Outputs:
//   None
// Returns:
//   None
// ***************************************************************************
//
void
tryPasscode(const Passcode& passcode)
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

  return;
}

// ***************************************************************************
// Description:
//   Displays the splash screen to the LED Matrix. In this case, it's "PS5".
//   We are limted by the font size of the Arduino Graphics, without have to
//   scroll the text.
//
// Inputs:
//   None
// Outputs:
//   None
// Returns:
//   None
// ***************************************************************************
//
void
displaySplashScreenOnLEDMatrix(ArduinoLEDMatrix& matrix)
{
  Serial.begin(115200);

  matrix.begin();
  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  const char text[] = "PS5";
  matrix.textFont(Font_4x6);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(text);
  matrix.endText();
  matrix.endDraw();

  delay(1000);

  return;
}

// ***************************************************************************
// Description:
//   This function displays a passcode to the LED Matrix.
//
// Inputs:
//   matrix: The LED Matrix to display on
//   passcode: The passcode to display.
// Outputs:
//   None
// Returns:
//   None
// ***************************************************************************
//
void
displayPasscodeOnLEDMatrix(ArduinoLEDMatrix& matrix, const Passcode& passcode)
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

  return;
}

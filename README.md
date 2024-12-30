# PS5 Brute Force Parental Controls
When you have forgotten, or in my case mis-typed, the parental controls passcode and you do not want to reset your PS5 in order to change your parental controls. Luckily, the PS5 does not prevent you from brute-forcing the passcode (at this point).

## Disclaimer

Use at your own risk. It's works on my PS5, but I cannot guarantee that Sony might change things in the future to either lock you out after too many attempts, or if this sketch will brick your PS5.

Note to the kiddos, don't use this to by-pass your parent's parent controls.

## Requirements

1. This sketch requires the Arduino UNO R4 (or similar device). The Arduino UNO R3 (or similar device) will not work as it cannot using the [Keyboard Library](https://docs.arduino.cc/language-reference/en/functions/usb/Keyboard/) from Arduino.

2. This sketch also currently requires either an onboard LED matrix or something external, i.e the ability to use the [ArduinoGraphics Library](https://github.com/arduino-libraries/ArduinoGraphics) and the [Arduino_LED_Matrix Library](https://docs.arduino.cc/tutorials/uno-r4-wifi/led-matrix/) from Arduino.

## The Circuit

Markup : ![](https://docs.arduino.cc/static/73702ee121860fa04c7f6db5bc77183b/a6d36/circuit.png "Simple Button Circuit")

## The Schematic

Markup : ![](https://docs.arduino.cc/static/fb8449b4834dfbbf3c03223c8bc3e5b5/a6d36/schematic.png "Simple Button Circuit Schematic")

## How to Use It

1. Upload the sketch to your Arduino
2. Navigate to the Parental Controls on your PS5
3. Change the entry method from the controller-style to keypad-style.
4. Plug the Arduino into your PS5. The word *UNO* should be displayed on the LED matrix
5. Press the button on your breadboard
6. Wait for the Arduino to crack your code
  - The sketch will do batches of 200, and then pause (awaiting you to press the button again).
  - The PS5 is particular about how fast the passcode is entered, so although the timing is super fast, it appears to work for me. Every 100 codes takes about 7 minutes.
  - The 3 most significant digits are displayed on the LED matrix. So once the passcode is cracked, you roughly know the range of the passcode. Take note of the digits and unplug the Arduino.
  - When the parental controls are unlocked, the sketch will continue to run, but the parental control menus don't really appear to care about the keyboard input.
  - If you want to start at something other than *0000*, update the indices of the *combination* variable.

## Todos

- [ ] Update the display on the LED matrix to show all digits
- [ ] Increase the efficency of the *incrementCombination* function. In particular *indexOf* part could be faster or done a different way.


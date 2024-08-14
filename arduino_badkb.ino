#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keyboard.h>  // Include the Keyboard library

LiquidCrystal_I2C lcd(0x3F, 16, 2);

const int VRx = A0;  // X-axis
const int VRy = A1;  // Y-axis
const int SW = 3;    // Button
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; // Adjust this value as needed
int joystickPinX = A0;
int joystickThreshold = 500;
int currentOption = 0;
int lastOption = 0;
int buttonState = LOW;
int lastButtonState = LOW;

void openNotePad() {
  // Open the Run dialog to launch Notepad
  Keyboard.press(KEY_LEFT_GUI);  // Press the Windows key
  Keyboard.press('r');           // Press the 'r' key
  delay(100);                    // Short delay
  Keyboard.releaseAll();         // Release both keys

  delay(500);  // Short delay before typing the command

  // Type "notepad"
  Keyboard.print("notepad");

  // Press Enter to open Notepad
  Keyboard.write(KEY_RETURN);

  delay(2000);  // Wait for Notepad to open

  Keyboard.print("Hello Arduino BadKB");

  // Optionally, add a new line after typing "Hello"
  Keyboard.write(KEY_RETURN);
}

void setup() {
  Serial.begin(9600);
  Keyboard.begin();
  lcd.init();
  lcd.backlight();
  pinMode(SW, INPUT);
  pinMode(VRx, INPUT);
  lcd.setCursor(0, 0);
  lcd.print("Arduino BadKB");
}

void loop() {
  int joystickValueX = analogRead(VRx);
  buttonState = digitalRead(SW);
  unsigned long currentMillis = millis();

  if (buttonState != lastButtonState) {
    lastDebounceTime = currentMillis;
  }

    if ((currentMillis - lastDebounceTime) > debounceDelay) {
      if (buttonState == LOW) {
        Serial.println("Button pressed");
        openNotePad();
        delay(1000);
    }
  }

  if (joystickValueX < (joystickThreshold - 100)) { 
    currentOption++;
  }else if(joystickValueX > (joystickThreshold + 100) && currentOption > 0){
    currentOption--;
  }

  if(currentOption > 0 && currentOption != lastOption) {
     lastOption = currentOption;
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print(currentOption);
  }

  delay(500);
}

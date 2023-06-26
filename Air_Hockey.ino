#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// IR Sensor 1
const int irSensor1Pin = 2;

// IR Sensor 2
const int irSensor2Pin = 3;

// Servo Motor 1
const int servo1Pin = 4;
Servo servo1;

// Servo Motor 2
const int servo2Pin = 5;
Servo servo2;

// Reset Switch
const int resetSwitchPin = 6;

// LCD Display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Scores
int score1 = 0;
int score2 = 0;

// Variables for tracking state
bool irSensor1Activated = false;
bool irSensor2Activated = false;
bool resetButtonPressed = false;

// Function to update LCD display
void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Score1: ");
  lcd.print(score1);

  lcd.setCursor(0, 1);
  lcd.print("Score2: ");
  lcd.print(score2);
}

// Function to handle servo movement
void moveServo(Servo& servo, int angle) {
  servo.write(angle);
  delay(2000);
  servo.write(0);
}

void setup() {
  lcd.begin(16, 2);
  lcd.backlight();

  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  pinMode(irSensor1Pin, INPUT);
  pinMode(irSensor2Pin, INPUT);
  pinMode(resetSwitchPin, INPUT_PULLUP);

  updateLCD();
}


void loop() {
  // Check if IR Sensor 1 is activated
  if (digitalRead(irSensor1Pin) == LOW && !irSensor1Activated) {
    score1++;
    irSensor1Activated = true;
    moveServo(servo1, 90);
    updateLCD();
  }
  else if (digitalRead(irSensor1Pin) == HIGH) {
    irSensor1Activated = false;
  }

  // Check if IR Sensor 2 is activated
  if (digitalRead(irSensor2Pin) == LOW && !irSensor2Activated) {
    score2++;
    irSensor2Activated = true;
    moveServo(servo2, 90);
    updateLCD();
  }
  else if (digitalRead(irSensor2Pin) == HIGH) {
    irSensor2Activated = false;
  }

  // Check if Reset button is pressed
  if (digitalRead(resetSwitchPin) == LOW && !resetButtonPressed) {
    score1 = 0;
    score2 = 0;
    resetButtonPressed = true;
    updateLCD();
  }
  else if (digitalRead(resetSwitchPin) == HIGH) {
    resetButtonPressed = false;
  }

  // Check if any score reaches 7
  if (score1 >= 7 || score2 >= 7) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Game Over!");

    lcd.setCursor(0, 1);
    if (score1 >= 7) {
      lcd.print("Player 1 wins!");
    } else {
      lcd.print("Player 2 wins!");
    }

    while (true) {
      // Do nothing, the game has ended
    }
  }
}

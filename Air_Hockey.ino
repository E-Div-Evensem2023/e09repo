#include <LiquidCrystal_I2C.h>

// Pin Definitions
#define LEFT_BUTTON_PIN   2
#define RIGHT_BUTTON_PIN  3
#define IR_SENSOR_1_PIN   4
#define IR_SENSOR_2_PIN   5
#define MOTOR_PIN1 8
#define MOTOR_PIN2 9

// Constants
#define WINNING_SCORE 5

// Global Variables
int strikerScore = 0;
int defenderScore = 0;

// Initialize LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address may vary, use LCD's address

void setup() {
  // Initialize Serial
  Serial.begin(9600);

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.print("Striker: 0   ");
  lcd.setCursor(0, 1);
  lcd.print("Defender: 0   ");

  // Initialize Pins
  pinMode(LEFT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(IR_SENSOR_1_PIN, INPUT);
  pinMode(IR_SENSOR_2_PIN, INPUT);
  pinMode(MOTOR_PIN1, OUTPUT);
  pinMode(MOTOR_PIN2, OUTPUT);
}

void loop() {
  // Read button states
  bool isLeftButtonPressed = digitalRead(LEFT_BUTTON_PIN) == LOW;
  bool isRightButtonPressed = digitalRead(RIGHT_BUTTON_PIN) == LOW;

  // Move motor based on button states
  if (isLeftButtonPressed) {
    moveMotorLeft();
  } else if (isRightButtonPressed) {
    moveMotorRight();
  } else {
    stopMotor();
  }

  // Check if ball passes sensors
  bool isBallPassedSensor1 = digitalRead(IR_SENSOR_1_PIN) == HIGH;
  bool isBallPassedSensor2 = digitalRead(IR_SENSOR_2_PIN) == HIGH;

  // Update scores and display
  if (isBallPassedSensor1 && isBallPassedSensor2) {
    strikerScore++;
    updateScoreDisplay(strikerScore, defenderScore);
  } else if (isBallPassedSensor1) {
    defenderScore++;
    updateScoreDisplay(strikerScore, defenderScore);
  }

  // Check for game over condition
  if (strikerScore >= WINNING_SCORE || defenderScore >= WINNING_SCORE) {
    displayWinner();
    while (true) {
      // Stay in an infinite loop to prevent further execution
    }
  }

  delay(100);  // Delay for stability
}

void moveMotorLeft() {
  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);
  Serial.println("Moving motor left");
}

void moveMotorRight() {
  digitalWrite(9, HIGH);
  digitalWrite(8, LOW);
  Serial.println("Moving motor right");
}

void stopMotor() {
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
  Serial.println("Stopping motor");
}

void updateScoreDisplay(int strikerScore, int defenderScore) {
  lcd.setCursor(9, 0);
  lcd.print(strikerScore);
  lcd.setCursor(9, 1);
  lcd.print(defenderScore);
}

void displayWinner() {
  lcd.clear();
  if (strikerScore >= WINNING_SCORE) {
    lcd.print("Striker wins!");
  } else {
    lcd.print("Defender wins!");
  }
}

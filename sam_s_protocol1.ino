#include <LiquidCrystal.h>

// LCD (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 4, 3, 8, 13);

// Sensors
int trigPin = 9;
int echoPin = 10;
int pirPin = 2;

// LEDs
int greenLED = 5;
int yellowLED = 6;
int redLED = 7;

// Buzzer (A0)
int buzzer = A0;

// Variables
long duration;
int distance;
int peopleCount = 0;
bool pirLast = LOW;

void setup() {
  lcd.begin(16, 2);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pirPin, INPUT);

  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);
}

// Distance function
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long dur = pulseIn(echoPin, HIGH, 30000);
  if (dur == 0) return -1;

  return dur * 0.034 / 2;
}

// High-frequency beep
void highBeep(int duration) {
  tone(buzzer, 3000);  // HIGH PITCH 🔥
  delay(duration);
  noTone(buzzer);
  delay(100);
}

void loop() {

  // PIR count (stable)
  bool pirNow = digitalRead(pirPin);
  if (pirNow == HIGH && pirLast == LOW) {
    peopleCount++;
    delay(500);
  }
  pirLast = pirNow;

  // Distance
  distance = getDistance();

  // LOGIC + HIGH SOUND
  if (distance >= 16 && distance <= 83) {
    // GREEN (no sound)
    digitalWrite(greenLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, LOW);
  }

  else if (distance > 83 && distance <= 241) {
    // YELLOW (alert)
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(redLED, LOW);
    highBeep(200);
  }

  else if (distance > 241 && distance <= 300) {
    // RED (warning)
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, HIGH);
    highBeep(300);
  }

  else if (distance > 300) {
    // EMPTY 🚨 (strong alert)
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, HIGH);

    for (int i = 0; i < 5; i++) {
      highBeep(150);
    }
  }

  // LCD
  lcd.setCursor(0, 0);
  lcd.print("People:");
  lcd.print(peopleCount);
  lcd.print("   ");

  lcd.setCursor(0, 1);
  lcd.print("Food:");
  lcd.print(distance);
  lcd.print("cm   ");

  delay(300);
}
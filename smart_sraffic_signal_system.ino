#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- Pin Configuration ---
const int TRIG = 2;
const int ECHO = 3;
const int PIR = 4;

const int L1_G = 5, L1_Y = 6, L1_R = 7;
const int L2_G = 8, L2_Y = 9, L2_R = 10;

const int PED_BTN = A0;    // Active LOW
const int EMERG_BTN = A1;  // Active LOW
const int BUZZER = A2;

// --- Timings (milliseconds) ---
const int YELLOW_MS = 2000;
const int ALL_RED_MS = 800;
const int MIN_GREEN = 3000;
const int MED_ADD = 2000;
const int HIGH_ADD = 4000;
const int PIR_ACTIVE_ADD = 2000;

// --- Distance Thresholds (cm) ---
const int TH_HIGH = 25;
const int TH_MED = 50;

void setup() {
  Serial.begin(115200);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(PIR, INPUT);

  pinMode(L1_G, OUTPUT);
  pinMode(L1_Y, OUTPUT);
  pinMode(L1_R, OUTPUT);
  pinMode(L2_G, OUTPUT);
  pinMode(L2_Y, OUTPUT);
  pinMode(L2_R, OUTPUT);

  pinMode(PED_BTN, INPUT_PULLUP);
  pinMode(EMERG_BTN, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Traffic");
  lcd.setCursor(0, 1);
  lcd.print("System Ready");
  delay(1500);
  allRed();
}

void loop() {
  if (isEmergency()) {
    handleEmergency();
    return;
  }

  long dist = getDistanceCM();
  int pirActive = digitalRead(PIR);

  int l1Green = greenTimeFromDistance(dist);
  int l2Green = greenTimeFromPIR(pirActive);

  if (l1Green >= l2Green) {
    serveLane(1, l1Green, dist, pirActive);
    serveLane(2, l2Green, dist, pirActive);
  } else {
    serveLane(2, l2Green, dist, pirActive);
    serveLane(1, l1Green, dist, pirActive);
  }
}

// ------------------- Functions -------------------

void serveLane(int lane, int greenMs, long distCm, int pirActive) {
  if (isPedestrian()) pedestrianPhase();

  showLaneOnLCD(lane, "GREEN", distCm, pirActive, greenMs);
  setLane(lane, 'G');
  beep(1);
  delay(greenMs);

  setLane(lane, 'Y');
  lcdSecond("YELLOW ...");
  delay(YELLOW_MS);

  allRed();
  lcdSecond("ALL RED");
  delay(ALL_RED_MS);
}

bool isPedestrian() {
  return digitalRead(PED_BTN) == LOW;
}
bool isEmergency() {
  return digitalRead(EMERG_BTN) == LOW;
}

void pedestrianPhase() {
  allRed();
  lcd.clear();
  lcd.print("PEDESTRIAN");
  lcd.setCursor(0, 1);
  lcd.print("CROSS NOW 5s");
  beep(3);
  countdown(5);
  lcd.clear();
}

void handleEmergency() {
  lcd.clear();
  lcd.print("EMERGENCY MODE");
  lcd.setCursor(0, 1);
  lcd.print("L1 PRIORITY");
  beep(3);

  allRed();
  delay(ALL_RED_MS);
  setLane(1, 'G');
  lcdSecond("L1 GREEN (EMERG)");
  delay(7000);

  setLane(1, 'Y');
  delay(YELLOW_MS);
  allRed();
  delay(ALL_RED_MS);
}

int greenTimeFromDistance(long d) {
  if (d <= 0 || d > 400) d = 400;
  if (d <= TH_HIGH) return MIN_GREEN + HIGH_ADD;
  if (d <= TH_MED) return MIN_GREEN + MED_ADD;
  return MIN_GREEN;
}

int greenTimeFromPIR(int pir) {
  return pir ? (MIN_GREEN + PIR_ACTIVE_ADD) : MIN_GREEN;
}

long getDistanceCM() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH, 30000UL);
  long cm = duration ? duration / 58 : 400;
  Serial.print("Distance: ");
  Serial.print(cm);
  Serial.println(" cm");
  return cm;
}

void setLane(int lane, char color) {
  if (lane == 1) {
    digitalWrite(L1_G, color == 'G');
    digitalWrite(L1_Y, color == 'Y');
    digitalWrite(L1_R, color == 'R');
    if (color != 'R') {
      digitalWrite(L2_G, LOW);
      digitalWrite(L2_Y, LOW);
      digitalWrite(L2_R, HIGH);
    }
  } else {
    digitalWrite(L2_G, color == 'G');
    digitalWrite(L2_Y, color == 'Y');
    digitalWrite(L2_R, color == 'R');
    if (color != 'R') {
      digitalWrite(L1_G, LOW);
      digitalWrite(L1_Y, LOW);
      digitalWrite(L1_R, HIGH);
    }
  }
}

void allRed() {
  digitalWrite(L1_G, LOW);
  digitalWrite(L1_Y, LOW);
  digitalWrite(L1_R, HIGH);
  digitalWrite(L2_G, LOW);
  digitalWrite(L2_Y, LOW);
  digitalWrite(L2_R, HIGH);
}

void beep(int n) {
  for (int i = 0; i < n; i++) {
    tone(BUZZER, 1200, 180);
    delay(230);
  }
}

void countdown(int secs) {
  for (int s = secs; s > 0; --s) {
    lcd.setCursor(12, 1);
    lcd.print("  ");
    lcd.setCursor(12, 1);
    lcd.print(s);
    tone(BUZZER, 800, 120);
    delay(1000);
  }
}

void showLaneOnLCD(int lane, const char* state, long distCm, int pirActive, int greenMs) {
  lcd.clear();
  lcd.print("Lane ");
  lcd.print(lane);
  lcd.print(": ");
  lcd.print(state);
  lcd.setCursor(0, 1);
  if (lane == 1) {
    lcd.print("Q~");
    lcd.print(distCm);
    lcd.print("cm  ");
  } else {
    lcd.print("PIR:");
    lcd.print(pirActive ? "ACT " : "IDLE");
  }
  lcd.print("T=");
  lcd.print(greenMs / 1000);
  lcd.print("s");
}

void lcdSecond(const char* msg) {
  lcd.setCursor(0, 1);
  lcd.print(msg);
  for (int i = strlen(msg); i < 16; ++i) lcd.print(' ');
}

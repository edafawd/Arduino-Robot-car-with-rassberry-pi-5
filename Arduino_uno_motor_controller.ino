// Elegoo Smart Car Shield V1.1 — left/right motors mapped correctly

#define PIN_Motor_STBY  3   // Standby/enable
#define PIN_Motor_AIN_1 7   // Motor A dir
#define PIN_Motor_PWMA  5   // Motor A PWM (left side: M1 + M4)
#define PIN_Motor_BIN_1 8   // Motor B dir
#define PIN_Motor_PWMB  6   // Motor B PWM (right side: M2 + M3)

void setup() {
  Serial.begin(9600);

  pinMode(PIN_Motor_STBY, OUTPUT);
  pinMode(PIN_Motor_AIN_1, OUTPUT);
  pinMode(PIN_Motor_PWMA,  OUTPUT);
  pinMode(PIN_Motor_BIN_1, OUTPUT);
  pinMode(PIN_Motor_PWMB,  OUTPUT);
}

void stopMotors() {
  digitalWrite(PIN_Motor_STBY, LOW);
  analogWrite(PIN_Motor_PWMA, 0);
  analogWrite(PIN_Motor_PWMB, 0);
}

void forward() {
  digitalWrite(PIN_Motor_STBY, HIGH);

  digitalWrite(PIN_Motor_AIN_1, HIGH);   // M1 + M4 forward
  analogWrite(PIN_Motor_PWMA, 180);     // left speed

  digitalWrite(PIN_Motor_BIN_1, HIGH);   // M2 + M3 forward
  analogWrite(PIN_Motor_PWMB, 180);     // right speed
}

void leftTurn() {
  digitalWrite(PIN_Motor_STBY, HIGH);

  digitalWrite(PIN_Motor_AIN_1, LOW);    // M1 + M4 reverse
  analogWrite(PIN_Motor_PWMA, 150);

  digitalWrite(PIN_Motor_BIN_1, HIGH);   // M2 + M3 forward
  analogWrite(PIN_Motor_PWMB, 150);
}

void rightTurn() {
  digitalWrite(PIN_Motor_STBY, HIGH);

  digitalWrite(PIN_Motor_AIN_1, HIGH);   // M1 + M4 forward
  analogWrite(PIN_Motor_PWMA, 150);

  digitalWrite(PIN_Motor_BIN_1, LOW);    // M2 + M3 reverse
  analogWrite(PIN_Motor_PWMB, 150);
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    if      (cmd == 'F') forward();
    else if (cmd == 'L') leftTurn();
    else if (cmd == 'R') rightTurn();
    else if (cmd == 'S') stopMotors();
  }
}

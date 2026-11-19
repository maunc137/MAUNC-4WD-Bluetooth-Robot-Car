
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// ================= MOTOR KIRI =================
#define IN1 25
#define IN2 27
#define ENA 18

// ================= MOTOR KANAN =================
#define IN3 26
#define IN4 33
#define ENB 19

// ================= SENSOR ULTRASONIK =================
#define TRIG_PIN 22
#define ECHO_PIN 23

#define JARAK_REM 30
#define BATAS_JARAK 20

// ================= LAMPU =================
#define PIN_LAMPU 15
bool statusLampu = false;

// ================= KLAKSON =================
#define PIN_KLAKSON 32
unsigned long waktuKlakson = 0;
const unsigned long DURASI_KLAKSON = 200;

// ================= KECEPATAN =================
#define MOTOR_SPEED 200
#define BRAKE_SPEED 200

char command;

// ================= KECEPATAN MOTOR =================
void setSpeedMotor() {
  analogWrite(ENA, MOTOR_SPEED);
  analogWrite(ENB, MOTOR_SPEED);
}

// ================= BERHENTI =================
void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// ================= REM OTOMATIS =================
void brakeCar() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, BRAKE_SPEED);
  analogWrite(ENB, BRAKE_SPEED);
}

// ================= MAJU =================
void forward() {
  float jarak = bacaJarak();

  if (jarak <= BATAS_JARAK) {
    stopCar();
    Serial.println("OBJEK TERLALU DEKAT!");
    return;
  }

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  setSpeedMotor();
}

// ================= MUNDUR =================
void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  setSpeedMotor();
}

// ================= BELOK KIRI =================
void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  setSpeedMotor();
}

// ================= BELOK KANAN =================
void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  setSpeedMotor();
}

// ================= BACA JARAK =================
float bacaJarak() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long durasi = pulseIn(ECHO_PIN, HIGH, 25000);

  if (durasi == 0) {
    return 999;
  }

  return durasi * 0.0343 / 2;
}

// ================= LAMPU =================
void toggleLampu() {
  statusLampu = !statusLampu;
  digitalWrite(PIN_LAMPU, statusLampu);

  Serial.println(statusLampu ? "LAMPU MENYALA" : "LAMPU MATI");
}

// ================= KLAKSON =================
void aktifkanKlakson() {
  digitalWrite(PIN_KLAKSON, HIGH);
  waktuKlakson = millis();
}

// ================= PERINTAH BLUETOOTH =================
void executeCommand(char command) {
  switch (command) {

    case 'F':
      forward();
      Serial.println("FORWARD");
      break;

    case 'B':
      backward();
      Serial.println("BACKWARD");
      break;

    case 'L':
      turnLeft();
      Serial.println("LEFT");
      break;

    case 'R':
      turnRight();
      Serial.println("RIGHT");
      break;

    case 'S':
      stopCar();
      Serial.println("STOP");
      break;

    case '0':
      toggleLampu();
      break;

    case 'X':
      aktifkanKlakson();
      Serial.println("KLAKSON ON");
      break;

    case 'C':
    case 'T':
    case 'A':
    case 'P':
      stopCar();
      break;

    default:
      break;
  }
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(PIN_LAMPU, OUTPUT);
  pinMode(PIN_KLAKSON, OUTPUT);

  digitalWrite(PIN_LAMPU, LOW);
  digitalWrite(PIN_KLAKSON, LOW);

  stopCar();

  SerialBT.begin("MAUNC_Robot_2");

  Serial.println("Bluetooth Ready!");
  Serial.println("Pair with MAUNC_Robot_2");
}

// ================= LOOP =================
void loop() {

  // Membaca perintah Bluetooth
  if (SerialBT.available()) {
    command = SerialBT.read();

    Serial.print("Received: ");
    Serial.println(command);

    executeCommand(command);
  }

  // Mematikan klakson setelah tidak menerima X
  if (millis() - waktuKlakson >= DURASI_KLAKSON) {
    digitalWrite(PIN_KLAKSON, LOW);
  }

  // Pemeriksaan sensor ultrasonik
  static unsigned long waktuSensor = 0;

  if (millis() - waktuSensor >= 50) {
    waktuSensor = millis();

    float jarak = bacaJarak();

    bool sedangMaju =
      digitalRead(IN1) == HIGH &&
      digitalRead(IN2) == LOW &&
      digitalRead(IN3) == HIGH &&
      digitalRead(IN4) == LOW;

    if (sedangMaju && jarak <= JARAK_REM) {
      brakeCar();
      Serial.println("REM OTOMATIS AKTIF!");
    }
  }
}

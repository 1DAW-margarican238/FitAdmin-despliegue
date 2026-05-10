#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_PN532.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

#define SERVO_PIN 32
Servo miServo;

const int ledPin = 4;

uint8_t chapitaUID[] = {0x7E, 0xC3, 0x0C, 0x06};
uint8_t uidLength = 4;

const char* ssid      = "DIGIFIBRA-28AF";
const char* password  = "8=jAF7gP4f";
const char* serverUrl = "http://192.168.1.236:3001/api/accesos";
const char* apiKey    = "fitadmin_torno_2026";

void conectarWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando WiFi");
  int intentos = 0;
  while (WiFi.status() != WL_CONNECTED && intentos < 20) {
    delay(500);
    Serial.print(".");
    intentos++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi conectado: " + WiFi.localIP().toString());
  } else {
    Serial.println("\nSin WiFi — modo local activo");
  }
}

void registrarAcceso(String uid) {
  if (WiFi.status() != WL_CONNECTED) return;
  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-API-Key", apiKey);
  http.setTimeout(2000);
  http.POST("{\"uid\":\"" + uid + "\"}");
  http.end();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando PN532...");

  Wire.begin(SDA_PIN, SCL_PIN);

  miServo.attach(SERVO_PIN);
  miServo.write(0);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  conectarWiFi();

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("No se encontró el PN532");
    while (1);
  }

  Serial.println("PN532 detectado correctamente");
  nfc.SAMConfig();
  Serial.println("Acerca la chapita NFC al lector...");
}

void loop() {
  uint8_t uid[7];
  uint8_t uidLengthRead;

  bool success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLengthRead);

  if (success) {
    String uidStr = "";
    for (uint8_t i = 0; i < uidLengthRead; i++) {
      if (uid[i] < 0x10) uidStr += "0";
      uidStr += String(uid[i], HEX);
    }
    Serial.println("UID detectado: " + uidStr);

    bool autorizada = (uidLengthRead == uidLength);
    for (uint8_t i = 0; i < uidLengthRead; i++)
      if (uid[i] != chapitaUID[i]) autorizada = false;

    if (autorizada) {
      Serial.println("Chapita autorizada! Abriendo servo...");
      miServo.write(90);
      digitalWrite(ledPin, LOW);
      delay(3000);
      miServo.write(0);
      digitalWrite(ledPin, HIGH);
      registrarAcceso(uidStr);
      delay(1000);
    } else {
      Serial.println("Chapita NO autorizada");
    }
  }
}
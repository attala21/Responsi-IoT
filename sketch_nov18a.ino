// Konfigurasi Blynk
#define BLYNK_TEMPLATE_ID   "TMPL6QGWgb5jL"
#define BLYNK_TEMPLATE_NAME "New Template"
#define BLYNK_AUTH_TOKEN    "W0xRFALShehrhkJpfoXVPKlNXmOI_lKO" 

// Library
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

// Konfigurasi WiFi
const char* ssid     = "Kost Bapak Munid";
const char* password = "candaceria";

// Konfigurasi Pin & Perangkat
#define SS_PIN  5
#define RST_PIN 2
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() {
  Serial.begin(115200);
  delay(1000);
 
  // Inisialisasi LCD & RFID
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();
  SPI.begin();
  mfrc522.PCD_Init();

  lcd.setCursor(0, 0);
  lcd.print("Connect Blynk...");
  
  // Koneksi ke WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Koneksi ke Server Blynk
  Blynk.config(BLYNK_AUTH_TOKEN);
  if (Blynk.connect()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Blynk Connected!");
    delay(1500);
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Blynk Gagal!");
    delay(1500);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Silakan Tap Kartu");
}

void loop() {
  // Menjaga koneksi Blynk tetap berjalan
  Blynk.run();

  // Cek keberadaan kartu RFID
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  // Membaca UID kartu
  String uidString = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) uidString += "0";
    uidString += String(mfrc522.uid.uidByte[i], HEX);
  }
  uidString.toUpperCase();

  // Tampilkan UID ke Serial Monitor & LCD
  Serial.print("Kartu: ");
  Serial.println(uidString);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ID Kartu:");
  lcd.setCursor(0, 1);
  lcd.print(uidString);

  // Mengirim UID ke Dashboard Blynk (Virtual Pin V0)
  Blynk.virtualWrite(V0, uidString);

  // Menghentikan pembacaan sementara
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  
  delay(3000);  
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Silakan Tap Kartu");
}

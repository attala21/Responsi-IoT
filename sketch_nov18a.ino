#define BLYNK_TEMPLATE_ID   "TMPL6QGWgb5jL"
#define BLYNK_TEMPLATE_NAME "New Template"
#define BLYNK_AUTH_TOKEN    "W0xRFALShehrhkjpfoXVPK1NXmOI_1K0"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

const char* ssid     = "RSML_IoT";
const char* password = "rsmliot2025";

#define SS_PIN  5
#define RST_PIN 2
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() {
  Serial.begin(115200);
  delay(1000);
 
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();
  
  SPI.begin();
  mfrc522.PCD_Init();

  lcd.setCursor(0, 0);
  lcd.print("Connect Blynk...");
  
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Blynk Connected!");
  delay(1500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Silakan Tap Kartu");
}

void loop() {
  Blynk.run();

  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  String uidString = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) uidString += "0";
    uidString += String(mfrc522.uid.uidByte[i], HEX);
  }
  uidString.toUpperCase();

  Serial.print("Kartu: ");
  Serial.println(uidString);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ID Kartu:");
  lcd.setCursor(0, 1);
  lcd.print(uidString);

  Blynk.virtualWrite(V0, uidString);

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  
  delay(3000);  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Silakan Tap Kartu");
}
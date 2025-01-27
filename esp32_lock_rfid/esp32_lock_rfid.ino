#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>  // Biblioteka do obsługi LCD I2C

#define RST_PIN 2      // Pin RST do ESP32 (zostaje ten sam)
#define SS_PIN 5       // Pin SDA do ESP32 (zostaje ten sam)
#define GPIO_OUTPUT 26 // Pin wyjściowy sterujący zamkiem

MFRC522 rfid(SS_PIN, RST_PIN);

// UID autoryzowanej karty
byte authorizedUID[] = {0x33, 0x9A, 0xC5, 0x27};

// Inicjalizacja LCD (adres 0x27 to często używany adres I2C dla wyświetlaczy 16x2)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200); // Inicjalizacja portu szeregowego
  SPI.begin();          // Inicjalizacja SPI
  rfid.PCD_Init();      // Inicjalizacja czytnika RFID

  // Ustawienie GPIO26 jako wyjście
  pinMode(GPIO_OUTPUT, OUTPUT);
  pinMode(33, OUTPUT);
  digitalWrite(GPIO_OUTPUT, LOW); // Domyślnie zamek zamknięty
  digitalWrite(GPIO_OUTPUT, LOW); // Domyślnie zamek zamknięty

  // Inicjalizacja I2C na nowych pinach SDA (GPIO21) i SCL (GPIO22) dla LCD
  Wire.begin(21, 22);  // Używamy GPIO21 (SDA) i GPIO22 (SCL) dla LCD
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.print("System zamka");  // Pierwszy wiersz
  lcd.setCursor(0, 1);       // Drugi wiersz
  lcd.print("Przyloz karte...");
  delay(2000);  // Opóźnienie, aby użytkownik zdążył zobaczyć komunikat

  Serial.println("System zamka RFID uruchomiony. Przyłóż kartę...");
}

void loop() {
  // Sprawdzenie, czy karta jest obecna
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return; // Nic nie robimy, jeśli nie ma karty
  }

  // UID karty
  Serial.print("UID karty: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();

  // Zaktualizowanie wyświetlacza LCD
  lcd.clear();
  lcd.print("UID karty: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    lcd.print(rfid.uid.uidByte[i], HEX);
    lcd.print(" ");
  }

  // Sprawdzenie, czy UID jest zgodne z autoryzowanym
  if (isAuthorized(rfid.uid.uidByte, rfid.uid.size)) {
    Serial.println("Karta autoryzowana. Otwieranie zamka...");
    lcd.clear();
    lcd.print("Karta autoryzowana");
    lcd.setCursor(0, 1);
    lcd.print("Otwieranie zamka...");
    digitalWrite(GPIO_OUTPUT, HIGH); // Otwórz zamek
    digitalWrite(33, HIGH); // Otwórz zamek
    delay(3000);                     // Czekaj 3 sekundy
    digitalWrite(GPIO_OUTPUT, LOW);  // Zamknij zamek
    digitalWrite(33, LOW);  // Zamknij zamek
    lcd.clear();
    lcd.print("Zamek zamkniety.");
    Serial.println("Zamek zamkniety.");
  } else {
    Serial.println("Nieautoryzowana karta.");
    lcd.clear();
    lcd.print("Nieautoryzowana");
    lcd.setCursor(0, 1);
    lcd.print("Karta!");
  }

  // Zatrzymanie komunikacji z kartą i zwolnienie zasobów
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

// Funkcja sprawdzająca, czy UID jest zgodne z autoryzowanym
bool isAuthorized(byte *uid, byte length) {
  if (length != sizeof(authorizedUID)) {
    return false; // Długość UID niezgodna
  }
  for (byte i = 0; i < length; i++) {
    if (uid[i] != authorizedUID[i]) {
      return false; // Bajty UID niezgodne
    }
  }
  return true; // UID zgodne
}

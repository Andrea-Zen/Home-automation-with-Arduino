#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
#include <SD.h> // Libreria per modulo SD

// Costanti per i pin
#define DHTPIN 2
#define DHTTYPE DHT11
#define RELAY_PIN_LIGHT 8
#define RELAY_PIN_FAN 12
#define PIR_PIN 7
#define RFID_SS_PIN 10
#define RFID_RST_PIN 9
#define LDR_PIN A0
#define TEMP_THRESHOLD 25
#define LIGHT_THRESHOLD 300
#define SD_CS_PIN 4 // Pin per modulo SD

// Oggetti e variabili globali
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 mfrc522(RFID_SS_PIN, RFID_RST_PIN);
SoftwareSerial BT(3, 4); // Bluetooth su RX=3, TX=4
File dataFile;

float temperature, humidity;
bool motionDetected = false;
int lightLevel;
bool authorizedUser = false;
unsigned long lastDataSave = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.begin();
  lcd.backlight();
  SPI.begin();
  mfrc522.PCD_Init();
  BT.begin(9600);
  
  pinMode(RELAY_PIN_LIGHT, OUTPUT);
  pinMode(RELAY_PIN_FAN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("Errore inizializzazione SD");
  } else {
    Serial.println("SD inizializzata");
  }

  lcd.setCursor(0, 0);
  lcd.print("Smart Home System");
  delay(2000);
  lcd.clear();
}

void loop() {
  readTemperatureHumidity();
  checkMotion();
  checkLightLevel();
  checkRFID();
  controlRelays();
  displayData();
  bluetoothControl();
  saveDataToSD();
  delay(1000);
}

// Funzione per leggere temperatura e umidità
void readTemperatureHumidity() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Errore di lettura DHT");
    return;
  }
  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.print(" C, Umidita': ");
  Serial.print(humidity);
  Serial.println(" %");
}

// Funzione per rilevare il movimento
void checkMotion() {
  motionDetected = digitalRead(PIR_PIN);
  if (motionDetected) {
    Serial.println("Movimento rilevato!");
  } else {
    Serial.println("Nessun movimento");
  }
}

// Funzione per misurare la luminosità
void checkLightLevel() {
  lightLevel = analogRead(LDR_PIN);
  Serial.print("Luminosità: ");
  Serial.println(lightLevel);
}

// Funzione per controllare i relè per luce e ventola
void controlRelays() {
  if (temperature > TEMP_THRESHOLD) {
    digitalWrite(RELAY_PIN_FAN, HIGH);  // Accendi ventola
    Serial.println("Ventola accesa");
  } else {
    digitalWrite(RELAY_PIN_FAN, LOW);   // Spegni ventola
    Serial.println("Ventola spenta");
  }
  
  if (motionDetected && lightLevel < LIGHT_THRESHOLD) {
    digitalWrite(RELAY_PIN_LIGHT, HIGH); // Accendi luce
    Serial.println("Luce accesa per movimento");
  } else {
    digitalWrite(RELAY_PIN_LIGHT, LOW);  // Spegni luce
    Serial.println("Luce spenta");
  }
}

// Funzione per riconoscimento utente tramite RFID
void checkRFID() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) return;
  Serial.print("RFID UID:");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
  authorizedUser = true;
  mfrc522.PICC_HaltA();
}

// Funzione di controllo tramite Bluetooth
void bluetoothControl() {
  if (BT.available()) {
    char command = BT.read();
    if (command == '1') {
      digitalWrite(RELAY_PIN_LIGHT, HIGH);
      Serial.println("Luce accesa via Bluetooth");
    } else if (command == '0') {
      digitalWrite(RELAY_PIN_LIGHT, LOW);
      Serial.println("Luce spenta via Bluetooth");
    } else if (command == '2') {
      digitalWrite(RELAY_PIN_FAN, HIGH);
      Serial.println("Ventola accesa via Bluetooth");
    } else if (command == '3') {
      digitalWrite(RELAY_PIN_FAN, LOW);
      Serial.println("Ventola spenta via Bluetooth");
    }
  }
}

// Funzione per visualizzare i dati su LCD
void displayData() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C ");
  lcd.setCursor(0, 1);
  lcd.print("Umid: ");
  lcd.print(humidity);
  lcd.print("%");

  if (motionDetected) {
    lcd.setCursor(10, 0);
    lcd.print("Mov");
  }
  
  delay(2000);
  if (authorizedUser) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Utente Riconosciuto");
    delay(2000);
  }
}

// Funzione per salvare dati su SD ogni minuto
void saveDataToSD() {
  if (millis() - lastDataSave >= 60000) { // Salva ogni minuto
    dataFile = SD.open("datalog.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.print("Temperatura: ");
      dataFile.print(temperature);
      dataFile.print(" C, Umidita': ");
      dataFile.print(humidity);
      dataFile.print(" %, Luminosità: ");
      dataFile.print(lightLevel);
      dataFile.print(", Movimento: ");
      dataFile.println(motionDetected ? "Si" : "No");
      dataFile.close();
      Serial.println("Dati salvati su SD");
      lastDataSave = millis();
    } else {
      Serial.println("Errore apertura file SD");
    }
  }
}

// Funzione di log per eventi significativi
void logEvent(const String &event) {
  dataFile = SD.open("eventlog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println(event);
    dataFile.close();
    Serial.println("Evento salvato: " + event);
  } else {
    Serial.println("Errore apertura file SD per log");
  }
}

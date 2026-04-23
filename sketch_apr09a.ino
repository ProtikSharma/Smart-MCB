#include <LiquidCrystal.h>
#include <DHT.h>

#define ACS_PIN A0
#define RELAY_PIN 8

#define LED_GREEN A1
#define LED_YELLOW A2
#define LED_RED A3

#define DHTPIN 2
#define DHTTYPE DHT11

LiquidCrystal lcd(9, 10, 11, 12, 13, 4);
DHT dht(DHTPIN, DHTTYPE);


float TRIP_CURRENT = 0.50;    
float WARNING_CURRENT = 0.30; 
float TRIP_TEMP = 60.0;       


float sensitivity = 0.066;    
float offsetVoltage = 2.5;    

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  digitalWrite(RELAY_PIN, HIGH); 

  lcd.begin(16, 2);
  dht.begin();

  lcd.clear();
  lcd.print(" Smart MCB ");
  lcd.setCursor(0, 1);
  lcd.print(" Initializing ");
  delay(2000);
}

void loop() {
  /
  int raw = analogRead(ACS_PIN);
  float voltage = (raw * 5.0) / 1023.0;
  float current = abs((voltage - offsetVoltage) / sensitivity);


  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("I:");
  lcd.print(current, 2);
  lcd.print("A ");

  lcd.print("T:");
  lcd.print(temp, 1);
  lcd.print("C");

  // -------- LOGIC --------
  if (current >= TRIP_CURRENT || temp >= TRIP_TEMP) {
    // TRIP CONDITION
    digitalWrite(RELAY_PIN, LOW);   // Relay OFF
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, HIGH);

    lcd.setCursor(0, 1);
    lcd.print("** MCB TRIPPED **");
  }
  else if (current >= WARNING_CURRENT) {
    
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_RED, LOW);

    lcd.setCursor(0, 1);
    lcd.print(" Warning Level ");
  }
  else {
    
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, LOW);

    lcd.setCursor(0, 1);
    lcd.print(" Normal Load ");
  }

  delay(800);
}

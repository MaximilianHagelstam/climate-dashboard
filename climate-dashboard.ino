#include <MQ2.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <dht.h>

LiquidCrystal_I2C lcd(0x3f, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

#define dht_input A0
dht DHT;

int mq2_input = A1;
int lpg, co, smoke;
MQ2 mq2(mq2_input);

int photo_input = A3;
int brightness = 0;

int moisture_input = A2; 
int moisture;

#define CLK 2
#define DT 3
#define SW 4

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;

void setup()
{
  lcd.begin(16, 2);
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Choose coffee");
  lcd.setCursor(0,1);  
  lcd.print(" Sugar level");

  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  pinMode(SW, INPUT_PULLUP);

  Serial.begin(9600);

  mq2.begin();

  lastStateCLK = digitalRead(CLK);
}

void loop()
{
  DHT.read11(dht_input);
  
  int hum = DHT.humidity;
  int temp = DHT.temperature;
  
  lpg = mq2.readLPG();
  co = mq2.readCO();
  smoke = mq2.readSmoke();

  brightness = analogRead(photo_input);

  moisture = analogRead(moisture_input);
  
  currentStateCLK = digitalRead(CLK);

  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1)
  {
    if (digitalRead(DT) != currentStateCLK)
    {
      counter --;
      currentDir ="CCW";
    }
    
    else
    {
      counter ++;
      currentDir ="CW";
    }

    Serial.print("Direction: ");
    Serial.print(currentDir);
    Serial.print(" | Counter: ");
    Serial.println(counter);
  }
  
  lastStateCLK = currentStateCLK;
  int btnState = digitalRead(SW);
  
  if (btnState == LOW)
  {
    if (millis() - lastButtonPress > 50)
    {
      int counter = 0;
    }
    
    lastButtonPress = millis();
  }
  delay(1);

  if (counter == 0)
  {
    lcd.setCursor(0, 0);
    lcd.print("Welcome to your");

    lcd.setCursor(0, 1);
    lcd.print("Enviormental dashboard");
  }

  if (counter == 1 || -1)
  {
    lcd.setCursor(0, 0);
    lcd.print("Temperature: ");
    lcd.print(temp);
    lcd.print("°C");

    lcd.setCursor(0, 1);
    lcd.print("Recommended: 23°C");
  }

  if (counter == 2 || -2)
  {
    lcd.setCursor(0, 0);
    lcd.print("Humidity: ");
    lcd.print(hum);
    lcd.print("%");

    lcd.setCursor(0, 1);
    lcd.print("Recommended: 45%");
  }

  if (counter == 3 || -3)
  {
    lcd.setCursor(0, 0);
    lcd.print("CO2: ");
    lcd.print(co);
    lcd.print("ppm");

    lcd.setCursor(0, 1);
    lcd.print("Recommended: 500ppm");
  }

  if (counter == 4 || -4)
  {
    lcd.setCursor(0, 0);
    lcd.print("LPG: ");
    lcd.print(lpg);
    lcd.print("ppm");

    lcd.setCursor(0, 1);
    lcd.print("Recommended: <100ppm");
  }

  if (counter == 5 || -5)
  {
    lcd.setCursor(0, 0);
    lcd.print("Smoke: ");
    lcd.print(smoke);
    lcd.print("ppm");

    lcd.setCursor(0, 1);
    lcd.print("Recommended: < 200ppm");
  }

  if (counter == 6 || -6)
  {
    lcd.setCursor(0, 0);
    lcd.print("Moisture: ");
    lcd.print(moisture);
    lcd.print("%");

    lcd.setCursor(0, 1);
    lcd.print("Recommended: 80%");
  }

  if (counter == 7)
  {
    lcd.setCursor(0, 0);
    lcd.print("Brightness: ");
    lcd.print(brightness);
    lcd.print("lx");

    lcd.setCursor(0, 1);
    lcd.print("Last page <--");
  }

  if (counter == -7)
  {
    lcd.setCursor(0, 0);
    lcd.print("Brightness: ");
    lcd.print(brightness);
    lcd.print("lx");

    lcd.setCursor(0, 1);
    lcd.print("Last page -->");
  }
}

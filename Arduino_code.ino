#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include<DHT.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
#include <Keypad.h>
DHT dht(A3, DHT11);

int relaybulb = 14;
//int relaybell=;
int ledgarden = 8;
int ledkitchen = 9;
int ledbedroom = 10;
int ledbathroom = 11;
int fan = 12;

byte k = 0;
int z = 0;
char key1 = ' ';
int p = 0;
int state = LOW;
int val = 0;
byte pirsensor = 38;
byte smoke = 40;
int tempsensor = A6;
const int ldrgarden = A0;
int ldrwall = A10;
int relaybuzzer = 22;

byte irgo = 24;
byte irgi = 26;
byte irbath = 2;
byte irbed = 32;
byte irgate = 34;
byte irdoor = 36;

byte mup = 4;
byte mdw = 5;
byte ml = 6;
byte mr = 7;


const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'D', 'C', 'B', 'A'},
  {'#', '9', '6', '3'},
  {'0', '8', '5', '2'},
  {'*', '7', '4', '1'}
};
byte rowPins[ROWS] = {31, 33, 35, 37}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {39, 41, 43, 45}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


String s;
char c;
int temp,humi,smo,hometemp,intru,gate=0,gar,garden;
void setup() {
  Serial.begin(9600);
   Serial1.begin(9600);
  // Serial2.begin(9600);
  pinMode(ldrwall, INPUT);
  pinMode(mup, OUTPUT);
  pinMode(mdw, OUTPUT);
  pinMode(ml, OUTPUT);
  pinMode(mr, OUTPUT);
  pinMode(ledgarden, OUTPUT);
  pinMode(ledbedroom, OUTPUT);
  pinMode(ledkitchen, OUTPUT);
  pinMode(ledbathroom, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(irgo, INPUT);
  pinMode(irgi, INPUT);
  pinMode(ldrgarden, INPUT);
  pinMode(smoke, INPUT);
  pinMode(ldrwall, INPUT);
  pinMode(pirsensor, INPUT);
  pinMode(relaybulb, OUTPUT);
  pinMode(relaybuzzer, OUTPUT);
  dht.begin();
  lcd.begin(20, 4);                             //Defining 16 columns and 2 rows of lcd display
  lcd.backlight();                              //To Power ON the back light
  Serial.println("C");
}
void humtemp()
{
    temp = dht.readTemperature();
     humi = dht.readHumidity();
}

void password()
{
  char key = keypad.getKey();
  while (key != '#')
  {
    key = keypad.getKey();
    if (key == '*')
    {
      s = "";
    }
    if (key)
    {
      s = s + key;
    }
  }
  //Serial.println(s);
  if (key == '#')
  {
    if (s.equals("5412#"))
    {
      Serial.println("open");
      s = "";
      lcd.clear();
      lcd.setCursor(0, 3);
      lcd.print(F("Gate open"));
      analogWrite(mr, 0);
      analogWrite(ml, 70);
      delay(500);
      gate=1;
      analogWrite(mr, 0);
      analogWrite(ml, 0);
      delay(3000);
      while (digitalRead(irgate) == 0)
      {
        analogWrite(mr, 0);
        analogWrite(ml, 0);
        delay(3000);
      }
      analogWrite(mr, 70);
      analogWrite(ml, 0);
      delay(500);
      return ;
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 3);
      lcd.print(F("Incorrect Pass"));
      z++;
      analogWrite(mr, 0);
      analogWrite(ml, 0);
      delay(2000);
      if (z == 3)
        intruder();
      return ;
    }
  }
}

void housetemp()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("HOME TEMP --> "));//Clean the screen
  lcd.setCursor(15, 0);
  hometemp = analogRead(tempsensor) * 0.48828125;
  lcd.print(hometemp);
}

void smokedetect()
{
   smo = digitalRead(smoke);
  if (s == 0)
  {
    lcd.clear();
    lcd.setCursor(0, 4);
    lcd.print(F("Fire Alert!!!"));
    intruder();
    delay(3000);
  }
}


void motion()
{
  digitalWrite(ledkitchen, HIGH);
  
}

void bedroom()
{ //led and fan off
  delay(1000);
  while (p != 0)
  {
    digitalWrite(ledbedroom, HIGH);
    digitalWrite(fan, HIGH);
    if (digitalRead(irbed) == 0)
      p--;
  }
  delay(1000);
}
void bath()
{
  delay(1000);
  while (p != 0)
  {
    digitalWrite(ledbathroom, HIGH);
    digitalWrite(relaybulb, HIGH);
    delay(2000);
    if (digitalRead(irbath) == 0)
      p--;
  }
  delay(1000);
}
void garage()
{
  analogWrite(mup, 0);
  analogWrite(mdw, 0);

  if (digitalRead(irgo) == 0)
  {
    analogWrite(mup, 100);
    analogWrite(mdw, 0);
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("GARAGE IS OPENING");
    delay(4000);
    gar=1;
    analogWrite(mup, 0);
    analogWrite(mdw, 0);
    
    delay(4000);
    analogWrite(mup, 0);
    analogWrite(mdw, 100);
    gar=0;
    delay(3500);

  }
  if (digitalRead(irgi) == 0)
  {
    analogWrite(mup, 100);
    analogWrite(mdw, 0);
    gar=1;
    delay(4000);
    analogWrite(mup, 0);
    analogWrite(mdw, 0);
    delay(4000);
    analogWrite(mup, 0);
    analogWrite(mdw, 100);
    gar=0;
    delay(3500);
  }
  analogWrite(mup, 0);
    analogWrite(mdw, 0);
}
void gardenon()
{
  garden=1;
  digitalWrite(ledgarden, HIGH);
}
void gardenof()
{
  digitalWrite(ledgarden, LOW);
  garden=0;
}
void intruder()
{
  digitalWrite(relaybuzzer, LOW);
  intru=1;
  delay(3000);
  
}

void writenode()
{
 String str = String(gar) + "\n" + String(gate) + "\n" + String(garden) + "\n" + String(smo) + "\n" + String(intru) + "\n" + String(humi) + "\n" + String(temp) + "\n" + String(hometemp);
    char* cString = (char*) malloc(sizeof(char) * (str.length() + 1));
    str.toCharArray(cString, str.length() + 1);
    Serial1.write(cString);
}
void loop() {
  intru=0;
  analogWrite(mr, 0);
  analogWrite(ml, 0);
  digitalWrite(relaybuzzer, HIGH);
  digitalWrite(ledkitchen, LOW);
  digitalWrite(ledgarden, LOW);
  digitalWrite(ledbathroom, HIGH);
  digitalWrite(ledbedroom, LOW);
  digitalWrite(fan, LOW);
  Serial.println("Y");
  Serial.println(analogRead(ldrgarden));
  if (keypad.getKey() == '*')
  { Serial.println("YAH");
    password();
  }
  if (digitalRead(pirsensor) == 1)
  { Serial.println("1");
    motion();
  }
  if (digitalRead(smoke) == 0)
  { Serial.println("2");
    smokedetect();
  }
  if (digitalRead(irbed) == 0)
  { Serial.println("3");
    p++;
    bedroom();
  }
  if (digitalRead(irbath) == 0)
  { Serial.println("4");
    p++;
    bath();
  }
  if ((digitalRead(irgo) == 0)||(digitalRead(irgi) == 0))
  { Serial.println("5");
    garage();
  }
  if (analogRead(ldrgarden) < 800)
  { Serial.println("6");
    gardenon();
  }
  if (analogRead(ldrgarden) > 900)
  { Serial.println("7");
    gardenof();
  }

}

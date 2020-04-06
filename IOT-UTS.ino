#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 2 
#define RST_PIN 0
#define triggerPin D0
#define echoPin D8
#define sensorLDR A0
#define LedMerah 03

int nilaiSensor;

LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 mfrc522(SS_PIN, RST_PIN); 

void setup()
{
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();  

  lcd.init(); 
  lcd.backlight();
  lcd.home();

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(LedMerah, OUTPUT);
}

void loop()
{
  
  nilaiSensor = analogRead(sensorLDR);
  Serial.print("Nilai Sensor : ");
  Serial.println(nilaiSensor);
  
  if (nilaiSensor >= 700){
    
    digitalWrite(LedMerah, HIGH);
    rfiddanlcd(); 
    
  }else{
    
    digitalWrite(LedMerah, LOW);
    rfiddanlcd();
    
  }

  delay(3000);
  
}

void rfiddanlcd()
{
  lcd.clear();
  lcd.home();
  
  scrollText(0, "TEMPELKAN KARTU ANDA!", 250, 16);
  
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("UID Card :");
  String content= "";
  byte letter;
  
  for (byte i = 0; i < mfrc522.uid.size; i++) {
 
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
 
  }
  
  Serial.println();
  content.toUpperCase();

  long duration, jarak;

  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  jarak = (duration/2) / 29.1;
  Serial.print(jarak);
  Serial.println(" cm");

  int jumlahPengunjung = 0;
  
  if (content.substring(1) == "19 50 4A 98")
  {
      scrollText(0, "KARTU ANDA TERDAFTAR! SILAHKAN MASUK!", 250, 16);
      if (jarak == 4 || jarak == 3 || jarak == 2){
        lcd.setBacklight(HIGH);
        lcd.setCursor(0,0);
        lcd.print("JUMLAH");
        lcd.setCursor(0,1);
        lcd.print("PENGUNJUNG = ");
        lcd.setCursor(13,1);
        lcd.print(jumlahPengunjung+1);
      }
      
      delay(2000);
      
  } else {
    
      scrollText(0, "KARTU TIDAK TERDAFTAR!", 250, 16);
      delay(2000);
      
  }
}

void scrollText(int row, String message, int delayTime, int lcdColumns)
{
  for (int i = 0; i < lcdColumns; i++)
  {
    
    message = " " + message;
    
  }
  
  message = message + " ";
  
  for (int pos = 0; pos < message.length(); pos++)
  {
    
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + lcdColumns));
    delay(delayTime);
    
  }
}

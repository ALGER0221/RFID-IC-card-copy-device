#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <MFRC522.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#define RST_PIN         9          
#define SS_PIN          10       
MFRC522 mfrc522(SS_PIN, RST_PIN); 
#define NEW_UID {0x00, 0x00, 0x00, 0x00}
byte ID[4];
byte newUid[4]{0x00, 0x00, 0x00, 0x00};
MFRC522::MIFARE_Key key;
int Mode_A=0,Mode_B=0;
void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();   
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  pinMode(4,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}

void loop() {
  
  if(digitalRead(4)==HIGH&&digitalRead(5)==LOW){
      Mode_B=0;
      if(Mode_A ==0){
         display.clearDisplay();
      }
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.println("Find UID");
      display.println("");
    
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      byte *id = mfrc522.uid.uidByte;  
      byte idSize = mfrc522.uid.size;   
      MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
      for (byte i = 0; i < idSize; i++) {  
        Serial.print("id[");
        Serial.print(i);
        Serial.print("]: ");
        Serial.println(id[i], HEX);       
        newUid[i]=newUid[i]+id[i];
        if(i%2==0){
           display.print("id[");
           display.print(i);
           display.print("]:0X");
           display.print(id[i], HEX);
           display.print(" ");
        }
        else{
           display.print("id[");
           display.print(i);
           display.print("]:0X");
           display.println(id[i], HEX);
        }
      }
      Serial.println();
     mfrc522.PICC_HaltA();
    } 
     Mode_A++;
     delay(100);
    }
  else if(digitalRead(4)==LOW&&digitalRead(5)==HIGH){
    Mode_A=0;
      if(Mode_B ==0){
         display.clearDisplay();
         display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.println("Copy");
      display.println("");
      }
      
      if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
    delay(50);
   return;
  }
  
  
//byte newUid[] = NEW_UID[0]+ID[0],NEW_UID[1]+ID[1],NEW_UID[2]+ID[2],NEW_UID[3]+ID[3];

//for(int i=0;i<4;i++){
 //newUid[i]=newUid[i]+ID[i];
//}
 if ( mfrc522.MIFARE_SetUid(newUid, (byte)4, true) ) {
    Serial.println(F("Finish"));
    display.println("Finish");
  }
   Mode_B++;
   delay(300);
  }

  
  else{
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.println(" By Gsx_Joker");
      display.println("                V2.0");
      display.println("    !!!Warning!!!");
      display.println("  Check before use");
      display.display();
      delay(200);
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.println(" By Gsx_Joker");
      display.println("                V2.0");
      display.println("    ");
      display.println("  Check before use");
      display.display();
      delay(200);
      Mode_B =0;
      Mode_A =0;
  }
  display.display();
}

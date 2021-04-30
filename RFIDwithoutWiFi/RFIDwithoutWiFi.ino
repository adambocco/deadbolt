#define SS_PIN D4  //D2
#define RST_PIN D3 //D1
#include <SPI.h>
#include <MFRC522.h>
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int BUZZER = D0;
int DEADBOLT = D1;


void setup() 
{
  Serial.begin(115200);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  digitalWrite(D0,LOW);
  digitalWrite(D1,LOW);
}

void unlock() {
  digitalWrite(DEADBOLT, HIGH);
  Serial.println("DOOR IS UNLOCKED");
  delay(5000);
  digitalWrite(DEADBOLT, LOW);
  Serial.begin(115200);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
}

void loop() 
{
  // Look for new cards
  Serial.println("looking for cards");
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    Serial.println("No card");
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    Serial.println("Can't read serial data");
    return;
  }
  //Show UID on serial monitor
  Serial.println();
  Serial.print(" UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  if (content.substring(1) == "B6 33 ED F8"||content.substring(1)=="09 8F 68 99"||content.substring(1)=="A9 81 AE 98"||content.substring(1)=="49 3D A3 98"||content.substring(1)== "16 17 EA F8" ||content.substring(1)=="36 D4 2A F9") //change UID of the card that you want to give access
  {
    delay(1000);
    Serial.println("Unlocking...");
    Serial.println();
    unlock();
    Serial.println("Locked.");
    } 
  }

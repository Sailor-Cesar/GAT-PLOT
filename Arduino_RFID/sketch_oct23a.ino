#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

char st[20];

void setup()
{
  Serial.begin(9600); 
  SPI.begin();
  mfrc522.PCD_Init();
}

void loop()
{
  // Procurar por cartao existente
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Selecionar um dos cartoes existentes
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : "");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : ""));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  conteudo.toUpperCase();
  Serial.println();
  delay(2000);
  

}

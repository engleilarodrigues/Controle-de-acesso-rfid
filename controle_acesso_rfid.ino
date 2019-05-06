#include <SPI.h>
#include <MFRC522.h>

/* Configuraçao de pinos utilizadas:
 * ----------------------------------
 *             MFRC522      Node     
 *             Reader/PCD   MCU      
 * Sinal       Pino          Pino     
 * ----------------------------------
 * RST/Reset   RST          D1 (GPIO5)        
 * SPI SS      SDA(SS)      D2 (GPIO4)       
 * SPI MOSI    MOSI         D7 (GPIO13)
 * SPI MISO    MISO         D6 (GPIO12)
 * SPI SCK     SCK          D5 (GPIO14)
 * 3.3V        3.3V         3.3V
 * GND         GND          GND
 */

constexpr uint8_t RST_PIN = 5;
constexpr uint8_t SS_PIN = 4;

#define MAX_SIZE_BLOCK  16
#define LEDGREEN 0 //pin D3
#define LEDRED 16 //pin D0
#define BUZZER 2 //pin D4

char idConvertida[16];
char cardAdmin1[9] = "AF69B079"; //cartão Admin. 1
char cardAdmin2[9] = "B05F18A8"; //cartão Admin. 2

MFRC522 rfid(SS_PIN, RST_PIN); 
//esse objeto 'chave' é utilizado para autenticação
MFRC522::MIFARE_Key key;

byte nuidPICC[4];

void setup() {
  Serial.begin(115200);
  SPI.begin();
  
  rfid.PCD_Init();//inicializa MFRC522
  
  pinMode(LEDRED, OUTPUT);          
  pinMode(LEDGREEN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  Serial.println("INICIANDO O SISTEMA ....");

}

void loop() {
  // verifica novos cartões
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
 
  // verifica se o UID já foi analisado anteriormente
  if ( ! rfid.PICC_ReadCardSerial())
    return;
  
  String info = "";

  for (byte i = 0; i < rfid.uid.size; i++) // Laço incremental que armazena ID na variavel "conteudo"
  {
    info.concat(String(rfid.uid.uidByte[i], HEX));
  }

  info.toUpperCase();                // Transforma caracteres em caixa alta
  info.toCharArray(idConvertida, 16);// Passa os caractere da id prrsente "conteudo", na matriz "idconvertida"

  // compara o cartão lido com os cartões previamente salvos como admins
  if ((info == cardAdmin1) or (info == cardAdmin2)){
    Serial.println("Acesso autorizado!");
    Serial.println("");
    digitalWrite(LEDGREEN, HIGH);
    delay(1000);
    digitalWrite(LEDGREEN, LOW);
    delay(1000);
  }
  else{
    Serial.println("Acesso negado!");
    Serial.println("");
    digitalWrite(BUZZER, HIGH);
    delay(1000);
    
    digitalWrite(BUZZER, LOW);
    delay(100);

    digitalWrite(LEDRED, HIGH);
    delay(1000);
    digitalWrite(LEDRED, LOW);
    delay(1000);
  }
}

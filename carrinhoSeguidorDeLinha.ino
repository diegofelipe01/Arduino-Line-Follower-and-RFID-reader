#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>

#define SS_PIN 10//pinos do RFID reader
#define RST_PIN 9

#define RightMotorForward 7
#define RightMotorBackward 6
#define LeftMotorForward 5
#define LeftMotorBackward 4
#define sensorL A0
#define sensorR A1
#define FW 55
#define TR 70
#define PAUSE 75

LiquidCrystal_I2C lcd(0x27, 16, 2);//Instancia do lcd
MFRC522 rfid(SS_PIN, RST_PIN);//Instancia do leitor rfid


//arrays usados para salvar os UIDs
short uid[5];//total de valores que vamos empilhar
short holder;
byte cardsTotal = 0;//assim que atingir 5 o robô deve parar e mostrar os dados em ordem de pilha
long currentTime;//holder para a função millis
long lastTime = 0;//vatiável para controle do tempo (última vez que o cartão foi lido)
const long interval = 5000;//o arduino só pode ler o cartão se um tempo mínimo de 5seg (5000ms) tiver passado

void setup() {
pinMode(RightMotorForward, OUTPUT);
pinMode(LeftMotorForward, OUTPUT);
pinMode(LeftMotorBackward, OUTPUT);
pinMode(RightMotorBackward, OUTPUT);
pinMode(sensorL,INPUT);
pinMode(sensorR,INPUT);
Serial.begin(9600);
SPI.begin();
rfid.PCD_Init();//Inicia o leitor rfid
//inicializando o lcd
lcd.init();
lcd.backlight();

}

void loop() {
//mensagem que e escrita no lcd
//lcd.setCursor(0,0);
//lcd.print("Ola, sou Clovis");
//lcd.setCursor(0, 1);
//lcd.print("      :D");
currentTime = millis();
if(rfid.PICC_IsNewCardPresent()){
    if(rfid.PICC_ReadCardSerial()){
      if(currentTime - lastTime >= interval){
        lastTime = currentTime;//salva a última vez que o arduino leu um cartão
      
        holder = 0;
        switch(cardsTotal){//checará quantos cartões foram preechidos e preecherá o array de acordo
          case 0:
            for(byte i = 0; i < rfid.uid.size; i++){
              holder += rfid.uid.uidByte[i];
            }
            uid[0] = holder;
            break;
          case 1:
            for(byte i = 0; i < rfid.uid.size; i++){
              holder += rfid.uid.uidByte[i];
            }
            uid[1] = holder;
          break;
          case 2:
            for(byte i = 0; i < rfid.uid.size; i++){
              holder += rfid.uid.uidByte[i];
          }
            uid[2] = holder;
          break;
          case 3:
            for(byte i = 0; i < rfid.uid.size; i++){
              holder += rfid.uid.uidByte[i];
            }
            uid[3] = holder;
          break;
          case 4:
            for(byte i = 0; i < rfid.uid.size; i++){
              holder += rfid.uid.uidByte[i];
            }
            uid[4] = holder;
          break;
          default://aviso para algo que não deve acontecer
            limpaTela();
            lcd.setCursor(0,0);
            lcd.print("Uh oh...");
            lcd.setCursor(0,1);
            lcd.print("PROGRAMADOR!");
          delay(5000);
          break;
        }
     
     cardsTotal++;//cada vez que um cartão é lido o número é incrementado
     delay(1200);
     }
     if(cardsTotal == 5){
      finale();
     }
     limpaTela();
    }
  }
int left = analogRead(sensorL);
int right = analogRead(sensorR);
Serial.print("Leitura de luminosidade sensor L: ");
Serial.println(left);
Serial.print("Leitura de luminosidade sensor R: ");
Serial.println(right);


if (left < 100 && right < 100){
moveForward();
}
else if (left > 100){
turnLeft();
}
else if (right > 100){
turnRight();
}
delay(PAUSE);
}
 
void moveForward(){
motorLForward();
motorRForward();
delay(FW);
moveStop();
}
 
void turnRight(){
Serial.println("Vira à direita");
motorLForward();
delay(TR);
moveStop();
}
 
void turnLeft(){
Serial.println("Vira à esquerda");
motorRForward();
delay(TR);
moveStop();
}
 
void moveStop(){
digitalWrite(RightMotorForward, LOW);
digitalWrite(LeftMotorForward, LOW);
digitalWrite(RightMotorBackward, LOW);
digitalWrite(LeftMotorBackward, LOW);
}
 
void motorRForward(){
digitalWrite(RightMotorForward, HIGH);
digitalWrite(RightMotorBackward, LOW);
}
 
void motorRBackward(){
digitalWrite(RightMotorForward, LOW);
digitalWrite(RightMotorBackward, HIGH);
}
 
void motorLForward(){
digitalWrite(LeftMotorForward, HIGH);
digitalWrite(LeftMotorBackward, LOW);
}
 
void motorLBackward(){
digitalWrite(LeftMotorForward, LOW);
digitalWrite(LeftMotorBackward, HIGH);
}
void finale(){//função que desempilha
  limpaTela();
    lcd.setCursor(0,0);
    lcd.print("Desempilhando:");
    lcd.setCursor(0,1);
    int j = 4;
    for(byte i = 0; i < 5; i++){//i não chega a ser menor que zero e buga a função, caso use o i para desempilhar
      lcd.print(uid[j], DEC);
      lcd.print(" ");
      Serial.println(uid[j], DEC);
      j--;
    }
    delay(12000);
}
void limpaTela(){//fução para limpar a toda a tela com um comando
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.clear();
}

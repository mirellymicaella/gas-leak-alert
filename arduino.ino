#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

#define sensor A0
#define buzzer 13
float leitura_sensor, leitura_setup, limite = 0,media;
int i = 0;
LiquidCrystal lcd(2,3,4,5,6,7);
SoftwareSerial s(0,1);

void setup() {
   pinMode(buzzer, OUTPUT);
   Serial.begin(115200);
   
   lcd.begin(16, 2);
   lcd.setCursor(0,0);
   lcd.print("Iniciando...");
   delay(60000);
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Calibrando: ");
   lcd.setCursor(0,1);
   lcd.print("Restam ");
   
   for(i = 60; i > 0; i--){
 
       leitura_setup = analogRead(sensor);
       media = media + leitura_setup; 

       lcd.setCursor(7,1);
       if(i<10){
          lcd.print(i);
          lcd.setCursor(8,1);
          lcd.print(" ");
       }else
          lcd.print(i);

       lcd.setCursor(9,1);
       lcd.println(" s...  ");
       delay(1000);
   }

   lcd.clear();
   media=media/60;
   media = media*1.5;
   limite=media;
    Serial.print("ESTE E O LIMITE >>> ");
    Serial.println(limite);
    delay(1000);
}

void loop() {

 leitura_sensor = analogRead(sensor);
 Serial.println(leitura_sensor);
 
 lcd.setCursor(0,1);
 lcd.print("Lim:");
 lcd.setCursor(4,1);
 lcd.print(limite);
 
 lcd.setCursor(9,1);
 lcd.print(" /");
 lcd.setCursor(11,1);
 lcd.print(leitura_sensor);
 
if(leitura_sensor >= limite){
    digitalWrite(buzzer, HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.println("GAS DETECTADO!");
    s.write('1');
 }
 else{
  digitalWrite(buzzer, LOW);
  lcd.setCursor(0,0);
  lcd.println("GAS AUSENTE!");
  s.write('0');
 }
 delay(1000);
}

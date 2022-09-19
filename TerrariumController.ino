#include <IRremote.h>
#include <LiquidCrystal.h>
#include <dht.h>

dht DHT;

#define DHT11 6
#define REC 8
#define R 11
#define G 10
#define B 9
#define FAN 7
#define LCDP13 13
#define LCDP12 12
#define LCDP5 5
#define LCDP4 4
#define LCDP3 3
#define LCDP2 2

int aux = 0;
int bright = 0;

// Inicializar Liquid Crystal
LiquidCrystal lcd(LCDP13, LCDP12, LCDP5, LCDP4, LCDP3, LCDP2);

// Criar objeto IRrecv e decode_results para receber e decodificar comando
IRrecv control(REC);
decode_results code;

byte Lamp[8] =
{
0b01110,
0b10001,
0b10001,
0b10001,
0b01010,
0b01010,
0b01110,
0b00100
};

byte Fan[8] =
{
0b00100,
0b00100,
0b10001,
0b01010,
0b10101,
0b01010,
0b10001,
0b00100
};

byte Flame[8] =
{
0b01000,
0b01100,
0b00110,
0b00110,
0b01001,
0b10001,
0b10101,
0b01110
};

byte Water[8] =
{
0b00000,
0b00100,
0b00100,
0b01010,
0b10001,
0b10001,
0b10001,
0b01110
};

void setup() {
  lcd.begin(16,2);
  lcd.createChar(0, Lamp);
  lcd.createChar(1, Fan);
  lcd.createChar(2, Flame);
  lcd.createChar(3, Water);
  
  lcd.setCursor(0, 0);
  lcd.write(byte(0));
  lcd.setCursor(8, 0);
  lcd.write(byte(2));
  lcd.setCursor(9, 0);
  lcd.print(": ");
  
  lcd.setCursor(0, 1);
  lcd.write(byte(1));
  lcd.setCursor(8, 1);
  lcd.write(byte(3));
  lcd.setCursor(9, 1);
  lcd.print(": ");
  
  lcd.setCursor(1, 0);
  lcd.print(": OFF");
  lcd.setCursor(1, 1);
  lcd.print(": LOW");

  control.enableIRIn(); //Inicializar conexão com o comando
  pinMode(FAN, OUTPUT);
  digitalWrite(FAN, LOW);
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Ler e exibir temperatura e humidade
  TempH();

  //Caso seja efetuado um comando
  if(control.decode(&code) && code.value != 0xFFFFFFFF){

    // Button 1
    if(code.value == 0xFF30CF){ //Turn on LED
      RGB_Collor(0, 0 ,255);
      WriteLcd(1);
      bright = 1;

    // Button 0
    } else if(code.value == 0xFF6897){ // Turn off LED
      RGB_Collor(0, 0 , 0);
      WriteLcd(6);
      bright = 0;

    // Button -
    } else if(code.value == 0xFFE01F && bright == 1){ //Reduce luminosity in 1 stage
      RGB_Collor(0, 0, 100);
      WriteLcd(2);
      bright = 2;
        
        
    } else if(code.value == 0xFFE01F && bright == 2){ //Reduce luminosity in 1+ stage
      RGB_Collor(0, 0, 30);
      WriteLcd(3);
      bright = 3;

    // Button +
    } else if(code.value == 0xFFA857 && bright == 3){ //Increase luminosity by 1 stage
      RGB_Collor(0, 0, 100);
      WriteLcd(2);
      bright = 2;
        
    } else if(code.value == 0xFFA857 && bright == 2){ //Increase luminosity by 1+ stage
      RGB_Collor(0, 0, 255);
      WriteLcd(1);
      bright = 1;

    // Button 3
    } else if(code.value == 0xFF7A85){
      digitalWrite(FAN, HIGH);
      WriteLcd(4);

    // Button 4
    } else if(code.value == 0xFF10EF){
      digitalWrite(FAN, LOW);
      WriteLcd(5);
    }
  }
  
  control.resume();
}

// Função de leitura e exibição de temperatura e humidade
void TempH(){
  
  DHT.read11(DHT11);
  int t = DHT.temperature;
  int h = DHT.humidity;
  
  lcd.setCursor(11, 0);
  if(t > 0){
    lcd.print(t);
    lcd.print(" C");
  }

  lcd.setCursor(11, 1);
  if(h > 0){
    lcd.print(h);
    lcd.print(" %");
  }
}


void RGB_Collor(int r, int g, int b){
  analogWrite(R, r);
  analogWrite(G, g);
  analogWrite(B, b);
}

void WriteLcd(int value){
  switch(value){
    case(1):
      lcd.setCursor(1, 0);
      lcd.print(": MAX");
      break;
    case(2):
      lcd.setCursor(1, 0);
      lcd.print(": MED");
      break;
    case(3):
      lcd.setCursor(1, 0);
      lcd.print(": LOW");
      break;
    case(4):
      lcd.setCursor(1, 1);
      lcd.print(": MAX");
      break;
    case(5):
      lcd.setCursor(1, 1);
      lcd.print(": LOW");
      break;
    case(6):
      lcd.setCursor(1, 0);
      lcd.print(": OFF");
      break;
    default:
      break;
    
  }
}

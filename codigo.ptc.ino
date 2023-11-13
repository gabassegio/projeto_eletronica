// === Biblioteca para o display === //
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>
#include <ezButton.h>

#define endereco  0x3F
#define colunas   16
#define linhas    2
ezButton ON_OFF(
  52);

// INSTANCIANDO OBJETOS
LiquidCrystal_I2C lcd(endereco, colunas, linhas);

// === Biblioteca para o TimerOne === //

#define botao_MAIS 49                //define o botão no pino 2 (MAIS)
#define botao_ENTER 51                 //define o botão no pino 3 (ENTER/VOLTAR)
#define botao_MENOS 53                 //define o botão no pino 4 (MENOS)
bool L_botao_MAIS = 1;                //variável para fazer a leitura do botão              (MAIS)
bool LA_botao_MAIS = 1;               //variável para verificar a leitura anterior do botão (MAIS)
bool L_botao_ENTER = 1;               //variável para fazer a leitura do botão              (ENTER/VOLTAR)
bool LA_botao_ENTER = 1;              //variável para verificar a leitura anterior do botão (ENTER/VOLTAR)
bool L_botao_MENOS = 1;               //variável para fazer a leitura do botão              (MENOS)
bool LA_botao_MENOS = 1;              //variável para verificar a leitura anterior do botão (MENOS)

int tela = 1;                         //variável para controlar a mudança de tela no menu
int passosPorVolta = 32;
int ledVerdePin = 13;
int ledAmareloPin = 18 ;
int Tmin = 0;
int peso = 30;
int trigger = 13;
int    echo = 12;

bool uma_vez = false;
bool state_1 = false;
bool state_2 = false;
unsigned long ant;


Stepper mp(passosPorVolta, 50, 46, 48, 44);

void alimentar(int peso) {
  lcd.setCursor(0, 0);
  lcd.print(F(" Alimentando... "));
  lcd.setCursor(0, 1);
  lcd.print(F(" YUMMY! (/^o^)/ "));
  digitalWrite(ledAmareloPin, HIGH);
  mp.step(2048);
  delay(3000);
  digitalWrite(ledAmareloPin, LOW);
  mp.step(0);
  tela = 30;
}

void ligar() {
  digitalWrite(ledVerdePin, HIGH);
  lcd.backlight();
  lcd.init();                    //inicia o display 16x2
  lcd.clear();                       //limpa todos os caracteres do lcd
  lcd.setCursor(0, 0);               //posiciona o cursor no display (caracter, linha)
  lcd.print("    PET CHEF    ");  //imprime o texto entre aspas
  lcd.setCursor(0, 1);               //posiciona o cursor no display (caracter, linha)
  lcd.print("   AUTOMATICO   ");  //imprime o texto entre aspas
  delay (1500);
  lcd.clear();                       //limpa todos os caracteres do lcd
  lcd.setCursor(0, 0);               //posiciona o cursor no display (caracter, linha)
  lcd.print(" SEJA BEM VINDO ");  //imprime o texto entre aspas
  lcd.setCursor(0, 1);               //posiciona o cursor no display (caracter, linha)
  lcd.print(" Iniciando...   ");  //imprime o texto entre aspas
  delay(1500);

}

void(* resetFunc) (void) = 0;

void setup() {
  ON_OFF.setDebounceTime(50);
  Serial.begin(9600);
  mp.setSpeed(500);
  pinMode(ledVerdePin, OUTPUT);
  pinMode(ledAmareloPin, OUTPUT);
  digitalWrite(ledVerdePin, LOW);
  pinMode(botao_MAIS, INPUT_PULLUP);   //define o botao (MAIS) como uma entrada pull up
  pinMode(botao_ENTER, INPUT_PULLUP);  //define o botao (ENTER/VOLTAR) como uma entrada pull up
  pinMode(botao_MENOS, INPUT_PULLUP);  //define o botao (MENOS) como uma entrada pull up

}


void loop() {
  ON_OFF.loop();
  int state = ON_OFF.getState();
  if (state == LOW) {
    if (uma_vez == false) {
      ligar();
    }
    uma_vez = true;
    state_1 = true;
  }
  else {
    state_1 = false;
    state_2 = false;
    digitalWrite(ledVerdePin, LOW);
    lcd.clear();
    tela = 1;
    uma_vez = false;
  }
  if (state_1) {

    // === Ações no display LCD === //
    if (tela == 1) {                       //se a variável tela for igual a MAIS, faça...
      lcd.setCursor(0, 0);                   //posiciona o cursor no display (caracter, linha)
      lcd.print(F(" <MENU Escolher>"));      //imprime o texto entre aspas
      lcd.setCursor(0, 1);                   //posiciona o cursor no display (caracter, linha)
      lcd.print(F(">Horario        "));      //imprime o texto entre aspas
    }
    else if (tela == 2) {

      lcd.setCursor(0, 0);
      lcd.print(F(" <MENU Escolher>"));
      lcd.setCursor(0, 1);
      lcd.print(F(">Quantidade     "));
    }
    else if (tela == 3) {

      lcd.setCursor(0, 0);
      lcd.print(F(" <MENU Escolher>"));
      lcd.setCursor(0, 1);
      lcd.print(F(">Comecar agora"));
    }
    else if (tela == 10) {

      while (digitalRead(botao_ENTER) == HIGH) {
        if (Tmin == 1440) {
          Tmin = 0;
        }
        int hora = Tmin / 60;
        int minuto = Tmin % 60;
        lcd.setCursor(0, 0);
        lcd.print(F(" :: HORARIO ::  ")); //(:: FREQUENCIA ::)
        lcd.setCursor(0, 1);
        lcd.print("> A CADA: ");
        lcd.print(hora);
        lcd.print(F(":"));
        lcd.print(minuto);
        L_botao_MAIS = digitalRead(botao_MAIS);
        if (!L_botao_MAIS && LA_botao_MAIS) {
          Tmin = Tmin + 30;
        }
        LA_botao_MAIS = L_botao_MAIS;
        L_botao_MENOS = digitalRead(botao_MENOS);
        if (!L_botao_MENOS && LA_botao_MENOS) {
          Tmin = Tmin - 30;
          if (Tmin == -30) {
            Tmin = 1410;
          }
        }
        LA_botao_MENOS = L_botao_MENOS;
      }
    }
    else if (tela == 20) {

      while (digitalRead(botao_ENTER) == HIGH) {
        if (peso == 330) {
          peso = 30;
        }
        lcd.setCursor(0, 0);
        lcd.print(F(":: Quantidade ::"));
        lcd.setCursor(0, 1);
        lcd.print(F(">"));
        lcd.print((peso));
        lcd.print(F(" gramas       "));
        L_botao_MAIS = digitalRead(botao_MAIS);
        if (!L_botao_MAIS && LA_botao_MAIS) {
          peso = peso + 30;
        }
        LA_botao_MAIS = L_botao_MAIS;
        L_botao_MENOS = digitalRead(botao_MENOS);
        if (!L_botao_MENOS && LA_botao_MENOS) {
          peso = peso - 30;
          if (peso == 0) {
            peso = 300;
          }
        }
        LA_botao_MENOS = L_botao_MENOS;
      }
    }
    else if (tela == 30) {
      lcd.setCursor(0, 0);
      lcd.print(F(":: AGUARDANDO ::"));
      lcd.setCursor(0, 1);
      lcd.print(F("                "));
      state_1 = false;
      state_2 = true;
    }





    /////////////////////////////////////////////////

    // === Ações do Botão MAIS === //    ////////////
    L_botao_MAIS = digitalRead(botao_MAIS);
    if (!L_botao_MAIS && LA_botao_MAIS) {
      if (tela == 1)tela = 2;
      else if (tela == 2)tela = 3;
      else if (tela == 3)tela = 1;
      delay(30);
    }
    LA_botao_MAIS = L_botao_MAIS;


    /////////////////////////////////////////////////

    // === Ações do Botão MENOS === //   ////////////
    L_botao_MENOS = digitalRead(botao_MENOS);
    if (!L_botao_MENOS && LA_botao_MENOS) {
      if (tela == 1)tela = 3;
      else if (tela == 2)tela = 1;
      else if (tela == 3)tela = 2;
      delay(30);
    }
    LA_botao_MENOS = L_botao_MENOS;


    /////////////////////////////////////////////////

    // === Ações do Botão ENTER === //   ////////////
    L_botao_ENTER = digitalRead(botao_ENTER);
    if (!L_botao_ENTER && LA_botao_ENTER) {
      if (tela == 1)tela = 10;
      else if (tela == 2)tela = 20;
      else if (tela == 3)tela = 30;
      else if (tela == 10)tela = 1;
      else if (tela == 20)tela = 2;
      else if (tela == 30)tela = 3;
      delay(30);
    }
    LA_botao_ENTER = L_botao_ENTER;
  }
  if (state_2) {
    if (millis() - ant >= (Tmin * 1000)) {
      alimentar(peso);
      ant = millis();

    }
  }
}

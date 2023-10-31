#include <ezButton.h>


int LED_GREEN = 13;

ezButton ON_OFF(2);  // BOTAO ON OFF


int BUTTON_1 = 3;
int BUTTON_2 = 4;
int BUTTON_3 = 5;
/* LDR PARA SABER QUANTIDADE DE COMIDA / ULTRASSOM / SENSOR DE CARRO PARA ÁGUA */
unsigned long ant;
bool state_1 = false;
bool state_2 = false;

int uma_vez = false;
int count_1 = 0;
int hours = {};
int times;

void setup() {

  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, 0);
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(BUTTON_3, INPUT);

  ON_OFF.setDebounceTime(50);

  Serial.begin(9600);
}

void loop() {
  ON_OFF.loop();
  int state = ON_OFF.getState();
  if (state == HIGH && not(state_2)) {
    digitalWrite(LED_GREEN, 1);
    state_1 = true;
  }
  else {
    state_1 = false;
    digitalWrite(LED_GREEN, 0);
    uma_vez = false;
  }
  if (state_1) {

    if (digitalRead(BUTTON_1) == 1) {
      Serial.println("Escolher quantidade de vezes ao dia");
      delay(1000);
      //times = how_many(); // CRIAR FUNÇÃO DE QUANTIDADE DE VEZES AO DIA > POTENCIOMETRO
    } else if (digitalRead(BUTTON_2) == 1) {
      Serial.println("Escolher horários");
      delay(1000);
      //hours = what_time(); //CRIAR FUNÇÃO DE HORÁRIOS > POTENCIOMETRO
    } else {
      while (not(uma_vez)) {
        Serial.println("Pressione o botão para configurar seu Pet Feeder");
        uma_vez = true;
      }
    }

    if (digitalRead(BUTTON_3) == 1) {
      Serial.println("Config ended..");
      uma_vez=false;
      state_1 = false;
      state_2 = true;
    }
  }

  
    else if (state_2) {
      digitalWrite(LED_GREEN,1);
    }
    /*
    for (int i = 0; i <= times; i++) {
      if (millis() - ant >= hours[i]*3600000) {
        Serial.println("Hora do lanchinho");
        dispenser(); //CRIAR FUNÇÃO PARA DESPEJAR COMIDA
      }
      if (digitalRead(BUTTON_2)) {
        Serial.println("PAUSED");
        state_2 = false;
        state_1 = true;
      }
    }
    ant = millis();
    }
  */
}

#include <Arduino.h>
#include <Motor.h>
#include <Ultrasonic.h>

#include <defines.h>

Ultrasonic sensorUltrasonico(TRIGGER_PIN, ECHO_PIN);

Motor motorEsquerdo = Motor(MOTOR_ESQUERDO_PWM, MOTOR_ESQUERDO_IN1, MOTOR_ESQUERDO_IN2);
Motor motorDireito = Motor(MOTOR_DIREITO_PWM, MOTOR_DIREITO_IN1, MOTOR_DIREITO_IN2);

enum DIRECAO {
    PARADO,
    FRENTE,
    RE,
    ESQUERDA,
    DIREITA
};

bool continuaExecutando(unsigned long tempo);
void frente();
void re();
void parar();
void virarDireita();
void virarEsquerda();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  parar();
  delay(5000);
}
  
int distance;
DIRECAO direcao = FRENTE;

unsigned long ultimaExecucao = 0;

void loop() {

  
  if(direcao == FRENTE) {
    distance = sensorUltrasonico.read();
      if(distance < 10) {
          direcao = RE;
          ultimaExecucao = millis();
      }
  }
  switch (direcao) {
    case FRENTE:
      frente();
      break;
    case RE:
      if(continuaExecutando(DELAY_RE)){
        re();
      }else {
        parar();
        randomSeed(analogRead(A0));
        int dir = random(1, 11);
        if(dir % 2){
          direcao = DIREITA;
        }else {
          direcao = ESQUERDA;
        }
        ultimaExecucao = millis();
      }
      break;
    case DIREITA:
      if(continuaExecutando(DELAY_VIRAR)){
        virarDireita();
      }else {       
        parar(); 
        direcao = FRENTE;
      }
      break;
    case ESQUERDA:
      if(continuaExecutando(DELAY_VIRAR)){
        virarEsquerda(); 
      }else {        
        parar();
        direcao = FRENTE;
      }
      break;  
    default: 
      parar();
      break;  
  }

  
#ifdef DEBUG
    LOG("DIR: ");
    LOG(direcao);
    LOG("\n");
#endif

}

bool continuaExecutando(unsigned long tempo) {
    unsigned long tempoPassado = (millis() - ultimaExecucao);
    
#ifdef DEBUG
    LOG("T: ");
    LOG(tempoPassado);
    LOG("\n");
#endif
    bool continua = tempoPassado < tempo;
#ifdef DEBUG
    LOG(" CONT: ");
    LOG(continua);
    LOG("\n");
#endif
   return continua; 
}

void frente() { 
    motorEsquerdo.moveFoward();
    motorDireito.moveFoward();
}

void re() {
    motorEsquerdo.moveBack();
    motorDireito.moveBack();
}

void virarDireita() {
  parar();
  motorEsquerdo.moveFoward();
}

void virarEsquerda() {
  parar();
  motorDireito.moveFoward();
}

void parar() {
  motorEsquerdo.stop();
  motorDireito.stop();
}
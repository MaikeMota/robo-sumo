#include <Arduino.h>
#include <Motor.h>

#include <defines.h>


Motor motorEsquerdo = Motor(MOTOR_ESQUERDO_PWM, MOTOR_ESQUERDO_IN1, MOTOR_ESQUERDO_IN2);
Motor motorDireito = Motor(MOTOR_DIREITO_PWM, MOTOR_DIREITO_IN1, MOTOR_DIREITO_IN2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // motorDireito.stop();
  motorEsquerdo.stop();
  LOG("started");
}

void loop() {

  LOG("STOPING ALL");
  motorDireito.stop();
  motorEsquerdo.stop();
  delay(500);

  LOG("MOVE BACK");
  // motorDireito.moveBack();
  motorEsquerdo.moveBack();
  delay(1500);

  LOG("MOVE FOWARD");
  // motorDireito.moveFoward();
  motorEsquerdo.moveFoward();
  delay(1500);

}
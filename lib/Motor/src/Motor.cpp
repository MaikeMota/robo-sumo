
#include <Arduino.h>
#include "Motor.h"


Motor::Motor(uint8_t pwmPin, uint8_t in1Pin, uint8_t in2Pin){
    _pwmPin = pwmPin;
    _in1Pin = in1Pin;
    _in2Pin = in2Pin;

    pinMode(_pwmPin, OUTPUT);
    pinMode(_in1Pin, OUTPUT);
    pinMode(_in2Pin, OUTPUT);
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
void Motor::moveFoward(){
    move(LOW, HIGH);
}

void Motor::moveBack(){
    move(HIGH, LOW);
}

void Motor::stop(){
    move(HIGH, HIGH);
}

void Motor::moveFoward(uint8_t pwmIntensity){
    setIntensity(pwmIntensity);
    moveFoward();
}

void Motor::moveBack(uint8_t pwmIntensity){
    setIntensity(pwmIntensity);
    moveBack();
}

void Motor::setIntensity(uint8_t intensity){
    targetPwmIntensity = intensity;
}

void Motor::move(bool in1, bool in2){
    digitalWrite(_in1Pin, in1);
    digitalWrite(_in2Pin, in2);
}
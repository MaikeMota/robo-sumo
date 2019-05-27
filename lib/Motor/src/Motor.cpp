
#include <Arduino.h>
#include "Motor.h"


Motor::Motor(int pwmPin, int in1Pin, int in2Pin){
    _pwmPin = pwmPin;
    _in1Pin = in1Pin;
    _in2Pin = in2Pin;

    pinMode(_pwmPin, OUTPUT);
    pinMode(_in1Pin, OUTPUT);
    pinMode(_in2Pin, OUTPUT);
}

void Motor::moveFoward(){
    move(HIGH, LOW);
}

void Motor::moveBack(){
    move(LOW, HIGH);
}

void Motor::stop(){
    move(HIGH, HIGH);
}

void Motor::moveFoward(int pwmIntensity){
    setIntensity(pwmIntensity);
    moveFoward();
}

void Motor::moveBack(int pwmIntensity){
    setIntensity(pwmIntensity);
    moveBack();
}

void Motor::setIntensity(int intensity){
    targetPwmIntensity = intensity;
}

void Motor::move(bool in1, bool in2){
    digitalWrite(_in1Pin, in1);
    digitalWrite(_in2Pin, in2);
}
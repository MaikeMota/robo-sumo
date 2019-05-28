#ifndef MOTOR_H

#define MOTOR_H

class Motor {

    public:
        Motor(uint8_t pwmControl, uint8_t in1Pin, uint8_t in2Pin);

        void moveFoward();
        void moveBack();
        void moveFoward(uint8_t pwmIntensity);
        void moveBack(uint8_t pwmIntensity);
        void setIntensity(uint8_t intensity);
        void stop();


    private:
        int _pwmPin;
        int _in1Pin;
        int _in2Pin;
        int targetPwmIntensity;
        int currentPwnIntensity;
        void move(bool in1, bool in2);

};

#endif
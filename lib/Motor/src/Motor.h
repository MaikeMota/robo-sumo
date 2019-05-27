#ifndef MOTOR_H

#define MOTOR_H

class Motor {

    public:
        Motor(int pwmControl, int in1Pin, int in2Pin);

        void moveFoward();
        void moveBack();
        void moveFoward(int pwmIntensity);
        void moveBack(int pwmIntensity);
        void setIntensity(int intensity);
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
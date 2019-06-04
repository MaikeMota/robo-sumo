#define MOTOR_ESQUERDO_PWM 3
#define MOTOR_ESQUERDO_IN1 4
#define MOTOR_ESQUERDO_IN2 5

#define MOTOR_DIREITO_PWM 11
#define MOTOR_DIREITO_IN1 12
#define MOTOR_DIREITO_IN2 13

#define SENSOR_LINHA_CENTRAL 2
#define SENSOR_LINHA_ESQUERDA 9
#define SENSOR_LINHA_DIREITA 8

#define TRIGGER_PIN 6
#define ECHO_PIN 7

#define DISTANCIA_MINIMA_PERSEGUIR 25
#define TEMPO_FUGIR_LINHA 500
#define TEMPO_VIRADA_PROCURAR_OPONENTE 350

// #define DEBUG

#ifdef DEBUG
    #define LOG(prefixo, valor, sufixo) Serial.print(prefixo); Serial.print(valor); Serial.print(sufixo)
    #define QUEBRA_LINHA() Serial.println();
    #define LINE_FEED \n
#endif
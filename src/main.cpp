#include <Arduino.h>
#include <Motor.h>
#include <Ultrasonic.h>

#include <defines.h>
#include <types.h>

/**                                                                                         
 * ===========================================================================================
 *                              Variáveis                                                   ||
 * ===========================================================================================
 * */
Ultrasonic sensorUltrasonico(TRIGGER_PIN, ECHO_PIN);
Motor motorEsquerdo = Motor(MOTOR_ESQUERDO_PWM, MOTOR_ESQUERDO_IN1, MOTOR_ESQUERDO_IN2);
Motor motorDireito = Motor(MOTOR_DIREITO_PWM, MOTOR_DIREITO_IN1, MOTOR_DIREITO_IN2);
Direcao direcao = PARADO;
Estado estado = PROCURANDO_OPONENTE;
unsigned long ultimaAcaoExecutada = 0;
unsigned long tempoMovimentacaoMotores = 0;

/**                                                                                         
 * ===========================================================================================
 *                              Protótipo de Funções                                        ||
 * ===========================================================================================
 * */
void procurarOponente();
void tratarMovimentacao();
bool sortearParImpar();
void moverDistanteLinha();
void verificarLinhas();
Direcao sortearEsquerdaDireita();
void marcarTempoUltimaMovimentacaoMotores();
bool continuaExecutando(unsigned long tempo);
void frente();
void re();
void parar();
void virarDireita();
void virarEsquerda();

void trataInterrupcao()
{
#ifdef DEBUG
  LOG("trataInterrupcao: ", digitalRead(SENSOR_LINHA_CENTRAL), LINE_FEED);
#endif
  moverDistanteLinha();
}

void setup()
{
#ifdef DEBUG
  Serial.begin(9600);
#endif
  parar();

#ifdef ARENA_CLARA
  attachInterrupt(digitalPinToInterrupt(SENSOR_LINHA_CENTRAL), trataInterrupcao, RISING);
#endif
#ifdef ARENA_ESCURA
  attachInterrupt(digitalPinToInterrupt(SENSOR_LINHA_CENTRAL), trataInterrupcao, FALLING);
#endif
  delay(5000);
#ifdef DEBUG
  LOG("INICIADO...", "", "");
#endif
}

void loop()
{
  switch (estado)
  {
  case PROCURANDO_OPONENTE:
    procurarOponente();
    break;
  case MOVIMENTANDO:
    tratarMovimentacao();
    break;
  }
  verificarLinhas();
}

void verificarLinhas()
{
  bool sensorEsquerda, sensorCentro;
  sensorEsquerda = digitalRead(SENSOR_LINHA_ESQUERDA);
  sensorCentro = digitalRead(SENSOR_LINHA_CENTRAL);
#ifdef DEBUG
  LOG(sensorEsquerda, " - ", sensorCentro);
  QUEBRA_LINHA();
#endif
#ifdef ARENA_CLARA
  if (sensorEsquerda || sensorCentro)
#endif
#ifdef ARENA_ESCURA
  if (!sensorEsquerda || !sensorCentro)
#endif
  {
    moverDistanteLinha();
  }
}

void procurarOponente()
{
  int distanciaObstaculo = sensorUltrasonico.read();
  if (distanciaObstaculo > DISTANCIA_MINIMA_PERSEGUIR)
  {
    direcao = sortearEsquerdaDireita();
#ifdef DEBUG
    LOG("procurarOponente: ", direcao, LINE_FEED);
#endif
    tempoMovimentacaoMotores = TEMPO_VIRADA_PROCURAR_OPONENTE;
    marcarTempoUltimaMovimentacaoMotores();
  }
  else
  {
    direcao = FRENTE;
  }
  estado = MOVIMENTANDO;
}

void tratarMovimentacao()
{
  switch (direcao)
  {
  case FRENTE:
    frente();
    break;
  case RE:
    if (continuaExecutando(tempoMovimentacaoMotores))
    {
      re();
    }
    else
    {
      estado = PROCURANDO_OPONENTE;
    }
    break;
  case DIREITA:
    if (continuaExecutando(tempoMovimentacaoMotores))
    {
      virarDireita();
    }
    else
    {
      estado = PROCURANDO_OPONENTE;
    }
    break;
  case ESQUERDA:
    if (continuaExecutando(tempoMovimentacaoMotores))
    {
      virarEsquerda();
    }
    else
    {
      estado = PROCURANDO_OPONENTE;
    }
    break;
  case PARADO:
    parar();
    break;
  }
#ifdef DEBUG
  LOG("Direção: ", direcao, LINE_FEED);
#endif
}

void moverDistanteLinha()
{
  direcao = RE;
  estado = MOVIMENTANDO;
  tempoMovimentacaoMotores = TEMPO_FUGIR_LINHA;
  marcarTempoUltimaMovimentacaoMotores();
}

bool sortearParImpar()
{
  randomSeed(analogRead(A0) + analogRead(A5));
  int numero = random(1, 11);
  return numero % 2;
}

Direcao sortearEsquerdaDireita()
{
  return sortearParImpar() ? DIREITA : ESQUERDA;
}

void marcarTempoUltimaMovimentacaoMotores()
{
  ultimaAcaoExecutada = millis();
}

bool continuaExecutando(unsigned long tempo)
{
  unsigned long tempoPassado = (millis() - ultimaAcaoExecutada);

#ifdef DEBUG
  LOG("Tempo passado: ", tempoPassado, LINE_FEED);
#endif
  bool continua = tempoPassado < tempo;
#ifdef DEBUG
  LOG("Continua?: ", continua, LINE_FEED);
#endif
  return continua;
}

void frente()
{
  motorEsquerdo.moveFoward();
  motorDireito.moveFoward();
}

void re()
{
  motorEsquerdo.moveBack();
  motorDireito.moveBack();
}

void virarDireita()
{
  parar();
  motorEsquerdo.moveFoward();
}

void virarEsquerda()
{
  parar();
  motorDireito.moveFoward();
}

void parar()
{
  motorEsquerdo.stop();
  motorDireito.stop();
}
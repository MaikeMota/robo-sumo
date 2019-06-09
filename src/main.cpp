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
Direcao direcao;
Estado estado = PROCURANDO_OPONENTE;
unsigned long ultimaMovimentacaoMotorExecutada = 0;
unsigned long tempoMovimentacaoMotores = 0;

/**                                                                                         
 * ===========================================================================================
 *                              Protótipo de Funções                                        ||
 * ===========================================================================================
 * */
void procurarOponente();
void tratarMovimentacao();
bool sortearParImpar();
void moverParaFrente();
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
void trataInterrupcao();

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
  verificarLinhas();
  switch (estado)
  {
  case PROCURANDO_OPONENTE:
    procurarOponente();
    break;
  case MOVIMENTANDO:
    tratarMovimentacao();
  }
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

bool isDistanciaMinima() {  
  int distanciaObstaculo = sensorUltrasonico.read();
#ifdef DEBUG
  LOG("distanciaObstaculo: ", distanciaObstaculo, LINE_FEED);
#endif
  return distanciaObstaculo <= DISTANCIA_MINIMA_PERSEGUIR;
}

void procurarOponente()
{
  if (isDistanciaMinima())
  {
    moverParaFrente();
  }
  else
  {
    direcao = sortearEsquerdaDireita();
    tempoMovimentacaoMotores = TEMPO_VIRADA_PROCURAR_OPONENTE;
    marcarTempoUltimaMovimentacaoMotores();
  }
#ifdef DEBUG
    LOG("procurarOponente: ", direcao, LINE_FEED);
#endif
  estado = MOVIMENTANDO;
}

void tratarMovimentacao()
{
  parar();
  switch (direcao)
  {
  case FRENTE:
    if (continuaExecutando(tempoMovimentacaoMotores)){
      frente();
    }else {
      estado = PROCURANDO_OPONENTE;
    }
    break;
  case RE:
    if (continuaExecutando(tempoMovimentacaoMotores))
    {
      re();
    }
    else
    {
      moverParaFrente();
    }
    break;
  case DIREITA:
    if (continuaExecutando(tempoMovimentacaoMotores))
    {
      virarDireita();
    }
    else
    {
      moverParaFrente();
    }
    break;
  case ESQUERDA:
    if (continuaExecutando(tempoMovimentacaoMotores))
    {
      virarEsquerda();
    }
    else
    {
      moverParaFrente();
    }
    break;
  }
#ifdef DEBUG
  LOG("Direção: ", direcao, LINE_FEED);
#endif
}

void moverParaFrente(){ 
  direcao = FRENTE;
  tempoMovimentacaoMotores = TEMPO_SEGUINDO_EM_FRENTE;
  marcarTempoUltimaMovimentacaoMotores();
  estado = MOVIMENTANDO;
}

void moverDistanteLinha()
{
  parar();
  direcao = RE;
  tempoMovimentacaoMotores = TEMPO_FUGIR_LINHA;
  marcarTempoUltimaMovimentacaoMotores();
  estado = MOVIMENTANDO;
}

bool sortearParImpar()
{
  long seed = (analogRead(A0) + (analogRead(A5))) / analogRead(A1);
  randomSeed(seed) ;
  int numero = random(1, 11);
  return numero % 2;
}

Direcao sortearEsquerdaDireita()
{
  return sortearParImpar() ? DIREITA : ESQUERDA;
}

void marcarTempoUltimaMovimentacaoMotores()
{
  ultimaMovimentacaoMotorExecutada = millis();
}

bool continuaExecutando(unsigned long tempo)
{
  unsigned long tempoPassado = (millis() - ultimaMovimentacaoMotorExecutada);

#ifdef DEBUG
  LOG("Tempo passado: ", tempoPassado, LINE_FEED);
#endif
  bool continua = tempoPassado < tempo;
#ifdef DEBUG
  LOG("Continua?: ", continua, LINE_FEED);
#endif
  return continua;
}

void trataInterrupcao()
{
#ifdef DEBUG
  LOG("trataInterrupcao: ", digitalRead(SENSOR_LINHA_CENTRAL), LINE_FEED);
#endif
  moverDistanteLinha();
}

void setup()
{
  Serial.begin(9600);
  parar();
  attachInterrupt(digitalPinToInterrupt(SENSOR_LINHA_CENTRAL), trataInterrupcao, RISING);
  motorDireito.setIntensity(255);
  motorEsquerdo.setIntensity(255);
  delay(5000);
#ifdef DEBUG
  LOG("INICIADO...", "", "");
#endif
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
  motorEsquerdo.moveFoward();
}

void virarEsquerda()
{
  motorDireito.moveFoward();
}

void parar()
{
  motorEsquerdo.stop();
  motorDireito.stop();
}
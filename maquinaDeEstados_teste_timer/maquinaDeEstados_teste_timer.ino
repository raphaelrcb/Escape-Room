
#include <Arduino.h>
#include <TimerOne.h>

#define estado_0 0
#define estado_1 1
#define estado_2 2
#define estado_3 3
#define tempoContador 15 // tempo em segundos


//STRING DAS DICAS

char Etapa1dica1[500] = "Tente apertar o botao1...";
char Etapa1dica2[500] = "O primeiro da direita para a esquerda..";
char Etapa1dica3[500] = "O terceiro da esquerda para a direita..";
char Etapa2dica1[500] = "Tente apertar o botao2...";
char Etapa2dica2[500] = "Tente apertar o botao1...";
char Etapa2dica3[500] = "Tente apertar o botao1...";
char Etapa3dica1[500] = "Tente apertar o botao1...";
char Etapa3dica2[500] = "Tente apertar o botao1...";
char Etapa3dica3[500] = "Tente apertar o botao1...";
char SemDica[500] = "Acabaram as dicas para essa etapa :( ";
char Acabou[500] = "Tente apertar o botao1...";


volatile char controle;

const int botao1 = 2;
const int botao2 = 3;
const int botao3 = 4;
const int dicas = 5;
const int led = 6;

int estado1 = 0;
int estado2 = 0;
int estado3 = 0;
int querDica = 0;

int tempo = tempoContador;
int contador = 0;
int tempoParaDica2 = 10;
int tempoParaDica3 = 5;

boolean dica1 = false;
boolean dica2 = false;
boolean dica3 = false;
int nextDica = 0;

void setup(){
  
  pinMode(botao1, INPUT);
  pinMode(botao2, INPUT);
  pinMode(botao3, INPUT);
  pinMode(dicas, INPUT);
  pinMode(led, OUTPUT);
  
  digitalWrite(led,LOW);
  Serial.begin(9600);
  
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(contaTempo); 
  
  controle = estado_0;
  estado1 = 0;
  estado2 = 0;
  estado3 = 0;
  querDica = 0;
  
}

void maquinaDeEstados(){
  
  switch(controle){
    case estado_0:
    {
      if(estado1 == HIGH){
        controle = estado_1;
      }else{
        if(querDica == HIGH && dica1 == false){
          Serial.println(Etapa1dica1);
          querDica = LOW;
          dica1 = true; //não deixa que a primeira dica dessa etapa volte a se repetir
          nextDica = 1;
          contador = tempo;
          Timer1.start();
          Timer1.attachInterrupt(contaTempo);
          delay(1000);
        }else if((dica2 == true || querDica == HIGH)  && nextDica == 1){
          Serial.println(Etapa1dica2);
          dica2 = false;
          nextDica = 2;
          querDica = LOW;
          delay(1000);
        }else if((dica3 == true  || querDica == HIGH) && nextDica == 2){
          Serial.println(Etapa1dica3);
          querDica = LOW;
          dica3 = false;
          nextDica = 3;
          delay(1000);
        }else if(querDica == HIGH && nextDica == 3){
          Serial.println(SemDica);
          querDica = LOW;
          delay(1000);
        }
      }
      break;
    }
    case estado_1:
    {
      if(estado2 == HIGH){
        controle = estado_2;
      }else{
        if(querDica == HIGH && dica1 == false){
          Serial.println(Etapa2dica1);
          querDica = LOW;
          dica1 = true;
          nextDica = 1;
          contador = tempo;
          Timer1.start();
          Timer1.attachInterrupt(contaTempo);
          delay(1000);
        }else if((dica2 == true || querDica == HIGH)  && nextDica == 1){
          Serial.println("O botao do meio..");
          querDica = LOW;
          dica2 = false;
          nextDica = 2;
          delay(1000);
        }else if((dica3 == true  || querDica == HIGH) && nextDica == 2){
          Serial.println("O segundo da esquerda para a direita..");
          querDica = LOW;
          dica3 = false;
          nextDica = 3;
          delay(1000);
        }else if(querDica == HIGH && nextDica == 3){
          Serial.println("Acabaram as dicas para essa etapa :( ");
          querDica = LOW;
          delay(1000);
        }
      }
      break;
    }
    case estado_2:
    {
      if(estado3 == HIGH){
        controle = estado_3;
      }else{
        if(querDica == HIGH && dica1 == false){
          Serial.println("Tente apertar o botao3...");
          querDica = LOW;
          dica1 = true;
          nextDica = 1;
          contador = tempo;
          Timer1.start();
          Timer1.attachInterrupt(contaTempo);
          delay(1000);
        }else if((dica2 == true || querDica == HIGH)  && nextDica == 1){
          Serial.println("O terceiro da direita para a esquerda..");
          querDica = LOW;
          dica2 = false;
          nextDica = 2;
          delay(1000);
        }else if((dica3 == true  || querDica == HIGH) && nextDica == 2){
          Serial.println("O primeiro da esquerda para a direita..");
          querDica = LOW;
          dica3 = false;
          nextDica = 3;
          delay(1000);
        }else if(querDica == HIGH && nextDica == 3){
          Serial.println("Acabaram as dicas para essa etapa :( ");
          querDica = LOW;
          delay(1000);
        }
      }
      break;
    }
    case estado_3:
    {
      if(querDica == HIGH && dica1 == false){
        Serial.println("VOCÊ TERMINOU!! ");
        querDica = LOW;
        delay(1000);
      }
      break;
    }
  }
  
}

void contaTempo(){
  if(contador <= 0){
    contador = 0;
  }
  
  contador--;
  
  if(contador == tempoParaDica2){
    if(nextDica == 1){
      dica2 = true;
    }
  }
  else if(contador == tempoParaDica3){
    if(nextDica == 2){
      dica3 = true;
    }
  }
  else{
    dica2 = false;
    dica3 = false;
  }
}

void loop(){
  
  if(digitalRead(dicas) == HIGH){
    querDica = HIGH;
  }

  if(digitalRead(botao1) == HIGH){
    estado1 = HIGH;
    querDica = LOW;
    Timer1.stop();
    contador = 0;
    dica1 = false;
  }
  if(digitalRead(botao2) == HIGH){
    estado2 = HIGH;
    querDica = LOW;
    Timer1.stop();
    contador = 0;
    dica1 = false;
  }
  if(digitalRead(botao3) == HIGH){
    estado3 = HIGH;
    querDica = LOW;
    Timer1.stop();
    contador = 0;
    dica1 = false;
  }
  
  maquinaDeEstados();
  
}

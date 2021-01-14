#include <IRremote.h>

//-------------------- Definiciones --------------------
#define SENSOR_IR 12
#define ZUMBADOR 3

//Rueda izquierda
#define RI_ACTIV 8
#define RI_2 10
#define RI_1 9

//Rueda derecha
#define RD_ACTIV 7
#define RD_2 6
#define RD_1 5

//Valores teclas mando
#define N 0xCF2F9DAB
#define E 0x6F15FF46
#define S 0xB2BBAC69
#define O 0xFB758842
#define NE 0x62C25DA5
#define SE 0x1584D3EB
#define SO 0xC0CDDA6
#define NO 0xF9F925C3
#define CLAXON 0x123CD34B
#define CONTINUAR 0xFFFFFF

//-------------------- Creacion de tipos --------------------
enum Rueda {derecha, izquierda};
enum Sentido {horario, antihorario};
enum Direccion {n, s, e, o, ne, no, se, so, nulo};

//-------------------- Declaracion de variables --------------------
IRrecv receptor(SENSOR_IR);
decode_results codigo_boton;
Direccion ultima_dir = nulo;
int zumbador_val = LOW;
int max_vel= 255; min_vel = 200;

//-------------------- Funciones --------------------
void setup() {
  pinMode(RD_ACTIV, OUTPUT);
  pinMode(RI_ACTIV, OUTPUT);
  pinMode(RI_2, OUTPUT);
  pinMode(RI_1, OUTPUT);
  pinMode(RD_2, OUTPUT);
  pinMode(RD_1, OUTPUT);
  pinMode(ZUMBADOR, OUTPUT);
  receptor.enableIRIn();
}

//Funcion encargada de mover una rueda en vel y sentido dados
void moverRueda(Rueda r, Sentido s, int v){
  int rueda, pin1, pin2;
  if (r == derecha){
    rueda = RD_ACTIV;
    pin1 = RD_1;
    pin2 = RD_2;
  }
  else{
    rueda = RI_ACTIV;
    pin1 = RI_1;
    pin2 = RI_2;
  }
  if(v == 0)
    digitalWrite(rueda, LOW);
  else {
    if(s == horario){
      analogWrite(pin1, v);
      analogWrite(pin2, 255 - v);
    } else {
      analogWrite(pin1, 255 - v);
      analogWrite(pin2, v);
    }
    digitalWrite(rueda, HIGH);
  }
}

//Funcion encargada de mover el coche en una direccion
void moverHacia (Direccion d){
  switch(d){
    case n:{
      moverRueda(izquierda, antihorario, max_vel);
      moverRueda(derecha, horario, max_vel);
    } break;
    case s: {
      moverRueda(izquierda, horario, max_vel);
      moverRueda(derecha, antihorario, max_vel);
    } break;
    case e: {
      moverRueda(izquierda, antihorario, max_vel);
      moverRueda(derecha, antihorario, max_vel);
    } break;
    case o: {
      moverRueda(izquierda, horario, max_vel);
      moverRueda(derecha, horario, max_vel);
    } break;
    case ne: {
      moverRueda(izquierda, antihorario, max_vel);
      moverRueda(derecha, horario, min_vel);
    } break;
    case no: {
      moverRueda(izquierda, antihorario, min_vel);
      moverRueda(derecha, horario, max_vel);
    } break;
    case se: {
      moverRueda(izquierda, horario, max_vel);
      moverRueda(derecha, antihorario, min_vel);
    } break;
    case so: {
      moverRueda(izquierda, horario, min_vel);
      moverRueda(derecha, antihorario, max_vel);
    } break;
    default: {
      moverRueda(izquierda, antihorario, 0);
      moverRueda(derecha, horario, 0);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (receptor.decode(&codigo_boton)) {
    if (codigo_boton.value != CONTINUAR){
      if (codigo_boton.value != CLAXON) zumbador_val = LOW;
      switch(codigo_boton.value) {
        case N: ultima_dir = n;
        break;
        case NE: ultima_dir = ne;
        break;
        case E: ultima_dir = e;
        break;
        case SE: ultima_dir = se;
        break;
        case S: ultima_dir = s;
        break;
        case SO: ultima_dir = so;
        break;
        case O: ultima_dir = o;
        break;
        case NO: ultima_dir = no;
        break;
        case CLAXON: {
          ultima_dir = nulo;
          zumbador_val = HIGH;
        }
        break;
        default:{
          ultima_dir = nulo;
        }
      }
    }
    moverHacia(ultima_dir);
    digitalWrite(ZUMBADOR, zumbador_val);
    receptor.resume();
    delay(150);
  } else {
    moverHacia(nulo);
    digitalWrite(ZUMBADOR, LOW);
  }
}

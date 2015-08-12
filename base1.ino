
/*
 * the interrupts are now implemented and working.
 * Int0 on D3(SCL), In1 on D2(SDA), Int2, on D0(RX), Int3 on D1(TX)
 * 
 * in https://code.google.com/p/arduino/issues/detail?id=714
 */

#define   MIN_VELOCITY 1  //velocidade minima em KM/h 
#define   MIN_INTERVAL 40 //minimo tempo de intervalo entre leituras para executar debouce em ms
int pin         = 13;     //saida led de indicação de leitura nos reeds

int pinSpeed    = 0 ;     //entrada do reed de velocidade
int pinCadence  = 1 ;     //entrada do reed de cadência


int intSpeed    = 2 ;     //numero da interrupção de velocidade
int intCadence  = 3 ;     //numero da interrupção de cadência

int WheelSize = 2080;     //tamanho da roda em decimas de mm
float speedMaxInterval;   //vai conter o valor maximo de ms para contar velocidade
float actualSpeed;        //velociade atual em KM/h
float rpmCadence;         //contagem de voltas dos pedais por minuto

int i =0;
int ii =0;
int j =0;
int jj =0;

volatile int state = 0;
volatile unsigned long speedLastInterval;
volatile unsigned long cadenceLastInterval;

void initialize_vars(){
  float temp1 = MIN_VELOCITY * 1000000;
  float temp2 = temp1 / 3600;
  speedMaxInterval = WheelSize/ temp2 * 1000;
}

void setup(){
  pinMode(pin, OUTPUT);
  pinMode(pinSpeed, INPUT_PULLUP);
  pinMode(pinCadence, INPUT_PULLUP);
  attachInterrupt(intSpeed, speedSensorActivated, FALLING);
  attachInterrupt(intCadence, cadenceSensorActivated, FALLING);
  Serial.begin(9600);
  initialize_vars();
  interrupts();
}

void loop(){
  digitalWrite(pin, state);
  Serial.print("i=");Serial.print(i);
  Serial.print(" ii=");Serial.print(ii);
  Serial.print("\tj=");Serial.print(j);
  Serial.print(" jj=");Serial.print(jj);
  long temp1 = 3600000 / speedLastInterval;
  float temp2 = (temp1 * WheelSize);
  float actualSpeed = temp2 / 1000000;
  Serial.print("\tSpeed=");Serial.print(actualSpeed);
  Serial.print("\tInterval=");Serial.print(cadenceLastInterval);
  rpmCadence = 60000 / cadenceLastInterval;
  Serial.print("\tcadenceRPM=");Serial.print(rpmCadence);
  Serial.println("");
}

void speedSensorActivated(){
  i++;
  static unsigned long speed_last_interrupt_time = 0;
  unsigned long speed_interrupt_time = millis();
 // If interrupts come faster than MIN_INTERVAL, assume it's a bounce and ignore
  if (speed_interrupt_time - speed_last_interrupt_time >  MIN_INTERVAL){
    ii++;
    state = !state;
    speedLastInterval = speed_interrupt_time - speed_last_interrupt_time;
  }
  speed_last_interrupt_time = speed_interrupt_time;
}

void cadenceSensorActivated(){
  j++;
  static unsigned long cadence_last_interrupt_time = 0;
  unsigned long cadence_interrupt_time = millis();
 // If interrupts come faster than MIN_INTERVAL, assume it's a bounce and ignore
  if (cadence_interrupt_time - cadence_last_interrupt_time >  MIN_INTERVAL){
    jj++;
    state = !state;
    cadenceLastInterval = cadence_interrupt_time - cadence_last_interrupt_time;
  }
  cadence_last_interrupt_time = cadence_interrupt_time;
}

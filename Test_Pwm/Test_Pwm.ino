//------------------------------------------------------------------------------------
// Test_Pwm.c
// 
//------------------------------------------------------------------------------------

#include    "Tiva_TM4C123.h"


#define PIN_LED     PF_3
#define PIN_MT1     PA_5
#define PIN_MT2     PB_0
#define SIZE       1024

#define SKIP       60

#define FS         10000.0f

#define WINDOW_MS  250

#define NWIN       int(WINDOW_MS * FS / 1000.0f)
 
#define F1 2100.0f

//#define F2 3900.0f
 
#define PIN_IR A0

struct cmpx { float real, imag; };
 
cmpx dataFFT[SIZE];

float magFFT[SIZE/2];

float segment[NWIN];
 
void fft(cmpx *Y, int fftlen);


float tab[SIZE];
float newTab[SIZE];
float min;
float max = 0;
 

int counter, vled;

void setup() 
//----------------------------------------------------
//----------------------------------------------------
{
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_MT1, INPUT);
  pinMode(PIN_MT2, INPUT);
  pinMode(PIN_IR, INPUT);

  delay(2000);
  Serial.println("+++ INIT  Debut");
  Motor_Pwm_Init();
  Serial.println("+++ INIT  Fin  ++++");

  Motor_Pwm_Action(MOTOR_STOP);
  delay(1000);
  counter = 0;      vled = 0;
}



      
void loop() 
//----------------------------------------------------
//----------------------------------------------------
{

  tab[SIZE];
  newTab[SIZE];
  for(int i = 0; i < SIZE; i++){
    tab[i] = (float) analogRead(6);
    if(i == 0){
      min = tab[0];
    }
    if(tab[i] < min){
      min = tab[i];
    }
    if(tab[i] > max){
      max = tab[i];
    }
    delayMicroseconds(0.1);
  }

  for(int i = 0; i < SIZE;i++){
    newTab[i] = -1 + ((tab[i]-min)*2)/(max-min);
    }

  float sum = 0;

  for(int i=0; i<SIZE; i++){

    float v = newTab[i];         // <-- lecture directe du tableau

    if(i >= SKIP) sum += v;

    dataFFT[i].real = (i<SKIP ? 0.0f : v);

    dataFFT[i].imag = 0.0f;

  }

  float mean = sum / (SIZE - SKIP);

  for(int i=SKIP; i<SIZE; i++){

    dataFFT[i].real -= mean;

  }
 
  // 2) FFT maison

  fft(dataFFT, SIZE);
 
  // 3) Magnitude

  for(int k=0; k<SIZE/2; k++){

    float re = dataFFT[k].real, im = dataFFT[k].imag;

    magFFT[k] = sqrtf(re*re + im*im);

  }
 
  // 4) Détection

  int k1 = int(F1 * SIZE / FS + 0.5f);


  float m1 = magFFT[k1];
  

  int detect = 0; float fdet = 0;


  Serial.print("Valeur M1 : ");Serial.print(m1);

 // if(m1>m2 && m1>m2*1.1f)      { detect=1; fdet=F1; }


 
  // 5) Extraction segment utile

  int idx_start = SKIP;

  float seuil = 0.3f * m1;

  for(int i=SKIP; i<SIZE; i++){

    if(fabsf(dataFFT[i].real) > seuil){

      idx_start = i; break;

    }

  }

  if(idx_start + NWIN <= SIZE){

    for(int i=0; i<NWIN; i++){

      segment[i] = newTab[idx_start];//j'ai changé avec +1

    }

  }
 
  // 6) Affichage

  Serial.print("Detect=");   Serial.print(detect);

  Serial.print(" f=");       Serial.print(fdet,1);

  Serial.print(" startIdx=");Serial.print(idx_start);

  Serial.print(" startMs="); Serial.println(idx_start*1000.0f/FS,2);
 
      //if(detect != 0){
        if(m1 < 5){
          Motor_Pwm_Action(MOTOR_AVANCER);
          delay(1000);
        }
        /*
        Motor_Pwm_Action(MOTOR_RECULER);
        delay(1000);
        Motor_Pwm_Action(MOTOR_AVANT_G);
        delay(1000);
        Motor_Pwm_Action(MOTOR_AVANCER);
        delay(1000);
        Motor_Pwm_Action(MOTOR_AVANT_D);
        delay(1000);*/
      //}
      

      delay(500);
      
}


#ifndef PI

#define PI 3.14159265358979323846

#endif
 
void make_twiddle(int N, cmpx *W){

  for(int n=0; n<N; n++){

    float a = PI * n / N;

    W[n].real = cosf(a);

    W[n].imag = -sinf(a);

  }

}
 
void fft(cmpx *Y, int N){

  static cmpx W[SIZE];

  make_twiddle(N, W);

  int levels = 0, t=1;

  while(t < N){ t <<= 1; levels++; }

  int half = N>>1, step = 2;

  for(int s=0; s<levels; s++){

    int idx = 0;

    for(int j=0; j<half; j++){

      for(int u=j; u<N; u+=(half<<1)){

        int l = u + half;

        float ur=Y[u].real, ui=Y[u].imag;

        float lr=Y[l].real, li=Y[l].imag;

        float tr = lr*W[idx].real - li*W[idx].imag;

        float ti = lr*W[idx].imag + li*W[idx].real;

        Y[u].real = ur + tr; Y[u].imag = ui + ti;

        Y[l].real = ur - tr; Y[l].imag = ui - ti;

      }

      idx += step;

    }

    half >>= 1; step <<= 1;

  }

  int j=0;

  for(int i=1; i<N-1; i++){

    int bit = N>>1;

    for(; j & bit; bit >>= 1) j ^= bit;

    j |= bit;

    if(i<j){ cmpx tmp=Y[i]; Y[i]=Y[j]; Y[j]=tmp; }

  }

}

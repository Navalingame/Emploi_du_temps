// ChariotFFT.ino

#include <Energia.h>

#include <math.h>

#include "buffer.h"      // <-- ton tableau de 1024 valeurs
 
#define SIZE       1024

#define SKIP       60

#define FS         10000.0f

#define WINDOW_MS  250

#define NWIN       int(WINDOW_MS * FS / 1000.0f)
 
#define F1 2100.0f

#define F2 3900.0f
 
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
 
//---------------------------------------------------------------------------
 
void setup(){

  Serial.begin(115200);

  pinMode(PIN_IR, INPUT);

}
 
void loop(){

  // 1) Charger dataFFT[] depuis monBuffer[] + centrage + skip

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
    delayMicroseconds(10);
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

  int k2 = int(F2 * SIZE / FS + 0.5f);

  float m1 = magFFT[k1];
  
  float m2 = magFFT[k2];

  int detect = 0; float fdet = 0;


  Serial.print("Valeur M1 : ");Serial.print(m1);
  Serial.print(" Valeur M2 : ");Serial.println(m2);
  if(m1>m2 && m1>m2*1.1f)      { detect=1; fdet=F1; }

  else if(m2>m1 && m2>m1*1.1f) { detect=2; fdet=F2; }
 
  // 5) Extraction segment utile

  int idx_start = SKIP;

  float seuil = 0.3f * max(m1, m2);

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
 
  delay(500);

}
 
//---------------------------------------------------------------------------

// FFT maison (copié précédemment)

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

 

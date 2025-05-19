//-----------------------------------------------------------------------------
//
// Sensor Measurement (Signal from Sensor : microphone or IR or ECG).
//
//  Read the comments and understand the program.
//  Replace the “????” with a suitable value before running the program.
// Step-1 : Goto ##TAG##1 Set the sensor pin and determine SAMPLE_FREQ and SIZE_BUFF
// Step-2 : Goto ##TAG##2 read and understand the sampling function
// Step-3 : Goto ##TAG##3 Translate your Matlab program to calculate the mean power
// Step-4 : Goto ##TAG##4 determine the LOW, MAX and AVR level of input signal
// Step-5 : Goto ##TAG##5 Complete the main program loop()
//-----------------------------------------------------------------------------

#include    "Gnx_Project_Chariot.h"


// ##TAG##1 deb
#define    SENSOR_PIN  6 //a completer

// Define the sampling parameters
//---------------------------------------------------------------
//  Choose the sampling frequency 
//  Integer value in "Hz" for ECG_SENSOR,  in "KHz" for others sensors.
#define   SAMPLE_FREQ   1000
//  Choose the size of samples buffer
// Depending on your project, determine the number to have 
// the desired recording duration   /* value between 1000 and 4000  */
#define   SIZE_BUFF   1024
#define   SIZE_AUTOCORR (SIZE_BUFF/2)
// ##TAG##1 end


// ##TAG##4 deb
// You must determine the power measured with no Signal
// this is the low level value in the below line
#define   LOW_LEVEL   5.0

// You must determine the power measured with the maximum Signal
#define   MAX_LEVEL   20.0

// You have to choose the average acceptable value of power
// It can be x percent of the MAX_LEVEL
#define   AVR_LEVEL   (MAX_LEVEL * 0.6)
// ##TAG##4 end


void  Sensor_Init(void)
//-----------------------------------------------------
// Initialization function 
//-----------------------------------------------------
{
  // Serial port baud rate = 9600 bauds
  Serial.begin(9600);
  // Initialize the sensor input as an analog input
  pinMode(SENSOR_PIN, INPUT);
}


void  Read_Samples(void);
float myPowerEstimation(void);
short myFreqEstimation( void);


// ##TAG##5 deb
short Sensor_Measure(void)
//-----------------------------------------------------
// Main Fonction 
//-----------------------------------------------------
{
  // Déclaration de variables
  float SignalPower;    // puissance du Son
  short freqSig;

  Serial.println(" ");
  Serial.println("--- Signal Power Measurement ---");

  // call function : get buffer samples of signal

  // call function : get power signal
  //SignalPower = ...

  // Display the result
  Serial.print(" Power = ");    Serial.println(SignalPower);
  if (SignalPower < LOW_LEVEL) {
    // Power is too low = no signal
    Serial.println("La puissance du signal est inferieur a la moyenne");
    
    // display an explicit message and turn on the white LED
    freqSig = -1;
  }
  else if (SignalPower > AVR_LEVEL) {
    // Power is too high = sound is painful
    Serial.println("La puissance du signal est supérieur a la moyenne");
    // display an explicit message and turn on the red LED
    freqSig = 0;
  }
  else {
    // acceptable power
    Serial.println("La puissance du signal est correct");
    // display an explicit message and turn on the green LED
    // Determine the signal frequency
    freqSig = 10; //a completer
  }
  return freqSig;
}
// ##TAG##5 end

//  Sampling period in micro-secondes for project with microphone
#define   SAMPLE_PERIOD (1000/SAMPLE_FREQ)
//  Signal samples Buffer 
float Buffer_Sample[SIZE_BUFF];
// Buffer of Instantaneous power
float Power_Inst[SIZE_BUFF];
// Buffer of autocorrelztion
float Buff_AutoCorr[SIZE_AUTOCORR];

// Nombre d'echantillons correspondant K
#define   LEN_K (SAMPLE_FREQ*5)


// ##TAG##3 deb
float myPowerEstimation(void)
//------------------------------------------------------------------------------
//  Read signal and compute mean power
//------------------------------------------------------------------------------
{
  float power;
  // add some local variables
  /*float  ech, pwr_sum, sumfen, pval, val_moy;
  short  n, k, szfen;

  power = 0;
  // center the signal : <signal> = <signal> - "????"
  //
  // compute mean power
  // Calcul de la puissance instantannée en floatant
  pwr_sum = 0.0;    pval = 0.0;
  for (n = 0; n < LEN_K; n++) {
    sumfen = 0.0; szfen = n + LEN_K;
    // .........
    // pval = 
    Power_Inst[n] = pval;
    pwr_sum = pwr_sum + pval;
  }
  for (n = LEN_K; n < (SIZE_BUFF - LEN_K); n++) {
    sumfen = 0.0; szfen = (2*LEN_K) + 1;
    // .........
    // pval = 
    Power_Inst[n] = pval;
    pwr_sum = pwr_sum + pval;
  }
  for (n = SIZE_BUFF - LEN_K; n < SIZE_BUFF; n++) {
    sumfen = 0.0; szfen = SIZE_BUFF - n + LEN_K;
    // .........
    // pval = 
    Power_Inst[n] = pval;
    pwr_sum = pwr_sum + pval;
  }
  // calculer la puissance moyenne
  power = pwr_sum / SIZE_BUFF;*/
  power = 10.0;
  return power;
}
// ##TAG##3 end


short myFreqEstimation(void)
//------------------------------------------------------------------------------
//  compute signal frequency
//  Autocorrelation or FFT
//------------------------------------------------------------------------------
{
  short freqSig;

  freqSig = 0;
  // Implement Signal processing algorithm and

  return freqSig;
}


#define   ADC_MAX_VDD   3.3
#define   ADC_MAX_NUM   4096

// ##TAG##2 deb
void  Read_Samples(void)
//------------------------------------------------------------------------------
// Do Not modify this function. Read and understand
//------------------------------------------------------------------------------
{ float val_float;
  short idx_sample, val_integer;    unsigned long curtime, nextime;

  // See help of 'micros()' function in Energia
  // compute next sampling time (nextime)
  nextime = micros() + SAMPLE_PERIOD;
  idx_sample = 0;
  while (idx_sample < SIZE_BUFF) {
    // wait time of next sample (nextime)
    do {
      curtime = micros();
    } while (curtime < nextime);
    // read sample
    val_integer = analogRead(SENSOR_PIN);
    val_float = ((float)val_integer*ADC_MAX_VDD)/ADC_MAX_NUM;
    Buffer_Sample[idx_sample] = val_float;
    idx_sample++;
    // update next sample time
    nextime += SAMPLE_PERIOD;
  }
  return;
}
// ##TAG##2 end

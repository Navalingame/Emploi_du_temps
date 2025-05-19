#include <Energia.h>

#include "buffer.h"

#include "filter_pb.h"

#include "power_est.h"

#include "dsp_detect.h"                                     

#include "decision.h"

#include "oled_ble.h"
 
// Paramètres projet

const float Fs = 20000.0f;           // 20 kHz

const float f_target = 2500.0f;      // porteuse IR

const float tol_percent = 10.0f;     // ±10%

const float seuil_offset_dB = 2.0f;  // +2 dB
 
// Buffers globaux

extern volatile uint16_t adc_buf[1024];

float filtered_buf[1024];

float power_buf[1024];

float snapshot_buf[1024];

float mag_buf[1024];
 
void setup() {

  Serial.begin(115200);

  initBuffer();      // ADC + DMA ring buffer

  initFilter();      // IIR passe‑bande matériel en logiciel

  initPowerEst(Fs);

  initDSP(1024);

  initDecision(f_target, tol_percent, seuil_offset_dB);

  initOLED_BLE();

}
 
void loop() {
  // 1) Toujours récupérer le snapshot flottant
  snapshotBuffer(adc_buf, snapshot_buf, BUF_SIZE);
 
  // 2) Filtrage numérique
  applyFilter(snapshot_buf, filtered_buf, BUF_SIZE);
 
  // 3) Détection de puissance
  if (powerAboveThreshold(filtered_buf, power_buf, BUF_SIZE)) {
    // 4) On refait un snapshot si besoin, ou on réutilise filtered_buf
 
    float f_est, E_cible;
    runDSP(filtered_buf, BUF_SIZE, Fs, mag_buf, &f_est, &E_cible);
 
    bool ok = makeDecision(f_est, E_cible);
    if (ok) startPursuit();
    else    continueSearch();
    updateOLED_BLE(ok, f_est, E_cible);
  }
} 

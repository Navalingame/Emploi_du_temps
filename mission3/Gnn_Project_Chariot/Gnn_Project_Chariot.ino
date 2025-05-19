//------------------------------------------------------------------------------------
// Gn_Projet_Chariot.c
// 
//------------------------------------------------------------------------------------

#include    "Gnx_Project_Chariot.h"
#include    "OLedAffi_MyLogo.h"


void setup()
//----------------------------------------------------
//----------------------------------------------------
{
    Serial.begin(9600);
    Serial.println("+++ INIT  Debut  ++++");
    pinMode(PIN_LED_R,  OUTPUT);
    pinMode(PIN_LED_G,  OUTPUT);
    pinMode(PIN_LED_B,  OUTPUT);

    pinMode(PIN_POTAR,  INPUT);
    pinMode(PIN_BOUTON, INPUT);

    //Sensor_Init();
   // AOLED_InitScreen();

    delay(1000);
    Serial.println("+++ INIT  Debut  ++++");
    Motor_Init();
    Serial.println("+++ INIT  Fin    ++++");

    
    Motor_Avance(1000);
    delay(1000);

    digitalWrite(PIN_LED_R, HIGH);
    digitalWrite(PIN_LED_B, LOW);
    digitalWrite(PIN_LED_G, LOW);
    Motor_Stop();
    delay(2000);
    AOLED_AffiLogoIsep();
    //AOLED_DisplayImage((char*)Gnn_myLogo);
}

void    Deplacement(short vitesse, short direc, short tourn);

short   sens = 0, old_vit = 0;

void loop()
//----------------------------------------------------
//----------------------------------------------------
{    short valpotar, vitesse, etat, nbtr;

    etat = digitalRead(PIN_BOUTON);
    if (etat == 0) {        // bouton appuyé
        Motor_Stop();
        old_vit = 0;
        while (1) {
            etat = digitalRead(PIN_BOUTON);
            if (etat == 1)  // attendre que le bouton soit relaché
                break;
        }
        sens = (sens + 1) & 1;  // changer 1 en 0 ou 0 en 1
    }
    
    valpotar = analogRead(PIN_POTAR);
    vitesse  = valpotar/700;        // valeur entre 0 et 5
    if (vitesse == old_vit)
        goto sortie;

    Serial.print("    --- new vitesse = ");	Serial.println(vitesse);

    Motor_Avance(1000);
    //Deplacement(vitesse, sens, 0);
    old_vit = vitesse;
sortie:
    //Sensor_Measure();
    delay(500);
}

void    Deplacement(short vitesse, short direc, short tourn)
//----------------------------------------------------
//----------------------------------------------------
{   short ledR, ledV, ledB;

    if (vitesse == 0) {
        ledR = 0;   ledB = 0;   ledV = 0;
        Motor_Stop();
    }
    else if (direc == 0 && tourn == 0) {
        ledR = 0;   ledB = 0;   ledV = 1;
        Motor_Avance(vitesse);
    }
    else if (direc == 1 && tourn == 0) {
        ledR = 0;   ledB = 1;   ledV = 0;
        Motor_Recule(vitesse);
    }
    else if (direc == 0 && tourn == 1) {
        ledR = 1;   ledB = 0;   ledV = 1;
        Motor_TournGL(2);
    }
    else if (direc == 1 && tourn == 1) {
        ledR = 1;   ledB = 1;   ledV = 0;
        Motor_TournDR(2);
    }
    digitalWrite(PIN_LED_R, ledR);
    digitalWrite(PIN_LED_B, ledB);
    digitalWrite(PIN_LED_G, ledV);
}

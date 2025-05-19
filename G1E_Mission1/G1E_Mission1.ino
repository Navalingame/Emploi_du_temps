#include    "Mfnc_Mission1.h"
#include    "Tab_LogoEquipe.h"


void setup()
//----------------------------------------------------
//----------------------------------------------------
{
   Serial.begin(9600);     // Initialise la vitesse de la liaison serie de la console
    Serial1.begin(9600);

    //-------- Init des Ports - Debut ---------
        // ajouter ici les lignes pour configurer le sens des 6 ports
        // definis dans le fichier "Mfnc_Mission1.h"
        // Il faut utiliser les symboles PIN_xxxx
    //-------- Init des Ports - Fin   ---------

    // ajouter ici les lignes pour eteindre les 3 leds RGB

    pinMode(PC_6, INPUT_PULLUP);

    delay(1000);
    Serial.println("+++ INIT AOLED Debut  ++++");

    AOLED_InitScreen();

    // ajouter ici les lignes pour allumer les 3 leds RGB
    
    delay(2000);
        // Enlever le commentaire de la ligne ci-dessous pour
        // afficher votre Logo
    AOLED_DisplayImage((char*)LogoEquipe);
    delay(1000);

    // ajouter ici les lignes pour eteindre les 3 leds RGB

    Serial.println("+++ INIT AOLED Fin    ++++");
}

void  Test_BlueTooth(void);
void  Test_Leds(void);
short Test_Potentiometre(void);
short Test_Temperature(void);
short Test_BoutonPoussoir(void);


// Les textes Ã¯Â¿Â½ afficher sur lecran OLED doivent Ã¯Â¿Â½tre defini en <constant> 
const char  Message[] = "Temperature = ";

short  Counter = 0, valR, valG, valB;

void loop()
//----------------------------------------------------
//----------------------------------------------------
{
    short etatBouton, valTemp, valEntier, valDecim;

        // avec les variables Counter, valR, valG, valB faire un programme pour
        // afficher successivement les 8 couleurs possibles de la led RGB
    //Counter = ??;
    //valR = Counter & 0x??;
    //valG = Counter & 0x??;
    //valB = Counter & 0x??;

    etatBouton = -1;    valTemp = 258;
    // Enlever le commentaire de la ligne ci-dessous pour tester le Bouton Poussoir
    //etatBouton = Test_BoutonPoussoir();
    // Quand le test du Bouton Poussoir est OK, enlever les commentaires des lignes ci-dessous
    // pour tester les composans suivants uniquement quand on on appuie sur le bouton
    //if (etatBouton == ETAT_APPUYE) {
            // enlever un commentaire parmis les trois 
       // Test_BlueTooth();
        Test_Potentiometre();
        //valTemp = Test_Temperature();
            // Afficher la temperature sur l'ecran OLED 
            /*
            valEntier = valTemp / 10;   valDecim = valTemp % 10;
            // Affiche sur la 7e ligne 2e colonne
            AOLED_DisplayTexte(2, 7, (char*)Message);
            AOLED_DisplayIntDeci(-1, -1, valEntier);    // x=y=-1 => a la suite
            AOLED_DisplayCarac(-1, -1, ',');
            AOLED_DisplayIntDeci(-1, -1, valDecim);
            AOLED_DisplayCarac(-1, -1, ' ');
            while (1) {
                etatBouton = Test_BoutonPoussoir();
                if (etatBouton != ETAT_APPUYE)
                    break;
            }
            */
   // }
    //
sortie:
    // afficher valR, valG, valB sur les trois Leds.

    delay(1000);    // repeter toutes les 1 seconde
}


#define  VCC_MILLIV     3300    /* en entier  */
#define  VCC_VOLTS      3.3     /* en flotant */
#define  NUM_MAX        4096

short Test_Temperature(void)
//----------------------------------------------------------------
// Faire les calculs en entier et afficher sous la forme
//    <valeur_entiere>.<1chiffre_decimale>
// Par exemple, pour 24,6 degres, 
//  tempNum = 246, tempEntier = 24, tempDecim = 6
// La division de short donne la valeur entiere
// l operateur % du langage C donne le reste de la division
// Il ne sert a rien d afficher un centieme,millieme de degre
//----------------------------------------------------------------
{
    int    valNum;      // valeur numerique lue
    int    tempNum;     // temperature en dizieme de degre
    short  tempEntier, tempDecim;

       valNum = analogRead(A0);

    float milliVolt = valNum * (1024.0 / 5000.0);
    // convert the voltage to the temperature in Celsius
    tempNum = milliVolt / 10;

    tempEntier = (short)tempNum;   
    tempDecim = (short) milliVolt % 10;

    Serial.print("Temperature ; ");
    Serial.print(tempEntier);
    Serial.print(",");
    Serial.print(tempDecim);
    Serial.print("°C ~ ");
    Serial.println(" ");    // terminer par un retour ligne
    return tempNum;
}

short Test_Potentiometre(void)
//----------------------------------------------------
//----------------------------------------------------
{
    short  valNum = 0;  // valeur numerique lue
    float  Tension;     // tension correspondante
    // ajouter ici le code pour lire et afficher la valeur du 
    // potentiometre ( valeur numerique et tension )

    valNum  = analogRead(A3);     // completer et enlever les commentaires
    Tension = valNum*(3.0/1023.0);
    Serial.print(" Valeur numerique lue =  ");   
    Serial.print(valNum);
    Serial.print(" , Tension correspondante = ");   
    Serial.print(Tension, 2);
    Serial.println("v");    // terminer par un retour ligne
    return valNum;
}

void  Test_BlueTooth(void)
//----------------------------------------------------
// Pour afficher sur la console, on utilise Serial.print()/Serial.println()
// Pour afficher sur le terminal BlueTooth, on utilise
//  Serial1.print()/Serial1.println() (Le module BlueTooth utilise le port serie 1)
// Pour lire depuis le terminal BlueTooth, on utilise
//  Serial1.available() et Serial1.readBytes(buffer, length)
//----------------------------------------------------
{
    // ajouter ici le code pour envoyer et recevoir des messages sur la liaison BlueTooth
    // Envoyer un message quand le bouton est appuye
   char buf[255];
    if (Serial1.available() > 0) {
        int rlen = Serial1.readBytes(buf, 50);
        Serial.print("I received: ");
    }

    int buttonState = digitalRead(PC_6);
    if (buttonState == ETAT_APPUYE)
    {
       Serial1.print("Etat : ");   
       Serial1.println("Appuyer"); 
       Serial.print("Etat : ");   
       Serial.println("Appuyer"); 
    }
    else {
       Serial1.print("Etat : ");   
       Serial1.println("Pas Appuyer"); 
       Serial.print("Etat : ");   
       Serial.println("Pas Appuyer");
    }

}

short Test_BoutonPoussoir(void)
//----------------------------------------------------
//----------------------------------------------------
{
    short etatBouton = -1;
    // ajouter ici le code pour lire et afficher 
    // l etat du bouton poussoir sur une seule ligne

    //etatBouton = ...;     // completer et enlever les commentaires
            // la constatnte ETAT_APPUYE doit etre definie dans le fichier  "Mfnc_Mission1.h"
    // afficher :  valeur lue = etatBouton
    // Si etatBouton = ETAT_APPUYE
    //      afficher etat = Appuye
    // si non
    //      afficher etat = pas Appuye
  
    Serial.println();    // terminer par un retour ligne
    
    return etatBouton;
}

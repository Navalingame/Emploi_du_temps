//-----------------------------------------------------------------------------
//  Mfnc_Mission1.h
//  Ce fichier est a inclure dans tous les fichiers <xx.ino> du projet.
//  Il definit de maniere :
//  les constantes gloales (avec #define) utilisees dans le projet
//  les fonctions  gloales definies dans un fichier et utilisees dans un autre
//-----------------------------------------------------------------------------

#define PIN_LED_R     PF_1
#define PIN_LED_G     PF_2
#define PIN_LED_B     PF_3
#define PIN_POTAR     PE_3		/* potentiometre			*/
#define PIN_BOUTON    PC_6		/* bouton poussoir			*/
#define PIN_TEMPER    PD_0		/* capteur de temperature	*/

// definir si etat du bouton appuye = HIGH ou LOW 
#define ETAT_APPUYE  LOW    

//------------------------------------------------------------------------------------
// Fonctions de l'afficheur OLED
//------------------------------------------------------------------------------------
		// Afficher le meme caractere (value) sur tout l'ecran
void    AOLED_FillScreen(char  value);

		// Afficher une image definie par le buffer <pBuff> de 128x8 octets
void    AOLED_DisplayImage(char *pBuff);

		// Afficher un caratere <car> a l'endroit numCol/numLin
		// numCol = [0 .. 127]      numLin = [0 .. 7]
void    AOLED_DisplayCarac(int numCol, int numLin, char car);

		// Afficher la valeur d'une variable short en decimal a l'endroit numCol/numLin
void    AOLED_DisplayIntDeci(int numCol, int numLin, short value);

		// Afficher la valeur d'une variable short en hexadecimal a l'endroit numCol/numLin
void    AOLED_DisplayIntHexa(int numCol, int numLin, short value);

		// Afficher un texte a partir de numCol/numLin
void    AOLED_DisplayTexte(int numCol, int numLin, char* texte);

		// Inverser les pixels noir <--> blanc 
void    AOLED_InvertDisplay(short invert);

void    AOLED_InitScreen(void);
void    AOLED_AffiLogoIsep(void);





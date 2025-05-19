//------------------------------------------------------------------------------------
// Tiva_TM4C123.h
// 
// Generer deux signaux PWM sur les sorties PB4 et PB5.
// Peroide = 20 ms. Duree impulsion = 1 ms (avant), 1.5 ms (arret) ou 2 ms (arriere).
//
// Dans la fonction setup(), Appeler la fonction Motor_Pwm_Init() pour initialiser 
// le module PWM.
// Dans la fonction loop(),  Appeler la fonction Motor_Pwm_Action(short action) pour
// donner la consigne de direction des moteurs.
//------------------------------------------------------------------------------------

void    Motor_Pwm_Init(void);
void    Motor_Pwm_Action(short action);

#define		MOTOR_STOP			0
#define		MOTOR_AVANCER		1
#define		MOTOR_RECULER		2
#define		MOTOR_AVANT_G		3
#define		MOTOR_AVANT_D		4
#define		MOTOR_RECUL_G		5
#define		MOTOR_RECUL_D		6

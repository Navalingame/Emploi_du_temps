//------------------------------------------------------------------------------------
// Tiva_TM4C123_Pwm.c
// Generer deux signaux PWM sur les sorties PB4 et PB5.
// Peroide = 20 ms. Duree impulsion = 1 ms (avant), 1.5 ms (arret) ou 2 ms (arriere).
//------------------------------------------------------------------------------------

#include    "Tiva_TM4C123.h"


#define SYSCTL_RCC				(*((volatile unsigned int *)0x400FE060))
#define SYSCTL_RCC2				(*((volatile unsigned int *)0x400FE070))
#define SYSCTL_SRPWM			(*((volatile unsigned int *)0x400FE540))
#define SYSCTL_RCGCGPIO			(*((volatile unsigned int *)0x400FE608))
#define SYSCTL_RCGCPWM			(*((volatile unsigned int *)0x400FE640))

#define GPIO_PORTB_AFSEL		(*((volatile unsigned int *)0x40005420))
#define GPIO_PORTB_DEN			(*((volatile unsigned int *)0x4000551C))
#define GPIO_PORTB_PCTL			(*((volatile unsigned int *)0x4000552C))

#define PWM0_ENABLE				(*((volatile unsigned int *)0x40028008))
#define PWM0G1_CTL				(*((volatile unsigned int *)0x40028080))
#define PWM0G1_LOAD				(*((volatile unsigned int *)0x40028090))
#define PWM0G1_CMPA				(*((volatile unsigned int *)0x40028098))
#define PWM0G1_CMPB				(*((volatile unsigned int *)0x4002809C))
#define PWM0G1_GENA				(*((volatile unsigned int *)0x400280A0))
#define PWM0G1_GENB				(*((volatile unsigned int *)0x400280A4))


//-------------------------------------------------
// Clock = 80 MHz/64 = 1.25 MHz --> T = 0.8 us
// Tempo pour avoir une pulse de 1, 1.5, et 2 ms
// Sur une periode de 20 ms
//-------------------------------------------------
#define VALLOAD_20P0MS   25000
#define VCOMPAR_1P0MS    (VALLOAD_20P0MS - 1250)
#define VCOMPAR_1P5MS    (VALLOAD_20P0MS - 1875)
#define VCOMPAR_2P0MS    (VALLOAD_20P0MS - 2500)


void    Motor_Pwm_Action(short action)
//--------------------------------------------------------------------
//--------------------------------------------------------------------
{
    //Motor_Pwm_Reset();
    PWM0G1_CTL    = 0x00;       // Disable Generator 1 counter
    if (action == MOTOR_STOP) {          // arret
        PWM0G1_CMPA = VCOMPAR_1P5MS;        PWM0G1_CMPB = VCOMPAR_1P5MS;
    }
    else if (action == MOTOR_AVANCER) {     // avance
        PWM0G1_CMPA = VCOMPAR_1P0MS;        PWM0G1_CMPB = VCOMPAR_2P0MS;
    }
    else if (action == MOTOR_RECULER) {     // recule
        PWM0G1_CMPA = VCOMPAR_2P0MS;        PWM0G1_CMPB = VCOMPAR_1P0MS;
    }
    else if (action == MOTOR_AVANT_G) {     // AV tourne a gauche
        PWM0G1_CMPA = VCOMPAR_1P5MS;        PWM0G1_CMPB = VCOMPAR_1P0MS;
    }
    else if (action == MOTOR_AVANT_D) {     // AV tourne a droite
        PWM0G1_CMPA = VCOMPAR_1P0MS;        PWM0G1_CMPB = VCOMPAR_1P5MS;
    }
    else if (action == MOTOR_RECUL_G) {     // tourne a gauche
        PWM0G1_CMPA = VCOMPAR_2P0MS;        PWM0G1_CMPB = VCOMPAR_1P5MS;
    }
    else if (action == MOTOR_RECUL_D) {     // tourne a droite
        PWM0G1_CMPA = VCOMPAR_1P5MS;        PWM0G1_CMPB = VCOMPAR_2P0MS;
    }
    else {
        goto sortie;
    }
    PWM0G1_CTL = 0x01;         // Enable Generator 1 counter
sortie:
    return;
}


void    Motor_Pwm_Init(void)
//--------------------------------------------------------------------
//--------------------------------------------------------------------
{   int value;

    // Clock setting for PWM and GPIO PORT
    //--------------------------------------------------------
    SYSCTL_RCC      |= 0x000E0000;          // Use pre-divider valur of 64
    SYSCTL_RCC      |= (1 << 20);           // Enable PWM Clock Divisor function 
    SYSCTL_RCGCPWM  |= 3;                   // Enable clock to PWM0 module
    SYSCTL_RCGCGPIO |= 0x02;                // Enable system clock to PORTB

    // Setting of PB4/PB5 pin for M0PWM2/3 channel output pin
    //--------------------------------------------------------
    GPIO_PORTB_AFSEL |= 0x30;                // PB4,PB5 sets an alternate function
    value = GPIO_PORTB_PCTL;
    value = (value & 0xFF00FFFF) | 0x00440000;
    GPIO_PORTB_PCTL   = value;               // make PB4,PB5 PWM output pin
    GPIO_PORTB_DEN   |= 0x30;                // Enable PB4,PB5 as a digital pin

    delay(10);      // Obligatoire

    PWM0G1_CTL    = 0x00;       // Disable Generator 1 counter
    SYSCTL_SRPWM  = 0x03;       // Reset Module
    delay(2);
    SYSCTL_SRPWM  = 0x00;       // end Reset Module

    delay(10);      // Obligatoire

    PWM0G1_CTL      = 0x00;             // Disable Generator 1 counter
    PWM0G1_LOAD     = VALLOAD_20P0MS;   // set load value for 50Hz : 80 MHz / 64 / 50 = 25.000
    PWM0G1_GENA     = 0x008C;           // Set PWM2(PB4) when reloaded and clear when matches CMPA
    PWM0G1_GENB     = 0x080C;           // Set PWM3(PB5) when reloaded and clear when matches CMPB
    PWM0_ENABLE     = 0x000C;           // Enable PWM2/3
}

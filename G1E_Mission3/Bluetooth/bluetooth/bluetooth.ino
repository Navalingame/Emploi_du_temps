#include    "Gnx_Project_Chariot.h"
#include    "OLedAffi_MyLogo.h"


void setup()
{
    Serial.begin(9600);
    Serial1.begin(9600); 
    Serial.println("+++ INIT  Debut  ++++");
    Serial1.println("BT Ready");
}
void loop()
{
    if (Serial1.available()) {
        char cmd = Serial1.read();
        switch (cmd) {
            case 'F': Motor_Avance(3); break;
            case 'B': Motor_Recule(3); break;
            case 'L': Motor_TournGL(3); break;
            case 'R': Motor_TournDR(3); break;
            case 'S': Motor_Stop(); break;
        }
    }

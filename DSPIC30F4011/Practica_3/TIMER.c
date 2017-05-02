#include <p30f4011.h>
void Init_Timer(void){
T3CONbits.TON = 0;
IEC0bits.T3IE = 0;
IFS0bits.T3IF = 0;
TMR3 = 0;
T3CONbits.TON = 0;
T3CONbits.TSIDL = 0; 
T3CONbits.TGATE = 0;
T3CONbits.TCKPS = 0x0;
T3CONbits.TCS = 0; 
PR3 = 8333;	
T3CONbits.TON = 1;
};

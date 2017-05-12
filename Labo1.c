//
//      Labo1: TMS320F28335
//
//###########################################################################
//
//			allumage LED
//			LED clignote suivant un délai reglé par une boucle
//
//###########################################################################
#include "DSP2833x_Device.h"

void InitSysCtrl(void);
void Gpio_select(void);


//###########################################################################
//					code	main
//###########################################################################
void main(void){
	
	



}



void Gpio_select(void){
	EALLOW;
	GpioCtrlRegs.GPAMUX1.all = ?;		// GPIO15 ... GPIO0 = General Puropse I/O
	GpioCtrlRegs.GPAMUX2.all = ?;		// GPIO31 ... GPIO16 = General Purpose I/O
	GpioCtrlRegs.GPBMUX1.all = ?;		// GPIO47 ... GPIO32 = General Purpose I/O
	GpioCtrlRegs.GPBMUX2.all = ?;		// GPIO63 ... GPIO48 = General Purpose I/O
	GpioCtrlRegs.GPCMUX1.all = ?;		// GPIO79 ... GPIO64 = General Purpose I/O
	GpioCtrlRegs.GPCMUX2.all = ?;		// GPIO87 ... GPIO80 = General Purpose I/O

	GpioCtrlRegs.GPADIR.all = ?;

	GpioCtrlRegs.GPBDIR.all = ?;		// 
	GpioCtrlRegs.GPBDIR.bit.GPIO32 = ?;	//  LED LD3 at GPIO32
	GpioCtrlRegs.GPCDIR.all = ?;		// 
	EDIS;
}   

//===========================================================================
// Fin du code source
//===========================================================================

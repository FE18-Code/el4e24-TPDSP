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
void watchdogEnable(void);
void watchdogDefuse(void);
void wait100ms(void);
void Gpio_select(void);


//###########################################################################
//					code	main
//###########################################################################
void main(void){
	
	InitSysCtrl();
	watchdogEnable();
	watchdogDefuse();
	Gpio_select();

	while(1){
		GpioDataRegs.GPBTOGGLE.bit.GPIO32 = 1 ;
		wait100ms();
	}

}

void watchdogEnable(){
	EALLOW; /* unprotect registers */
	SysCtrlRegs.WDCR = 0x00AF; /* WD enable */
	EDIS; /* reprotect registers */
}

void watchdogDefuse(){
	/* reset watchdog timer so prevents from triggering reset */
	EALLOW;
	SysCtrlRegs.WDKEY=0x55;
	SysCtrlRegs.WDKEY=0xAA;
	EDIS;
}

void wait100ms(){
	long i;

	for(i=0;i<577000;i++){
		asm (" NOP ");
		asm (" NOP ");
		watchdogDefuse();
	}
}

void Gpio_select(void){
	EALLOW;
	GpioCtrlRegs.GPAMUX1.all = 00;		// GPIO15 ... GPIO0 = General Puropse I/O
	GpioCtrlRegs.GPAMUX2.all = 00;		// GPIO31 ... GPIO16 = General Purpose I/O
	GpioCtrlRegs.GPBMUX1.all = 00;		// GPIO47 ... GPIO32 = General Purpose I/O
	GpioCtrlRegs.GPBMUX2.all = 00;		// GPIO63 ... GPIO48 = General Purpose I/O
	GpioCtrlRegs.GPCMUX1.all = 00;		// GPIO79 ... GPIO64 = General Purpose I/O
	GpioCtrlRegs.GPCMUX2.all = 00;		// GPIO87 ... GPIO80 = General Purpose I/O

	GpioCtrlRegs.GPADIR.all = 0;

	GpioCtrlRegs.GPBDIR.all = 0;		//
	GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;	//  LED LD3 at GPIO32
	GpioCtrlRegs.GPCDIR.all = 0;		//
	EDIS;
}   

//===========================================================================
// Fin du code source
//===========================================================================

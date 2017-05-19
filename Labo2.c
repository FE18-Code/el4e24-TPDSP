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

/* custom definitions */
#define DSP_CLOCK_SPEED_MHZ 150
#define TIMER0_PERIOD_US (float)100000

/* ext functions prototypes */
void InitSysCtrl(void);
void InitPieCtrl(void);
void InitPieVectTable (void);
void ConfigCpuTimer(struct CPUTIMER_VARS *, float, float);
void InitCpuTimers(void);

/* functions prototypes */
void timerConfig(float timer_period_us);
void connectITFunctions(void);
void watchdogEnable(void);
void watchdogDefuse(void);
void wait100ms(void);
void Gpio_select(void);
void ePWM_Config (void);
void ePWM_SetSync(void);

/* it functions prototypes*/
interrupt void cpu_timer0_isr(void);

//###########################################################################
//					code	main
//###########################################################################
void main(void){
	
	InitSysCtrl();

	watchdogEnable();
	watchdogDefuse();

	Gpio_select();
	ePWM_Config();
	InitPieCtrl(); /* init ITs */
	InitPieVectTable(); /* Cette fonction TI va initialiser la mémoire du module PIE. */
	connectITFunctions(); /* branch functions to interruptions */

	timerConfig(TIMER0_PERIOD_US);
	IER |= 1; /* INT1 enable */
	EINT; /* all ITs allowed */
	CpuTimer0Regs.TCR.bit.TSS = 0; /* timer start */

	while(1){
		/* loop */
	}

}/* main */


/* ITs */

interrupt void cpu_timer0_isr(void){
	CpuTimer0.InterruptCount++;
	GpioDataRegs.GPBTOGGLE.bit.GPIO32 = 1 ;
	watchdogDefuse();

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; /* it ACK : must be set to allow further calls to cpu_timer0_isr */
}

void connectITFunctions(){
	EALLOW;
	PieVectTable.TINT0 = & cpu_timer0_isr;
	EDIS;
}


/* Timer */

void timerConfig(float timer_period_us){
	InitCpuTimers();
	ConfigCpuTimer(& CpuTimer0, DSP_CLOCK_SPEED_MHZ, timer_period_us);
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1; /* INT1 timer enable */
}


/* Watchdog */

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


/* Utils */

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

	GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1; // enable PWM 1A
	GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1; // enable PWM 2A
	GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1; // enable PWM 3A

	GpioCtrlRegs.GPADIR.all = 0;

	GpioCtrlRegs.GPBDIR.all = 0;		//
	GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;	//  LED LD3 at GPIO32
	GpioCtrlRegs.GPCDIR.all = 0;		//
	EDIS;
}   

/* PWM */

void ePWM_Config (void) {
	/* PWM 1A */
	EPwm1Regs.TBCTL.bit.CLKDIV = 1;
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0;
	EPwm1Regs.TBCTL.bit.CTRMODE = 2; // Mode up/down
	EPwm1Regs.TBPRD = 37500;
	EPwm1Regs.AQCTLA.all = 6; // zero = set; period = clear

	/* PWM 2A */
	EPwm2Regs.TBCTL.bit.CLKDIV = 1;
	EPwm2Regs.TBCTL.bit.HSPCLKDIV = 0;
	EPwm2Regs.TBCTL.bit.CTRMODE = 2; // Mode up/down
	EPwm2Regs.TBPRD = 37500;
	EPwm2Regs.AQCTLA.all = 6; // zero = set; period = clear

	/* PWM 3A */
	EPwm3Regs.TBCTL.bit.CLKDIV = 1;
	EPwm3Regs.TBCTL.bit.HSPCLKDIV = 0;
	EPwm3Regs.TBCTL.bit.CTRMODE = 2; // Mode up/down
	EPwm3Regs.TBPRD = 37500;
	EPwm3Regs.AQCTLA.all = 6; // zero = set; period = clear

	ePWM_SetSync();
}

void ePWM_SetSync(void) {
	/* PWM1 SYNC OUT */
	EPwm1Regs.TBCTL.bit.SYNCOSEL = 01; // Sync Out Select : generates sync signal when CTR = 0

	/* PWM2 SYNC IN */
	EPwm2Regs.TBCTL.bit.PHSEN = 1; // Validation du reglage de phase : CTR=TBPHS on EPWNxSYNCI signal
	EPwm2Regs.TBPHS.half.TBPHS = 12500; // 1/3 de TBPRD

	/* PWM2 SYNC OUT */
	EPwm2Regs.TBCTL.bit.SYNCOSEL = 00; // Sync Out Select: SYNCIN = SYNCOUT generates sync signal when SYNCIN is received

	/* PWM 3 SYNC IN */
	EPwm3Regs.TBCTL.bit.PHSEN = 1; // Validation du reglage de phase : CTR=TBPHS on EPWNxSYNCI signal
	EPwm3Regs.TBPHS.half.TBPHS = 25000; // 2/3 de TBPRD
}

//===========================================================================
// Fin du code source
//===========================================================================

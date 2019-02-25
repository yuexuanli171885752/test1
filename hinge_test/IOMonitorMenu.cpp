#include "interface.h"
#include "inf_motion_aa.h"
#include "test_reg_mark_chart.h"
#include "defio.h"

extern MOTION_SHM mshm;
extern void IO_port0_monitor(int Number);
extern void IO_port1_monitor(int Number);
extern void IO_port2_monitor(int Number);
extern void IO_port3_monitor(int Number);


// ===== IO monitor menu =====================================
void IO_monitor_menu(int Number)
{
int detect = 1;
     
//	number = 0;
	while(detect)
	{ 
		Sleep(20);
		number=Number;
 		if(number==0x00) detect = 0;
 		else { Number=0x00; }
		log("init:button_val:%d", number);

		if((number >= 400)&&(number <= 431)){
			IO_port0_monitor(number);
		}
		else if((number >= 432)&&(number <= 463)){
			IO_port1_monitor(number);
		}
		else if((number >= 464)&&(number <= 495)){
			IO_port2_monitor(number);
		}
		else if((number >= 496)&&(number <= 527)){
			IO_port3_monitor(number);
		}
		else{	// exit to main menu
			detect = 0;
		}
	}  // end of while-loop 
	return;
}

void IO_port0_monitor(int Number)
{
	MANUAL_MOVING = ON;
	Sleep(50);
	switch(Number){
	case 400:	Y01_ON;	 break;
	case 401:	Y01_OFF; break;
		
	case 402:	Y02_ON;  break;
	case 403:	Y02_OFF; break;
		
	case 404:	Y03_ON;  break;
	case 405:	Y03_OFF; break;
		
	case 406:	Y04_ON;  break;
	case 407:	Y04_OFF; break;
		
	case 408:	Y05_ON;  break;
	case 409:	Y05_OFF; break;
		
	case 410:	Y06_ON;  break;
	case 411:	Y06_OFF; break;
		
	case 412:	Y07_ON;  break;
	case 413:	Y07_OFF; break;
		
	case 414:	Y08_ON;  break;
	case 415:	Y08_OFF; break;

	case 416:	Y09_ON;	 break;
	case 417:	Y09_OFF; break;
		
	case 418:	Y10_ON;  break;
	case 419:	Y10_OFF; break;

	case 420:	Y11_ON;  break;
	case 421:	Y11_OFF; break;

	case 422:	Y12_ON;  break;
	case 423:	Y12_OFF; break;

	case 424:	Y13_ON;  break;
	case 425:	Y13_OFF; break;

	case 426:	Y14_ON;  break;
	case 427:	Y14_OFF; break;

	case 428:	Y15_ON;  break;
	case 429:	Y15_OFF; break;

	case 430:	Y16_ON;  break;
	case 431:	Y16_OFF; break;

	}  // end of switch case
	MANUAL_ERROR  = OFF;
	MANUAL_MOVING = OFF;
	return;
}

void IO_port1_monitor(int Number)
{
	MANUAL_MOVING = ON;
	Sleep(50);
	switch(Number){
	case 432:	Y17_ON;	 break;
	case 433:	Y17_OFF; break;

	case 434:	Y18_ON;  break;
	case 435:	Y18_OFF; break;

	case 436:	Y19_ON;  break;
	case 437:	Y19_OFF; break;

	case 438:	Y20_ON;  break;
	case 439:	Y20_OFF; break;

	case 440:	Y21_ON;  break;
	case 441:	Y21_OFF; break;

	case 442:	Y22_ON;  break;
	case 443:	Y22_OFF; break;

	case 444:	Y23_ON;  break;
	case 445:	Y23_OFF; break;

	case 446:	Y24_ON;  break;
	case 447:	Y24_OFF; break;

	case 448:	Y25_ON;	 break;
	case 449:	Y25_OFF; break;

	case 450:	Y26_ON;  break;
	case 451:	Y26_OFF; break;

	case 452:	Y27_ON;  break;
	case 453:	Y27_OFF; break;

	case 454:	Y28_ON;  break;
	case 455:	Y28_OFF; break;

	case 456:	Y29_ON;  break;
	case 457:	Y29_OFF; break;

	case 458:	Y30_ON;  break;
	case 459:	Y30_OFF; break;

	case 460:	Y31_ON;  break;
	case 461:	Y31_OFF; break;

	case 462:	Y32_ON;  break;
	case 463:	Y32_OFF; break;
	}  // end of switch case
	MANUAL_ERROR  = OFF;
	MANUAL_MOVING = OFF;
	return;
}

void IO_port2_monitor(int Number)
{
	MANUAL_MOVING = ON;
	Sleep(50);
	switch(Number){
	case 464:	Y33_ON;	 break;
	case 465:	Y33_OFF; break;

	case 466:	Y34_ON;  break;
	case 467:	Y34_OFF; break;

	case 468:	Y35_ON;  break;
	case 469:	Y35_OFF; break;

	case 470:	Y36_ON;  break;
	case 471:	Y36_OFF; break;

	case 472:	Y37_ON;  break;
	case 473:	Y37_OFF; break;

	case 474:	Y38_ON;  break;
	case 475:	Y38_OFF; break;

	case 476:	Y39_ON;  break;
	case 477:	Y39_OFF; break;

	case 478:	Y40_ON;  break;
	case 479:	Y40_OFF; break;

	case 480:	Y41_ON;	 break;
	case 481:	Y41_OFF; break;

	case 482:	Y42_ON;  break;
	case 483:	Y42_OFF; break;

	case 484:	Y43_ON;  break;
	case 485:	Y43_OFF; break;

	case 486:	Y44_ON;  break;
	case 487:	Y44_OFF; break;

	case 488:	Y45_ON;  break;
	case 489:	Y45_OFF; break;

	case 490:	Y46_ON;  break;
	case 491:	Y46_OFF; break;

	case 492:	Y47_ON;  break;
	case 493:	Y47_OFF; break;

	case 494:	Y48_ON;  break;
	case 495:	Y48_OFF; break;

	}  // end of switch case
	MANUAL_ERROR  = OFF;
	MANUAL_MOVING = OFF;
	return;
}

void IO_port3_monitor(int Number)
{
	MANUAL_MOVING = ON;
	Sleep(50);
	switch(Number){
	case 496:	Y49_ON;	 break;
	case 497:	Y49_OFF; break;

	case 498:	Y50_ON;  break;
	case 499:	Y50_OFF; break;

	case 500:	Y51_ON;  break;
	case 501:	Y51_OFF; break;

	case 502:	Y52_ON;  break;
	case 503:	Y52_OFF; break;

	case 504:	Y53_ON;  break;
	case 505:	Y53_OFF; break;

	case 506:	Y54_ON;  break;
	case 507:	Y54_OFF; break;

	case 508:	Y55_ON;  break;
	case 509:	Y55_OFF; break;

	case 510:	Y56_ON;  break;
	case 511:	Y56_OFF; break;

	case 512:	Y57_ON;	 break;
	case 513:	Y57_OFF; break;

	case 514:	Y58_ON;  break;
	case 515:	Y58_OFF; break;

	case 516:	Y59_ON;  break;
	case 517:	Y59_OFF; break;

	case 518:	Y60_ON;  break;
	case 519:	Y60_OFF; break;

	case 520:	Y61_ON;  break;
	case 521:	Y61_OFF; break;

	case 522:	Y62_ON;  break;
	case 523:	Y62_OFF; break;

	case 524:	Y63_ON;  break;
	case 525:	Y63_OFF; break;

	case 526:	Y64_ON;  break;
	case 527:	Y64_OFF; break;
		

	}  // end of switch case
	MANUAL_ERROR  = OFF;
	MANUAL_MOVING = OFF;
	return;
}


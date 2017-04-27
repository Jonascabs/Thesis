#ifndef PROTOCOL_TIMER_H
#define PROTOCOL_TIMER_H


#include <Arduino.h>
#include <TimerOne.h>

class PROTOCOL_TIMER{ 
public:
	PROTOCOL_TIMER();
	unsigned long rx_rts_timer();
	unsigned long rx_cts_timer();
	unsigned long rx_rts_reset();
	unsigned long rx_cts_reset();
	unsigned long rx_packet_ack_timer();
	unsigned long rx_packet_timer();
private:		
	static int display(unsigned long time);
	static unsigned long count_timer();
};

//extern PROTOCOL_TIMER timer;
#endif
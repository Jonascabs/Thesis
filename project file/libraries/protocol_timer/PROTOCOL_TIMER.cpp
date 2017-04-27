#ifndef PROTOCOL_TIMER_cpp
#define PROTOCOL_TIMER_cpp

#include <TimerOne.h>
#include <PROTOCOL_TIMER.h>

const int led = LED_BUILTIN;




PROTOCOL_TIMER::PROTOCOL_TIMER(){
	Timer1.initialize(100000);
}

 int ledstate = LOW;
volatile unsigned long timeCount;
volatile unsigned long timeCopy;

static int PROTOCOL_TIMER::display(unsigned long time)
{
	  noInterrupts();
	  timeCopy = time;
	  interrupts();
  
  
	Serial.print("Timer:");
	Serial.println(timeCopy);

}


static unsigned long PROTOCOL_TIMER::count_timer()
	{
		int ledstate = LOW;
		
		if (ledstate == LOW){
		ledstate = HIGH;
		timeCount++;
		}else{
		ledstate = LOW;
		}
		digitalWrite(led, ledstate);
		display(timeCount++);
	}	



unsigned long PROTOCOL_TIMER::rx_rts_timer()
{
	pinMode(led, OUTPUT);
	Timer1.initialize(1000000);
	Timer1.attachInterrupt(count_timer);
	return (timeCopy);
	
}

unsigned long PROTOCOL_TIMER::rx_cts_timer()
{
	pinMode(led, OUTPUT);
	Timer1.initialize(1000000);
	Timer1.attachInterrupt(count_timer);
	return (timeCopy);
	
}

unsigned long PROTOCOL_TIMER::rx_packet_timer()
{
	pinMode(led, OUTPUT);
	Timer1.initialize(1000000);
	Timer1.attachInterrupt(count_timer);
	return (timeCopy);
	
}





unsigned long PROTOCOL_TIMER::rx_packet_ack_timer(){
	pinMode(led, OUTPUT);
	Timer1.initialize(1000000);
	Timer1.attachInterrupt(count_timer);
	return (timeCopy);
	}
	
unsigned long PROTOCOL_TIMER::rx_cts_reset()
{
	Timer1.stop();
	Timer1.detachInterrupt();
	timeCopy = 0;
}

unsigned long PROTOCOL_TIMER::rx_rts_reset()
{
	Timer1.stop();
	Timer1.detachInterrupt();
	timeCopy = 0;
}


#endif

#include <8051.h>
#include "preemptive.h"



/* 
 * @@@ [2pt] 
 * declare your global variables here, for the shared buffer 
 * between the producer and consumer.  
 * Hint: you may want to manually designate the location for the 
 * variable.  you can use
 *        __data __at (0x30) type var; 
 * to declare a variable var of the type
 */
__data __at (0x27) char buffer;
__data __at (0x28) char Token;
//__data __at (0x29) char is_buffer_full;
__data __at (0x2A) char full;
__data __at (0x2B) char mutex;
__data __at (0x2C) char empty;
__data __at (0x2D) char Token1;
__data __at (0x26) char abc;
__data __at (0x25) char num;
__data __at (0x24) char _i;
#define merge(a,b) a ## b
#define leftbracket (
#define conca(x) merge leftbracket x, $)

void Producer1(void) {
                Token = 'A';
                while (1) {
                        SemaphoreWaitBody(abc, conca(__COUNTER__) );
                        SemaphoreWaitBody(empty, conca(__COUNTER__) );
                        SemaphoreWaitBody(mutex, conca(__COUNTER__) );

                        __critical{
                                buffer = Token;
                                Token = ( Token == 'Z' ) ? 'A' :  Token + 1;
                                
                        }
                        
                        SemaphoreSignal(mutex);
                        SemaphoreSignal(full);
                        SemaphoreSignal(num);
                        
                }
}
void Producer2(void) {
                Token1 = '0';
                while (1) {
                        SemaphoreWaitBody(num, conca(__COUNTER__) );
                        SemaphoreWaitBody(empty, conca(__COUNTER__) );
                        SemaphoreWaitBody(mutex, conca(__COUNTER__) );
                        __critical{

                                buffer = Token1;
                                Token1 = ( Token1 == '9' ) ? '0' :  Token1 + 1;  
                                
                        }
                        
                        SemaphoreSignal(mutex);
                        SemaphoreSignal(full);
                        SemaphoreSignal(abc);
                        
                }
}
/* [10 pts for this function]
 * the consumer in this test program gets the next item from
 * the queue and consume it and writes it to the serial port.
 * The Consumer also does not return.
 */

void Consumer(void) {
        /* @@@ [2 pt] initialize Tx for polling */
        TMOD |= 0x20;
        TH1 = -6;
        SCON = 0x50;
        TR1 = 1;
        
            while (1) {
                /* @@@ [2 pt] wait for new data from producer
                 * @@@ [6 pt] write data to serial port Tx, 
                 * poll for Tx to finish writing (TI),
                 * then clear the flag
                 */
                SemaphoreWaitBody(full,  conca(__COUNTER__));
                SemaphoreWaitBody(mutex,  conca(__COUNTER__));
                __critical{
                        SBUF = buffer;
                        //is_buffer_full = 0;
                }
                SemaphoreSignal(mutex);
                SemaphoreSignal(empty);
                while( !TI ){
			//ThreadYield();
		}
                TI = 0;
                 

        }   
        
        
}

/* [5 pts for this function]
 * main() is started by the thread bootstrapper as thread-0.
 * It can create more thread(s) as needed:
 * one thread can acts as producer and another as consumer.
 */
void main(void) {
        
        SemaphoreCreate(full, 0);
        SemaphoreCreate(mutex, 1);
        SemaphoreCreate(empty,1);
        SemaphoreCreate(abc,0);
        SemaphoreCreate(num,1);
        ThreadCreate( Producer1 );
        ThreadCreate( Producer2 );
        
        Consumer();     
}

void _sdcc_gsinit_startup(void) {
        __asm
            ljmp  _Bootstrap
        __endasm;
}


void _mcs51_genRAMCLEAR(void) {}
void _mcs51_genXINIT(void) {}
void _mcs51_genXRAMCLEAR(void) {}
void timer0_ISR(void) __interrupt(1) {
        __asm
            ljmp _myTimer0Handler
        __endasm;
}
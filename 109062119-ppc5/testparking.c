
#include <8051.h>
#include "preemptive.h"



__data __at (0x27) char id;
__data __at (0x28) char spot1;
//__data __at (0x29) char is_buffer_full;
__data __at (0x2A) char done;
__data __at (0x2B) char mutex;
__data __at (0x2C) char empty;
__data __at (0x2D) char spot2;
__data __at (0x26) char car_thread_id;
__data __at (0x25) char num;
__data __at (0x24) char car;


#define merge(a,b) a ## b
#define leftbracket (
#define conca(x) merge leftbracket x, $)

#define print(a, b, c)\
    TMOD |= 0x20;\
    TH1 = -6;\
    SCON = 0x50;\
    TR1 = 1;\
    for(id = 0; id < 5 ; id++){\
        if( id==0 ) SBUF = a;\
        else if( id == 1 ) SBUF = b;\
        else if( id == 2 ) SBUF = c;\
        else if( id == 3 ) SBUF = (time%10) + '0';\
        else if( id == 4) SBUF = '\n';\
        while( !TI ){}\
        TI = 0;\
    }\

void Producer1(void) {
        
                SemaphoreWaitBody(empty, conca(__COUNTER__) );
                SemaphoreWaitBody(mutex, conca(__COUNTER__) );
                EA = 0;
                
                        //find a space
                        if(spot1 == 'x') {
                                spot1 = '1';
                                print('1','i','1');
                        } else if(spot2 == 'x'){
                                spot2 = '1';
                                print('1','i','2');
                        }       
                
                EA = 1;
                SemaphoreSignal(mutex);
                delay(1);
                EA = 0;
                
                        //leave 
                        if(spot1 == '1') {
                                spot1 = 'x';
                                print('1','o','1');
                        } else if(spot2 == '1') {
                                spot2 = 'x';
                                print('1','o','2');
                        }
                
                EA = 1;
                SemaphoreSignal(empty);
                SemaphoreSignal(done);
                ThreadExit();
        
}

void Producer2(void) {
        
                SemaphoreWaitBody(empty, conca(__COUNTER__) );
                SemaphoreWaitBody(mutex, conca(__COUNTER__) );
                EA = 0;
                
                        //find a space
                        if(spot1 == 'x') {
                                spot1 = '2';
                                print('2','i','1');
                        } else if(spot2 == 'x'){
                                spot2 = '2';
                                print('2','i','2');
                        }       
                
                SemaphoreSignal(mutex);
                EA = 1;
                delay(2);
                EA = 0;
                
                        //leave 
                        if(spot1 == '2') {
                                spot1 = 'x';
                                print('2','o','1');
                        } else if(spot2 == '2') {
                                spot2 = 'x';
                                print('2','o','2');
                        }
                
                EA = 1;
                SemaphoreSignal(empty);
                SemaphoreSignal(done);
                ThreadExit();
        
}

void Producer3(void) {
        
                SemaphoreWaitBody(empty, conca(__COUNTER__) );
                SemaphoreWaitBody(mutex, conca(__COUNTER__) );
                EA = 0;
                
                        //find a space
                        if(spot1 == 'x') {
                                spot1 = '3';
                                print('3','i','1');
                        } else if(spot2 == 'x'){
                                spot2 = '3';
                                print('3','i','2');
                        }       
                
                EA = 1;
                SemaphoreSignal(mutex);
                delay(3);
                EA = 0;
                
                        //leave 
                        if(spot1 == '3') {
                                spot1 = 'x';
                                print('3','o','1');
                        } else if(spot2 == '3') {
                                spot2 = 'x';
                                print('3','o','2');
                        }
                
                EA = 1;
                SemaphoreSignal(empty);
                SemaphoreSignal(done);
                ThreadExit();
        
}

void Producer4(void) {
        
                SemaphoreWaitBody(empty, conca(__COUNTER__) );
                SemaphoreWaitBody(mutex, conca(__COUNTER__) );
                EA = 0;
                
                        //find a space
                        if(spot1 == 'x') {
                                spot1 = '4';
                                print('4','i','1');
                        } else if(spot2 == 'x'){
                                spot2 = '4';
                                print('4','i','2');
                        }       
                
                EA = 1;
                SemaphoreSignal(mutex);
                delay(4);
                EA = 0;
                
                        //leave 
                        if(spot1 == '4') {
                                spot1 = 'x';
                                print('4','o','1');
                        } else if(spot2 == '4') {
                                spot2 = 'x';
                                print('4','o','2');
                        }
                
                EA = 1;
                SemaphoreSignal(empty);
                SemaphoreSignal(done);
                ThreadExit();
        
}

void Producer5(void) {
        
                SemaphoreWaitBody(empty, conca(__COUNTER__) );
                SemaphoreWaitBody(mutex, conca(__COUNTER__) );
                EA = 0;
                
                        //find a space
                        if(spot1 == 'x') {
                                spot1 = '5';
                                print('5','i','1');
                        } else if(spot2 == 'x'){
                                spot2 = '5';
                                print('5','i','2');
                        }       
                
                EA = 1;
                SemaphoreSignal(mutex);
                delay(5);
                EA = 0;
                
                        //leave 
                        if(spot1 == '5') {
                                spot1 = 'x';
                                print('5','o','1');
                        } else if(spot2 == '5') {
                                spot2 = 'x';
                                print('5','o','2');
                        }
                
                EA = 1;
                SemaphoreSignal(empty);
                SemaphoreSignal(done);
                ThreadExit();
        
}


void main(void) {
        
        SemaphoreCreate(done, 0);
        SemaphoreCreate(mutex, 1);
        SemaphoreCreate(empty,2);
        spot1 = 'x';
        spot2 = 'x';
        car = '1';
        EA = 1;
        ThreadCreate( Producer1 );
        ThreadCreate( Producer2 );
        ThreadCreate( Producer3 );
        // SemaphoreWaitBody(done, conca(__COUNTER__));
        // ThreadCreate(Producer4);
        // SemaphoreWaitBody(done, conca(__COUNTER__));
        // ThreadCreate(Producer5);
        while(time < 0x06){
                SemaphoreWaitBody(done, conca(__COUNTER__));
                ThreadCreate(Producer3);
                SemaphoreWaitBody(done, conca(__COUNTER__));
                ThreadCreate(Producer4);
                SemaphoreWaitBody(done, conca(__COUNTER__));
                ThreadCreate(Producer5);
                SemaphoreWaitBody(done, conca(__COUNTER__));
                ThreadCreate(Producer1);
                SemaphoreWaitBody(done, conca(__COUNTER__));
                ThreadCreate(Producer2);
        }
        
        //print('e','n','d');
        ThreadExit();
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
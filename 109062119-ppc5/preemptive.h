
#ifndef __PREEMPTIVE_H__
#define __PREEMPTIVE_H__

#define MAXTHREADS 4  /* not including the scheduler */


__data __at (0x36) unsigned char time_temp[4];
__data __at (0x3E) unsigned char time_s;
__data __at (0x3F) unsigned char time;
__data __at (0x34) char cur_thread;
/* the scheduler does not take up a thread of its own */
#define CNAME(s) _ ## s
#define delay(n)\
        time_temp[cur_thread] = time + n;\
        while( time_temp[cur_thread] != time ){}\
//create a counting semaphore s that is initialized to n
#define SemaphoreCreate(s, n)\
            s = n\


#define SemaphoreWaitBody(s, label)\
    { __asm \
        label:\
        MOV A, CNAME(s)\
        JB ACC.7, label \
        JZ  label\
        DEC  CNAME(s) \
      __endasm; }


#define SemaphoreSignal(s)\
            __asm \
            INC CNAME(s)\
            __endasm;\

typedef char ThreadID;
typedef void (*FunctionPtr)(void);

ThreadID ThreadCreate(FunctionPtr);
void ThreadYield(void);
void ThreadExit(void);
void myTimer0Handler();


unsigned char now(void);
//void delay(unsigned char n);


#endif // __PREEMPTIVE_H__

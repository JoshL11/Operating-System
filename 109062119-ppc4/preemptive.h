
#ifndef __PREEMPTIVE_H__
#define __PREEMPTIVE_H__

#define MAXTHREADS 4  /* not including the scheduler */
/* the scheduler does not take up a thread of its own */
#define CNAME(s) _ ## s

// create a counting semaphore s that is initialized to n
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
#endif // __PREEMPTIVE_H__
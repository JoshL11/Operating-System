#include <8051.h>

#include "preemptive.h"

/*
 * @@@ [2 pts] declare the static globals here using 
 *        __data __at (address) type name; syntax
 * manually allocate the addresses of these variables, for
 * - saved stack pointers (MAXTHREADS)
 * - current thread ID
 * - a bitmap for which thread ID is a valid thread; 
 *   maybe also a count, but strictly speaking not necessary
 * - plus any temporaries that you need.
 */
__data __at (0x30) char saved_sp[4];

__data __at (0x35) char i;

__data __at (0x3A) char sp_temp;
__data __at (0x3B) char new_thread;
__data __at (0x3C) char temp;
__data __at (0x3D) char mask;


/*
 * @@@ [8 pts]
 * define a macro for saving the context of the current thread by
 * 1) push ACC, B register, Data pointer registers (DPL, DPH), PSW
 * 2) save SP into the saved Stack Pointers array
 *   as indexed by the current thread ID.
 * Note that 1) should be written in assembly, 
 *     while 2) can be written in either assembly or C
 */
#define SAVESTATE \
         __asm \
         PUSH ACC\
         PUSH B\
         PUSH DPL\
         PUSH DPH\
         PUSH PSW\
         __endasm;\
         saved_sp[cur_thread] = SP;\
        

/*
 * @@@ [8 pts]
 * define a macro for restoring the context of the current thread by
 * essentially doing the reverse of SAVESTATE:
 * 1) assign SP to the saved SP from the saved stack pointer array
 * 2) pop the registers PSW, data pointer registers, B reg, and ACC
 * Again, popping must be done in assembly but restoring SP can be
 * done in either C or assembly.
 */
#define RESTORESTATE \
            SP = saved_sp[cur_thread];\
            __asm \
            POP PSW\
            POP DPH\
            POP DPL\
            POP B\
            POP ACC\
            __endasm; \
         

 /* 
  * we declare main() as an extern so we can reference its symbol
  * when creating a thread for it.
  */

extern void main(void);

/*
 * Bootstrap is jumped to by the startup code to make the thread for
 * main, and restore its context so the thread can run.
 */

void Bootstrap(void) {
      /*
       * @@@ [2 pts] 
       * initialize data structures for threads (e.g., mask)
       *
       * optional: move the stack pointer to some known location
       * only during bootstrapping. by default, SP is 0x07.
       *
       * @@@ [2 pts]
       *     create a thread for main; be sure current thread is
       *     set to this thread ID, and restore its context,
       *     so that it starts running main().
       */
      mask = 0;
      TMOD = 0;  // timer 0 mode 0
      IE = 0x82;  // enable timer 0 interrupt; keep consumer polling
                // EA  -  ET2  ES  ET1  EX1  ET0  EX0 
      TR0 = 1; // set bit TR0 to start running timer 0
      cur_thread = ThreadCreate( main );
      RESTORESTATE;
}

// void delay(unsigned char n){
//    time_temp[cur_thread] = time + n;
//    while( time_temp[cur_thread] != time ){}
   
//  }

unsigned char now (void) {
    return time;
}




ThreadID ThreadCreate(FunctionPtr fp) {
      EA = 0;
        if( mask == 15 ) 
            return -1;
        
         
         for(i = 0; i < 4 ; i++){
            temp = 1 << i;
            if( ( mask & temp ) == 0){
               mask |= temp;
               new_thread = i;
               break;
            }
         }
         //a.

         //b.

         //c.
         sp_temp = SP;
         SP = (0x3F) + (0x10) * new_thread;

         //d. e.
         __asm
            PUSH DPL
            PUSH DPH
         __endasm;

         //e.
         __asm
            CLR A
            PUSH ACC
            PUSH ACC
            PUSH ACC
            PUSH ACC
         __endasm;

         //f.
         if(new_thread == 0) PSW = 0;
         else if(new_thread == 1) PSW = 8;
         else if(new_thread == 2) PSW = 16;
         else if(new_thread == 3) PSW = 24;
         __asm 
            PUSH PSW
         __endasm;
         //g.
         saved_sp[new_thread] = SP;
         //h.
         SP = sp_temp;
         //i.
        EA =1;
        return new_thread;
        
       
}








void ThreadExit(void) {
   EA = 0;
    temp = 1 << cur_thread;
    mask ^= temp;
    for(i=0 ; i < 4 ; i++) {
      temp = 1 << i;
      if( mask & temp ){
         cur_thread = i;
         break;
      }
    }
      if(i == 4) {
         while(1){}
      }
      
      RESTORESTATE;
      EA = 1;
   }


void myTimer0Handler(){
   EA = 0;
   SAVESTATE;
   __asm
      PUSH ar0
      PUSH ar1
      PUSH ar2
      PUSH ar3
      PUSH ar4
      PUSH ar5
      PUSH ar6
      PUSH ar7
   __endasm;

   time_s = time_s + 1;
   if(time_s == 16) {
      time = time + 1;
      time_s = 0;
   }
   do {
         if(cur_thread == 3) cur_thread = 0;
         else cur_thread = cur_thread + 1;
         temp = 1 << cur_thread;
         if( mask & temp ){
            break;
         } 
   } while (1);
   __asm
         POP ar7
         POP ar6
         POP ar5
         POP ar4
         POP ar3
         POP ar2
         POP ar1
         POP ar0
   __endasm;  
   RESTORESTATE;
   EA = 1;
   __asm
      RETI
   __endasm;
}




#ifndef __CONFIG_H

#define DBG_TRACE           /* Comment this line to disable trace messages              */
                            /* use this for displaying simple messages                  */
                            /* When using format specifiers use PRINTF instead of TRACE */ 
#define DBG_PRINTF          /* Comment this line to disable printf messages */

#ifdef DBG_TRACE
 #define TRACE(x) serial_trace(x)
#else
 #define TRACE(x)
#endif

#ifdef DBG_PRINTF
  #define PRINTF(str, args...) printf(str, ##args)
#else
  #define PRINTF(str, args...) 
#endif 

#endif

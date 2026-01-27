#ifndef __CPU_USAGE_H__
#define __CPU_USAGE_H__


#include "FreeRTOSConfig.h"
#if configGENERATE_RUN_TIME_STATS
  void ConfigureTimerForRunTimeStats();
  configRUN_TIME_COUNTER_TYPE GetRunTimeCounterValue();

#endif
#endif

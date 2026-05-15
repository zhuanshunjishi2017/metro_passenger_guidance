#ifndef REMIND_H
#define REMIND_H

#include "metro_line.h"
#include "timestruct.h"

#define MAX_REMIND 10

typedef struct
{
  const Station *station;
  int8_t direction;
  TimeStruct start_time;
  uint16_t remind_sec;
  int8_t flag;

} RemindInfo;





#endif

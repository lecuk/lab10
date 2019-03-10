#ifndef DATE_TIME_H
#define DATE_TIME_H

#include <time.h>
#include "tools.h"

struct tm *dateTime_construct(const int d, const int m, const int y);
void dateTime_getTrueDMY(const struct tm* dateTime, int* d, int* m, int* y);
int dateTime_getMonthDayCount(int m, int y);
bool dateTime_isYearLeap(int y);

#endif
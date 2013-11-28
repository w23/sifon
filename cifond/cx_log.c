#include "cx_log.h"
#include <stdarg.h>
#include <time.h>
#include <stdio.h>

#define BUFFER_SIZE 356

int cx_log_level = CX_LOG_LEVEL_WARNING;

void cx_log(int level, const char *format, ...) {
  char buffer[BUFFER_SIZE];
  va_list args;
  int length;
  time_t utime;
  struct tm tm;

  utime = time(NULL);
  localtime_r(&utime, &tm);
  length = snprintf(buffer, BUFFER_SIZE - 1,
    "%d.%02d.%02d %02d:%02d:%02d [%d]: ", tm.tm_year + 1900, tm.tm_mon,
    tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, level);

  va_start(args, format);
  length += vsnprintf(buffer + length, BUFFER_SIZE-length-1, format, args);
  va_end(args);

  buffer[length] = '\n';
  buffer[length+1] = 0;
  fputs(buffer, stderr);
}

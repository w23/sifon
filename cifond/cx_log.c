#include "cx_log.h"
#include <stdarg.h>
#include <time.h>
#include <stdio.h>

#define BUFFER_SIZE 256

int cx_log_level = CX_LOG_LEVEL_WARNING;

void cx_log(int level, const char *format, ...) {
  char buffer[BUFFER_SIZE];
  va_list args;
  int length, left = BUFFER_SIZE - 2;
  time_t utime;
  struct tm tm;

  utime = time(NULL);
  localtime_r(&utime, &tm);
  length = snprintf(buffer, left,
    "%d.%02d.%02d %02d:%02d:%02d [%d]: ", tm.tm_year + 1900, tm.tm_mon,
    tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, level);
  if (length > left) {
    length = BUFFER_SIZE - 2;
    left = 0;
  } else left -= length;

  va_start(args, format);
  length += vsnprintf(buffer + length, left, format, args);
  va_end(args);
  if (length > left) {
    length = BUFFER_SIZE - 2;
    left = 0;
  } else left -= length;

  buffer[length] = '\n';
  buffer[length+1] = 0;
  fputs(buffer, stderr);
}

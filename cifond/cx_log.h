#ifndef __CX_LOG_H__
#define __CX_LOG_H__

void cx_log(int level, const char *format, ...);

extern int cx_log_level;

#define CX_LOG_MESSAGE(level, ...) \
  if ((level) <= cx_log_level) cx_log((level), __VA_ARGS__)

#define CX_LOG_LEVEL_ERROR    3
#define CX_LOG_LEVEL_WARNING  7
#define CX_LOG_LEVEL_INFO     11
#define CX_LOG_LEVEL_DEBUG    15

#define CX_LOG(...) \
  CX_LOG_MESSAGE(0, __VA_ARGS__)
#define CX_LOG_ERROR(...) \
  CX_LOG_MESSAGE(CX_LOG_LEVEL_ERROR, __VA_ARGS__)
#define CX_LOG_WARNING(...) \
  CX_LOG_MESSAGE(CX_LOG_LEVEL_WARNING, __VA_ARGS__)
#define CX_LOG_INFO(...) \
  CX_LOG_MESSAGE(CX_LOG_LEVEL_INFO, __VA_ARGS__)
#define CX_LOG_DEBUG(...) \
  CX_LOG_MESSAGE(CX_LOG_LEVEL_DEBUG, __VA_ARGS__)

#endif /*ifndef __CX_LOG_H__*/

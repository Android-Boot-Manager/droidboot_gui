#ifndef DROIDBOOT_LOGGING_H
#define DROIDBOOT_LOGGING_H
typedef unsigned int droidboot_log_level;

void droidboot_log(droidboot_log_level log_level, const char *fmt, ...);
void droidboot_dump_hex(droidboot_log_level log_level, const void* data, size_t size);

#define DROIDBOOT_LOG_TRACE   0
#define DROIDBOOT_LOG_INFO    1
#define DROIDBOOT_LOG_WARNING 2
#define DROIDBOOT_LOG_ERROR   3
#define DROIDBOOT_LOG_FATAL   4
#endif


typedef droidboot_log_level uint;

void droidboot_log(droidboot_log_level log_level, const char *fmt, ...);

#define DROIDBOOT_LOG_TRACE   0
#define DROIDBOOT_LOG_INFO    1
#define DROIDBOOT_LOG_WARNING 2
#define DROIDBOOT_LOG_ERROR   3
#define DROIDBOOT_LOG_FATAL   4


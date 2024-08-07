#pragma once
// DROIDBOOT_ON_SCREEN_LOG can be 0-5 where 0 - trace, 1 - info, 2 - warning, 3 - error, 4 - fail, 5 - nothing
#define DROIDBOOT_ON_SCREEN_LOG 1

// DROIDBOOT_SYSTEM_LOG can be 1 or 0, enables logging with printf wich is usually handled at platform side. Recomended.
#define DROIDBOOT_SYSTEM_LOG 1

// DROIDBOOT_NO_LOGGING can be 1 or 0, overrides previous 2 defines and disables any logging from droidboot. Must be treated with care, and totally not recommended.
//#define DROIDBOOT_NO_LOGGING 0

// DROIDBOOT_LOG_LEVEL can be 0-5 where 0 - trace, 1 - info, 2 - warning, 3 - error, 4 - fail, 5 - nothing
#define DROIDBOOT_LOG_LEVEL 0

// DROIDBOOT_NO_SD_SUPPORT can be 1 or 0, enables SD less dualboot support
#define DROIDBOOT_NO_SD_SUPPORT 1

// DROIDBOOT_NO_SD_ENCRYPTED_SUPPORT can be 1 or 0, enables SD less dualboot support, on devices with metadata encrypted data, depends on DROIDBOOT_NO_SD_SUPPORT
#define DROIDBOOT_NO_SD_ENCRYPTED_SUPPORT 1 

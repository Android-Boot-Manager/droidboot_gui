#include <stdbool.h>

static inline bool _is_digit(char ch)
{
    return (ch >= '0') && (ch <= '9');
}

// internal ASCII string to unsigned int conversion
unsigned int droidboot_atoi(const char ** str)
{
    unsigned int i = 0U;
    while(_is_digit(**str)) {
        i = i * 10U + (unsigned int)(*((*str)++) - '0');
    }
    return i;
}

#pragma once
#include <cstdio>

#define LOG(logtype, format, ...) \
    do \
    { \
        fprintf(stderr, logtype " %s::%s, line %d: ", __FILE__, __FUNCTION__, __LINE__); \
        fprintf(stderr, format "\n", ##__VA_ARGS__);                              \
    } while (false);

#define LOG_ERROR(format, ...) LOG("ERROR", format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...) LOG("WARNING", format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) LOG("INFO", format, ##__VA_ARGS__)
    
#define RUNTIME_ERROR(format, ...)                             \
    do                                                         \
    {                                                          \
        LOG_ERROR(format, ##__VA_ARGS__)\
        exit(2);\
    } while (false);

#define ASSERT(cond) RUNTIME_ERROR("Assertion failed: " #cond)
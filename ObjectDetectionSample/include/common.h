#ifndef __COMMON_H__
#define  __COMMON_H__

typedef enum {
    EXECUTE_ERROR = -2,
    INVALID_INPUT = -1,
    NO_ERROR = 0
}state_t;

typedef enum  {
    CPU         = 0,
    GPU         = 1,
    DSP         = 2,
    APU         = 3
}runtime_t;
#endif

#include <stdint.h>

void init_controller(void);

void calculate_error(uint32_t expected, uint32_t actual);

uint32_t next_output(void);

uint32_t get_output(void);
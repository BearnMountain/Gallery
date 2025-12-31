#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "defines.h"

typedef struct Config {
	u32 window_width, window_height;
	
} Config;


extern Config config;

void config_init(void);

#endif

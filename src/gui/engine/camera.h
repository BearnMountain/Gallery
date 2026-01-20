#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "defines.h"
#include <cglm/cglm.h>

/*
 * Camera.h
 *  - stores one instance of a camera for the entire program
 *  - 
*/

void camera_init(void);

// changes camera
void camera_move(const f32 x, const f32 y);
void camera_set_zoom(const f32 zoom);

// get camera info
void camera_get_projection_matrix();

#endif

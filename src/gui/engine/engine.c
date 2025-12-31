#include "engine.h"


void engine_init(void) {

}

b32 widget_contains_bounds(Widget* widget, f16 mouse_x, f16 mouse_y) {
	if (!widget->visible || !widget->enabled) return 0;
    return mouse_x >= widget->bounds.x && mouse_x <= widget->bounds.x + widget->bounds.width &&
           mouse_y >= widget->bounds.y && mouse_y <= widget->bounds.y + widget->bounds.height;
}

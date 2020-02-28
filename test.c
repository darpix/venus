#include <unistd.h>

#include "src/window.h"
#include "src/venus_common.h"
#include "src/engine/graphics.h"
#include "src/toolkit/theme.h"

int main(int argc, char **argv) {
	venus_initialize();
	
	window my_window;
	create_window(&my_window);
	show(&my_window);
	set_title(&my_window, "Venus");
	color color = make_color(3, 135, 170, 222);
	set_background_color(&my_window, color);
	
	venus_begin_loop();
	graph_test(&my_window);
	swap_buffers(&my_window);
	
	sleep(2);
	
	vec_delete(color);
	venus_terminate();
	return 0;
}



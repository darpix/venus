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
	color col = create_color(3);
	col[0] = 135;
	col[1] = 170;
	col[2] = 222;
	flush();
	set_background_color(&my_window, col);
	vec_delete(col);
	
	venus_begin_loop();
	
	graph_test(&my_window);
	
	sleep(2);
	
	venus_terminate();
	return 0;
}



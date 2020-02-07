#include "src/window.h"
#include "src/venus_common.h"

int main(int argc, char **argv) {
	venus_initialize();
	
	window my_window;
	create_window(&my_window);
	show(&my_window);
	flush();
	
	
	venus_begin_loop();
	
	destroy_window(&my_window);
	venus_terminate();
	return 0;
}



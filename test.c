#include "src/window.h"
#include "src/venus_common.h"
#include "src/util/matrix.h"

int main(int argc, char **argv) {
	venus_initialize();
	
	window my_window;
	create_window(&my_window);
	show(&my_window);
	flush();
	
	//while(1);
	
	matrix mat0 = create_matrix(2, 4);
	
	mat0.data[0] = 1;
	mat0.data[1] = 2;
	mat0.data[2] = 3;
	mat0.data[3] = 4;
	mat0.data[4] = 5;
	mat0.data[5] = 6;
	mat0.data[6] = 7;
	mat0.data[7] = 8;
	
	matrix mat1;
	//mat1 = create_matrix(4, 2);
	
	print_matrix(&mat0);
	matrix_transpose(&mat1, &mat0);
	print_matrix(&mat1);
	
	zlog_category_t *log = zlog_get_category("venus");
	

	venus_terminate();
	return 0;
}



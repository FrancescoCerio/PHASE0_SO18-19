#include "umps/types.h"
#include "umps/arch.h"
#include "printer.h"

typedef unsigned int u32;

static volatile dtpreg_t *printer = (dtpreg_t *) DEV_REG_ADDR(IL_PRINTER,0);

int print_putchar(char c){
	u32 stat=printer->status; 
	if (stat !=1)
		return -1;

	printer->data0=c;
	printer->command=2;

	return 0;
}

int print_str(char *str){
	for (; *str; ++str){
		if (print_putchar(*str))
			return -1;
	}
	return 0;
}



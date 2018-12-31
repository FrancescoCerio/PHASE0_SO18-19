#include "umps/types.h"
#include "umps/arch.h"
#include "printer.h"

typedef unsigned int u32;
/* Puntatore alla struttura dati utilizzata dalla stampante */
static volatile dtpreg_t *printer = (dtpreg_t *) DEV_REG_ADDR(IL_PRINTER,0);

void printer_reset(){
	printer->command=0;
}

void printer_ack(){
	printer->command=1;
}

int print_putchar(char c){
	/* Eseguo un controllo sullo stato della stampante prima dell'operazione di stampa */
	switch(printer->status){
		case 0:
			term_puts("\nDEVICE NOT INSTALLED\n");
			return -1;
			break;
		case 2:
			term_puts("\nILLEGAL OPERATION CODE\n");
			return -1;
			break;
		case 3:
			term_puts("\nDEVICE BUSY\n");
			return -1;
			break;
		case 4:
			term_puts("\nPRINT ERROR\n");
			return -1;
			break;
	}
	/* Il campo command indica il tipo di operazione che la stampante deve eseguire. */
	/* Il numero 2 corrisponde all'operazioni di stampa del valore salvato in data0. */
	printer->data0=c;
	printer->command=2;
	while ((printer->status == 3)){
        	;
	}
	       
	printer_ack();
	return 0;
}

int print_str(char *str){
	/* Per ogni carattere di str eseguo l'operazione di print_putchar e controllo se viene eseguita con successo */
	for (; *str; ++str){
		if (print_putchar(*str))
			return -1;
	}
	return 0;
}



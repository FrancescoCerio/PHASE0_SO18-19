#include "umps/libumps.h"
#include "umps/arch.h"
#include "terminal.h"
#include "printer.h"

#define LINE_BUF_SIZE 64
#define TOD 0x1000001c			        /*  Numero di istruzioni svolte dalla CPU   */
#define TIME_SCALE 0x10000024		    /*  Numero di istruzioni svolta al secondo  */
#define address(a) *((volatile unsigned int *)a) 

typedef unsigned int u32;
static char buf[LINE_BUF_SIZE];

int main(int argc, char *argv[]){
    term_puts("TEST PER MACCHINA UMPS\n");
    
    /* Test I/O su terminale
    term_puts("TEST LETTURA/SCRITTURA DA BUFFER\n");
    term_puts("INPUT");
    readline(buf,LINE_BUF_SIZE);
    term_puts("\nHo letto:");
    term_puts(buf);
    term_puts("\nProva stampa valore int: 123->");
    term_putint(123);
	*/
	
    readline(buf,LINE_BUF_SIZE);
    print_str(buf);
	
	

    return 0;
}

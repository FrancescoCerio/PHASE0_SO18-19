#include "umps/libumps.h"
#include "umps/arch.h"
#include "terminal.h"

#define LINE_BUF_SIZE 64

static char buf[LINE_BUF_SIZE];

/* Funzione per leggere da terminale */
static void readline(char *buf, unsigned int count)
{
    int c;

    while (--count && (c = term_getchar()) != '\n')
        *buf++ = c;

    *buf = '\0';
}

#define IS_DIGIT(x) ('0' <= x && x <= '9')
static unsigned int broken_strtou(const char *str)
{
    unsigned int retval = 0;
    const char *p = str;

    for (p = str; *p && IS_DIGIT(*p); p++)
        retval = retval * 10 + *p - '0';

    return retval;
}




int main(int argc, char *argv[]){
	term_puts("TEST PER MACCHINA UMPS\n");
	
	
	term_puts("TEST LETTURA/SCRITTURA DA BUFFER\n");
        term_puts("INPUT");
	readline(buf,LINE_BUF_SIZE);
	term_puts("\nHo letto:");
	term_puts(buf);
    	term_puts("\nProva stampa valore int: 123->");
	term_putint(123);
	
	
	
	/* Stop uMPS*/
	halt();

    	return 0;
}

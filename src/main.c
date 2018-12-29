#include "umps/libumps.h"
#include "umps/arch.h"
#include "terminal.h"
#include "printer.h"
#include "disk.h"

#define TOD 0x1000001c			        /*  Numero di istruzioni svolte dalla CPU   */
#define TIME_SCALE 0x10000024		    /*  Numero di istruzioni svolta al secondo  */
#define address(a) *((volatile unsigned int *)a) 

typedef unsigned int u32;


int main(int argc, char *argv[]){
	term_puts("PHASE0_SO18-19\nTest funzionamento di disco e stampante virtuali di uMPS\n");
	term_puts("Scrittura dell'alfabeto su disco, lettura e copia in memoria e  stampa \n\n");
	char c[27]="abcdefghijklmnopqrstuvwxyz";
	
	disk_info();
	if(disk_seek(0x0000009)){
		term_puts("\nSEEK FALLITA\n");
		return -1;
	}else{
		term_puts("\n -Seek eseguita");
	}
	if(disk_write((u32 *)c,0x00000001,0x00000007)){
		term_puts("\nSCRITTURA DISCO FALLITA\n");
		return -1;
	}else{
		term_puts("\n -Scrittura disco eseguita");
	}
	
	int i=0;
	for(;i<27;i++){
		c[i]=0x0;
	}
	
	if(disk_read((u32 *)c,0x00000001,0x00000007)){
		term_puts("\nLETTURA DISCO FALLITA\n");
		return -1;
	}else{
		term_puts("\n -Lettura disco eseguita");
	}
	
	if(print_str(c)){
		term_puts("\nSTAMPA FALLITA\n");
		return -1;
	}else{
		term_puts("\n -Stampa eseguita");
	}
	
	
	return 0;
}

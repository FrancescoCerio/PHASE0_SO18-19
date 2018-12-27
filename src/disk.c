#include "umps/libumps.h"
#include "umps/types.h"
#include "umps/arch.h"
#include "terminal.h"
#include "disk.h"

#define address(a) *((volatile unsigned int *)a)

typedef unsigned int u32;

/* Puntatore alla struttura dati del disco utilizzato */

static volatile dtpreg_t *disk = (dtpreg_t *)(DEV_REG_ADDR(IL_DISK, 0));

/* Funzione per stampare a video le informazioni del disco */
void disk_info(){
	term_puts("\nMax Cylinder = ");
	term_putunsignedint(disk->data1>>16);
	term_puts("\nMax Head = ");
	term_putunsignedint(disk-data1<<16>>24);
	term_puts("\nMax Sector = ");
	term_putunsignedint(disk->data1<<24>>24);
}

/* Eseguo i controlli sullo stato del disco, se presenti viene mostrato sul terminale quale tipo di errore è rilevato */
u32 disk_status(){
	term_putchar('\n');
		
	switch(disk->status){
		case 0:
			term_puts("DEVICE NOT INSTALLED");
			return -1;
			break;
		case 2:
			term_puts("ILLEGAL OPERATION CODE");
			return -1;
			break;
		case 3:
			term_puts("DEVICE BUSY");
			return -1;
			break;
		case 4:
			term_puts("SEEK ERROR");
			return -1;
			break;
		case 5:
			term_puts("READ ERROR");
			return -1;
			break;
		case 6:
			term_puts("WRITE ERROR");
			return -1;
			break;
		case 7:
			term_puts("DMA TRANSFER ERROR");
			return -1;
			break;
	}

	return 0;
}

/* Funzioni per la scrittura e la lettura sul disco */
int disk_write(u32 *ptr_current_ram, u32 head, u32 sect){

	if(disk_status())
		return-1;
	
	/* Se lo stato del disco è settato su Busy, aspetto che torni operativo per evitare che si creino problemi */
	while(disk->status == 3){			;
		;
	}
	
	/* Copio l'indirizzo RAM in cui scrivere i valori, dentro il registro data0 */
	disk->data0 = ptr_current_ram;
	
	/* Imposto il registro command con il valore 4 per attivare la scrittura, con i relativi valori di headnum e sectnum del silindro corrente */
	u32 cmd=((0x0+sect)<<8+(0x0+head)<<8)+0x04;
	disk->command = cmd;
	
	return 0;

}


int disk_read(u32 *ptr_current_ram, u32 head, u32 sect){
	
	if(disk_status())
		return -1;
	
	/* Se lo stato del disco è settato su Busy, aspetto che torni operativo per evitare che si creino problemi */
	while(disk->status == 3){
		;
	}
	
	/* Copio l'indirizzo RAM in cui leggere i valori, dentro il registro data0 */
	disk->data0 = ptr_current_ram;
	
	/* Imposto il registro command con il valore 3 per attivare la lettura, con i relativi valori di headnum e sectnum del cilindro corrente */
	u32 cmd=((0x0+sect)<<8+(0x0+head)<<8)+0x03;
	disk->command = cmd;
	
	return 0;
}

void disk_reset(){
	disk->command = 0;
}

void disk_ack(){
	disk->command = 1;
}

/* Funzione per la ricerca del cilindro necessario alla lettura o alla scrittura all'interno del registro data1 */ 
u32 disk_seek(u32 cylnum){
	if (disk_status(disk)){
		return -1;
	}
	u32 cmd=((0x00000000+cylnum)<<8)+2;
	disk->command=cmd;
	return 0;
}

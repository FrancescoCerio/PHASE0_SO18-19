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
	term_putunsignedint(disk->data1<<16>>24);
	term_puts("\nMax Sector = ");
	term_putunsignedint(disk->data1<<24>>24);
}


/* Eseguo i controlli sullo stato del disco, se presenti viene mostrato sul terminale quale tipo di errore è rilevato */
u32 disk_status(){
	switch(disk->status){
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
			term_puts("\nSEEK ERROR\n");
			return -1;
			break;
		case 5:
			term_puts("\nREAD ERROR\n");
			return -1;
			break;
		case 6:
			term_puts("\nWRITE ERROR\n");
			return -1;
			break;
		case 7:
			term_puts("\nDMA TRANSFER ERROR\n");
			return -1;
			break;
	}

	return 0;
}

/* Funzioni per la scrittura e la lettura sul disco */
u32 disk_write(u32 *ptr_current_ram, u32 head, u32 sect){

	if(disk_status())
		return-1;
	/* Copio l'indirizzo RAM in cui scrivere i valori, dentro il registro data0 */
	disk->data0 = ptr_current_ram;
	
	/* Imposto il registro command con il valore 4 per attivare la scrittura, con i relativi valori di headnum e sectnum del silindro corrente */
	u32 cmd=(((head)<<8)+sect<<8)+4;
	disk->command = cmd;
	/* Se lo stato del disco è settato su Busy, aspetto che torni operativo per evitare che si creino problemi */
	while(disk->status == 3){			
		;
	}
	disk_ack();
	return 0;

}


u32 disk_read(u32 *ptr_current_ram, u32 head, u32 sect){
	
	if(disk_status())
		return -1;
	/* Copio l'indirizzo RAM in cui leggere i valori, dentro il registro data0 */
	disk->data0 = ptr_current_ram;
	
	/* Imposto il registro command con il valore 3 per attivare la lettura, con i relativi valori di headnum e sectnum del cilindro corrente */
	u32 cmd=(((head)<<8)+sect<<8)+3;
	disk->command = cmd;
	/* Se lo stato del disco è settato su Busy, aspetto che torni operativo per evitare che si creino problemi */
	while(disk->status == 3){
		;
	}
	disk_ack();
	return 0;
}

void disk_reset(){
	disk->command = 0;
}

void disk_ack(){
	disk->command = 1;
}

/* Funzione di seek che permette di cambiare cilindro del disco  */ 
u32 disk_seek(u32 cylnum){
	/* Eseguo un controllo sullo stato del disco prima dell'operazione di seek */
	if (disk_status(disk)){
		return -1;
	}
	/* Controllo che cylnum sia compreso tra 0 e il numero di cilindri del disco */
	if ((cylnum > (disk->data1>>16)) || cylnum <0){
		term_puts("\n Numero cilindro non valido\n");
		return -1;
	}
	/* I bit da 8 a 23 del campo command nelle operazioni di seek corrispondono al numero del cilindro */
	/* Il valore 2 corrisponde all'operazioni di seek */
	u32 cmd=((0x00000000+cylnum)<<8)+2;
	disk->command=cmd;
	disk_ack();
	return 0;
}

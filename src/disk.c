#include "umps/libumps.h"
#include "umps/types.h"
#include "umps/arch.h"
#include "terminal.h"
#include "disk.h"

/* define per lo stato del disco */ 

#define SD_READY 1
#define SD_BUSY 3

/* define per settare i comandi del disco */

#define CMD_RESET 0x00
#define CMD_ACK 0x01
#define CMD_SEEKCYL 0x02
#define CMD_READBLK 0x03
#define CMD_WRITEBLK 0x04

/* define per gli offset del COMMAND_FIELD */

#define CMD_RW 8
#define CMD_SECTNUM 8
#define CMD_HEADNUM 8

#define address(a) *((volatile unsigned int *)a)
#define SIZE_IN 64

typedef unsigned int u32;
static unsigned int *ptr_ram = 0x20005000;
static char in[SIZE_IN];

static volatile dtpreg_t *disk = (dtpreg_t *)(DEV_REG_ADDR(IL_DISK, 0));

/*disk->data0=0x20005000;*/

void disk_info(){
	term_puts("\nMax Cylinder = ");
	term_putunsignedint(disk->data1>>16);
	term_puts("\nMax Head = ");
	term_putunsignedint(disk->data1<<16>>24);
	term_puts("\nMax Sector = ");
	term_putunsignedint(disk->data1<<24>>24);
}
	
	
int disk_write(u32 *ptr_current_ram, u32 head, u32 sect){

	if(disk_status())
		return-1;
			
	/*disk->command = 0;*/

	disk->data0 = ptr_current_ram;
	u32 cmd=((0x00000000+sect)<<8+(0x00000000+head)<<16)+4;
	disk->command = cmd;
	
	disk_reset();
	
	return 0;

}


int disk_read(u32 *ptr_current_ram, u32 head, u32 sect){
	
	if(disk_status())
		return -1;
	
	/*disk->command = 0;*/
	
	/*u32 cmd = 0x00000003;*/
	
	disk->data0 = ptr_current_ram;
	u32 cmd=((0x00000000+sect)<<8+(0x00000000+head)<<16)+3;
	disk->command = cmd;
	term_putunsignedint(*ptr_ram);
	
	disk_reset();

	return 0;
}

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

void disk_chdata0(unsigned int chdata){
	disk->data0 = chdata;
}

void disk_reset(){
	disk->command = 0;
}

void disk_ack(){
	disk->command = 1;
}

u32 disk_seek(u32 cylnum){
	/* Eseguo un controllo sullo stato del disco prima dell'operazione di seek */
	if (disk_status(disk)){
		return -1;
	}
	/* I bit da 8 a 23 del campo command nelle operazioni di seek corrispondono al numero del cilindro */
	/* Il valore 2 corrisponde all'operazioni di seek */
	u32 cmd=((0x00000000+cylnum)<<8)+2;
	disk->command=cmd;
	return 0;
}

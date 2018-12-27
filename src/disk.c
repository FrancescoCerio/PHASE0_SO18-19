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
	if (disk_status(disk)){
		return -1;
	}
	u32 cmd=((0x00000000+cylnum)<<8)+2;
	disk->command=cmd;
	return 0;
}

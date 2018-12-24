#include "umps/libumps.h"
#include "umps/arch.h"
#include "umps/types.h"
#include "terminal.h"

#define address(a) *((volatile unsigned int *)a)

typedef unsigned int u32;
static volatile dtpreg_t *disk = (dtpreg_t *)(DEV_REG_ADDR(IL_DISK,0));
disk->data0=0x20005000;

u32 disk_status(){
	term_putchar('\n');

	switch(disk->status){
		case 0:
			term_puts("Device not Installed \n");
			return -1;
			break;
		case 2:
			term_puts("Illegal operation code \n");
			return -1;
			break;
		case 3:
			term_puts("Device busy \n");
			return -1;
			break;
		case 4:
			term_puts("Seek error \n");
			return -1;
			break;
		case 5:
			term_puts("Read error \n");
			return -1;
			break;
		case 6:
			term_puts("Write errore \n");
			return -1;
			break;
		case 7:
			term_puts("DMA transfer error");
			return -1;
			break;
	}
}

void disk_reset(dtpreg_t *disk){
	disk->command=0;
}

u32 disk_seek(u32 cylnum){
	if (disk_status(disk){
		return -1;
	}
	u32 cmd=((0x00000000+cylnum)<<8)+2;
	disk->command=cmd;
	return 0;
}

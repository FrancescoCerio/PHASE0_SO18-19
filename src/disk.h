/*Dichiaro le funzioni necessarie per la gestione dei dischi */
typedef unsigned int u32;

u32 disk_write(u32 *ptr_current_ram, u32 head, u32 sect);
u32 disk_read(u32 *ptr_current_ram, u32 head, u32 sect);
u32 disk_status();
void disk_info();
void disk_reset();
void disk_ack();
u32 disk_seek(u32 cylnum);

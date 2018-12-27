typedef unsigned int u32;

int disk_write(u32 *ptr_current_ram);
int disk_read(u32 *ptr_current_ram);
u32 disk_status();
void disk_chdata0(unsigned int chdata);
void disk_reset();
void disk_ack();
u32 disk_seek(u32 cylnum);

/* Gestione I/O terminale */
/* Scrive un char su terminale */
int term_putchar(char c);
/* Scrive un int su terminale */
int term_putint(int x);
/* Scrive un unsigned int su terminale */
int term_putunsigned(unsigned int x);
/* Scrive una stringa su terminale */
int term_puts(char *str);
/* Legge un char da terminale */
int term_getchar(void);


/* Gestione macchina uMPS */
void halt(void);

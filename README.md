# PHASE0_SO18-19
**Fase** 0 del progetto per il corso **Sistemi Operativi 18/19 UniBO**, gruppo *lso19az22*.  
Obbiettivo di questa fase è realizzare un programma che mostri il funzionamento di un componente della macchina emulata μMPS2.


## Progetto
Per questa fase abbiamo scelto di creare un programma che utilizzi il disco e la stampante di μMPS2.
Ci siamo occupati di sviluppare delle piccole librerie per entrambi i dispositivi, seguendo le specifiche che si possono trovare nel capitolo 5 del [manuale di μMPS2](http://www.cs.unibo.it/~renzo/so/princOfOperations.pdf).

### Dispositivi in μMPS2
Ogni dispositivdispositivoo ha un registro associato di 4 parole di memoria che permettono di interagire con l'hardware simulato attraverso la lettura e la scrittura di quest'ultime.  

Tutti i device condividono la stessa struttura dati.
````
typedef struct dtpreg {
    unsigned int status;   /* Utilizzato per comunicare lo stato del dispositivo    */
    unsigned int command;  /* Utilizzato per impartire comandi al device controller */
    unsigned int data0;    /* Utilizzo varia per ogni dispositivo */
    unsigned int data1;    /* Utilizzo varia per ogni dispositivo */
} dtpreg_t;
````

Per usare un dispositivo:
```
static volatile dtpreg_t *disk = (dtpreg_t *)(DEV_REG_ADDR(IL_DISK, 0)); /* Esempio di utilizzo di un disco */
```
Dove DEV_REG_ADDR(..) è una macro (definita in arch.h, libreria fornita con μMPS2) che restituisce l'indirizzo del registro del dispositivo.
Per aggiungere un dispositivo è necessario prima abilitarlo nella configurazione della propria macchina μMPS2. Sono supportate massimo 8 istanze per ogni dispositivo.
### Disco
Per creare un disco si può utilizzare l'utility *umps2-mkdev*, fornita con μMPS2.
...
### Stampante
La stampante di μMPS2 viene simulata scrivendo su un file di testo. Ogni carattere passato viene poi scritto sul file associato alla stampante nella configurazione della macchina virtuale.

Il campo status, che può solo essere letto, indica lo stato della macchina.

| Code| Status |  
|:-:|:-:|
|  0 |  Device not installed | 
|  1 |  Device ready |  
|  2 |  Illegal operation code|  
|  3 |  Device busy|  
|  4 |  Printe error |  

Il campo command indica l'operazione che il controller della stampante deve eseguire.

| Code| Command |  
|:-:|:-:|
|  0 |  Reset | 
|  1 |  Ack |  
|  2 |  Print char|  
 
Il campo data0 serve a settare il carattere che verrà poi trasmesso alla stampante al momento della stampa

La funzione *print_putchar(char c)* salva in data0 il char c e dà alla stampante il comando di stampa. Prima di terminare si assicura che la stampante non sia più occupata dal processo di stampa e manda un ack al termine.

Per poter stampare più caratteri insieme si può utilizzare la funzione *print_str(char \*str)* che chiama *print_putchar* per ogni carattere della stringa.

## Compilare per μMPS2
Per compilare i file in src è necessario avere prima installato [μMPS2](https://github.com/tjonjic/umps).
```
$ git clone https://github.com/filippobartolucci/PHASE0_SO18-19/
$ cd PHASE0_SO18-19/src/
$ make -f Makefile-templ
``` 
Verrano creati due file, kernel.core.umps e kernel.stab.umps, che sono i file da usare per la configurazione della macchina MIPS in μMPS2

## Sviluppatori
...

## Licenza 
GNU

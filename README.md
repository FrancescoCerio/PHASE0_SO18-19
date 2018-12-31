# PHASE0_SO18-19
**Fase 0** del progetto per il corso **Sistemi Operativi 18/19 UniBO**, gruppo *lso19az22*.  
Obbiettivo di questa fase è realizzare un programma che mostri il funzionamento di un componente della macchina emulata μMPS2.


## Progetto
Per questa fase abbiamo scelto di creare un programma che utilizzi il disco e la stampante di μMPS2.
Ci siamo occupati di sviluppare delle piccole librerie per entrambi i dispositivi, seguendo le specifiche che si possono trovare nel capitolo 5 del [manuale di μMPS2](http://www.cs.unibo.it/~renzo/so/princOfOperations.pdf).

### Dispositivi in μMPS2
Ogni dispositivo ha un registro associato di 4 parole di memoria che permettono di interagire con l'hardware simulato attraverso la lettura e la scrittura di quest'ultime.  

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
Dove DEV_REG_ADDR(..) è una macro (definita in arch.h, libreria fornita con μMPS2) che restituisce l'indirizzo del registro del dispositivo in base al tipo e al numero del dispositivo richiesto.
Per aggiungere un dispositivo è necessario prima abilitarlo nella configurazione della propria macchina μMPS2. Sono supportate massimo 8 istanze per ogni dispositivo.
### Disco
Per creare un disco si può utilizzare l'utility *umps2-mkdev*, fornita con μMPS2.

Le unità disco di μMPS2 sono dei device che supportano operazioni di lettura/scrittura DMA (*Direct Memory Access*) e i blocchi di memoria su cui operano hanno una dimensione di 4KB.

Il registro **DATA1** è un campo di sola lettura nel quale sono contenute le caratteristiche fisiche del device ed è diviso in 3 parti:
- MAXCYL;
- MAXHEAD;
- MAXSECT;

Nei quali sono salvati i valori massimi per ogni campo.

Il campo **STATUS**, che permette la sola lettura, indica lo stato della macchina, in base ai valori che contiene, riportati nella tabella seguente.

| Code| Status |  Possible Reason for Code |
|:-:|:-:|:-:|
|  0 |  Device not installed | Device not installed |
|  1 |  Device ready |  Device waiting for a command |
|  2 |  Illegal operation code|  Device presented unknown command |
|  3 |  Device busy|  Device executing a command |
|  4 |  Seek error | Illegal parameter/hardware failure |
|  5 |  Read error | Illegal parameter/hardware failure |
|  6 |  Write error | Illegal parameter/hardware failure |
|  7 |  DMA Transfer error | Illegal physical address/hardware failure |

La funzione *disk_status()* controlla lo stato del disco e se non è pronto per ricevere un comando (*Device Ready*), mostra a video sul terminale l'errore corrente e imposta -1 come valore di ritorno della funzione, per segnalare che è presente un errore.

Il registro **DATA0** è un campo di lettura e scrittura nel quale viene specificato l'indirizzo in memoria dal quale partire per effettuare le operazioni di lettura e scrittura DMA.

Il registro **COMMAND** è anch'esso un campo di lettura e scrittura e viene utilizzato per passare alla macchina i comandi da eseguire su disco, seguendo i valori riportati nella tabella.

| Code| Command |  Operation |
|:-:|:-:|:-:|
|  0 |  RESET | Reset the device and move the boom to cylinder 0 |
|  1 |  ACK |  Acknowledge a pending interrupt |
|  2 |  SEEKCYL | Seek to specified **CYLNUM**  |
|  3 |  READBLK | Read the block located at (**HEADNUM**, **SECTNUM**) in the current cylinder and copy it into RAM starting at the address in **DATA0** |
|  4 |  WRITEBLK | Copy the 4KB of RAM starting at the address in **DATA0** into the block located at (**HEADNUM**, **SECTNUM**) in the current cylinder) |

Le operazioni di lettura e scrittura sono regolate dalle funzioni *disk_read(u32 *ptr_current_ram, u32 head, u32 sect)* e *disk_write(u32 *ptr_current_ram, u32 head, u32 sect)* nelle quali, dopo aver effettuato i controlli necessari sullo stato della macchina e che i valori di **HEADNUM** e **SECTNUM** rientrino nel range prefissato, vengono effetturate le operazioni necessarie.

I parametri locali passati alla funzione sono di tipo *u32* (unsigned int, definito all'inizio della libreria disk.h) e indicano rispettivamente: 
- L'indirizzo di memoria nel quale effettuare l'operazione (*ptr_current_ram*);
- Il numero della testa del blocco (*head*);
- Il numero del settore del blocco (*sect*);

Inizialmente il disco è vuoto, quindi bisogna effetturare un'operazione di scrittura chiamando *disk_write(u32 *ptr_current_ram, u32 head, u32 sect)* che, dopo aver effettuato i controlli sullo stato del device e che le dimensioni di *head* e *sect* non superino i valore massimi, 
- copia l'indirizzo di memoria dentro il campo data0
- imposta il comando per la scrittura (passando i valori di head e sect e settando il valore 4)
- invia un ack come risposta

Per le operazioni di lettura i procedimenti sono uguali alle operazioni di scrittura, solamente che nel passare i valori al campo command viene impostato il valore 3, il quale attiva la lettura (vedi tabella sopra riportata).

### Stampante
La stampante di μMPS2 viene simulata scrivendo su un file di testo. Ogni carattere passato viene poi scritto sul file associato alla stampante nella configurazione della macchina virtuale.

Il campo **STATUS**, che può solo essere letto, indica lo stato della macchina.

| Code| Status |  
|:-:|:-:|
|  0 |  Device not installed | 
|  1 |  Device ready |  
|  2 |  Illegal operation code|  
|  3 |  Device busy|  
|  4 |  Print error |  

Il campo **COMMAND** indica l'operazione che il controller della stampante deve eseguire.

| Code| Command |  
|:-:|:-:|
|  0 |  Reset | 
|  1 |  Ack |  
|  2 |  Print char|  
 
Il campo **DATA0** serve a settare il carattere che verrà poi trasmesso alla stampante al momento della stampa.
**DATA1** non viene utilizzato dalla stampante.

La funzione *print_putchar(char c)* salva in **DATA0** il char c e dà alla stampante il comando di stampa. Prima di terminare si assicura che la stampante non sia più occupata dal processo di stampa e manda un ack al termine.

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

* Filippo Bartolucci - filippo.bartolucci2@studio.unibo.it
* Francesco Cerio - francesco.cerio@studio.unibo.it
* Tonino Petrulli - tonino.petrulli@studio.unibo.it


## Licenza 
GNU

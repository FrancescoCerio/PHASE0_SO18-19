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
...
Per usare un dispositivo:
```
static volatile dtpreg_t *disk = (dtpreg_t *)(DEV_REG_ADDR(IL_DISK, 0));
```
...
Per aggiungere un dispositivo bisogna prima aggiungerlo e abilitarlo nella configurazione della propria macchina μMPS2
μMPS2 supporta 8 istanze per ogni dispositivo.
### Disco
Per creare un disco si può utilizzare l'utility *umps2-mkdev*, fornita con μMPS2.
...
### Stampante
Il campo data0 nelle stampante serve a settare il carattere che verrà poi trasmesso alla stampante al momento della stampa
...

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

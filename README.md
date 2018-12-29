# PHASE0_SO18-19
**Fase** 0 del progetto per il corso **Sistemi Operativi 18/19 UniBO**, gruppo *lso19az22*.  
Obbiettivo di questa fase è realizzare un programma che mostri il funzionamento di un componente della macchina emulata μMPS2.

## Progetto
Per questa fase del progetto abbiamo scelto di creare un programma che mostra il funzionamento di disco e stampante di μMPS2.
Ci siamo occupati di sviluppare delle piccole librerie per entrambi i dispositivi,seguendo le specifiche che si possono trovare nel capitolo 5 del [manuale di μMPS2](http://www.cs.unibo.it/~renzo/so/princOfOperations.pdf).

### Dispositivi in μMPS2
### Disco
### Stampante

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

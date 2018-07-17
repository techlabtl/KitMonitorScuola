## Prima configurazione del kit

### Collegamenti
Per configurare il kit bisogna:
* collegare l'arduino yun con un cavo USB - microUSB al proprio PC
* inserire la scheda microSD nell'arduino yun
* inserire la *base shield* sull'arduino yun
* collegare ogni sensore al relativo ingresso analogico

Come faccio a riconoscere i giusti ingressi analogici? 
Basta guardare tra le prime righe di codice, nella cartella test_sensori :)

### Arduino
Scaricatelo da qui:
https://www.arduino.cc/en/Main/Software

e poi avviatelo una volta in modo che si configuri correttamente.

### Librerie
Per utilizzare i sensori serviranno due librerie che ci semplifichino la vita:
le potete trovare qua:

* DHTLib: https://drive.google.com/open?id=0B4KNW3XBN0r0c1QxMjl2dkpQc2c
* Emonlib: https://github.com/openenergymonitor/EmonLib

Per installarle correttamente (basta poco perché Arduino non le riconosca..) dovete:
* scaricare e scompattarle
* copiarle nella cartella **Arduino/Libraries/** che si trova in diversi posti a seconda del vostro sistema operativa, solitamente dentro la cartella **Documenti**

Prima di poterle utilizzare dovrete chiudere e riaprire il software Arduino.

### Primo upload nella scheda!
Adesso siete pronti per scaricare il codice (o copiarlo) ed aprirlo con Arduino. Dopo di che dovrete compiere i seguenti passi
per poterlo inserire nel microchip interno alla scheda:
* Selezionare dal menù **Strumenti** la scheda: Arduino YUN
* Selezionare dal menù **Porta** la scheda Arduino (collegata al PC :)
* Schiacciare il pulsante tondo con la freccia verso destra!
 

**Per capire cosa fa il codice leggete il README.md nella cartella test_sensori**




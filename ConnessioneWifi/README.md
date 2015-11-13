## Fin qui tutto bene...

### Inizia il divertimento
Bene!
Hai tra le mani un kit con dei sensori che leggono e stampano in seriale. Come facciamo a mandare questi dati ad un sito internet dedicato proprio alla loro visualizzazione?
2 grandi passi:
 * collegare l'arduino YUN in wifi
 * mandare i dati sul plot.ly

### Collegare l'arduino YUN in wifi
Esistono molte leggende su tutti i vari comportamenti che questa scheda ha quando si connette in wifi.. Diciamo che date le molte variabili presente in una rete wifi casalinga o pubblica è difficile predire tutte le possibilità, quindi vi consiglio di seguire bene tutti i passaggi della seguente guida:

http://www.maffucci.it/2013/10/02/benvenuto-arduino-yun/

La guida è stata scritta per i sistemi operativi OSX, per apple, per Windows 7 c'è questo:

http://www.logicaprogrammabile.it/installazione-arduino-sistema-windows-7/

e ce ne sono molte altre, facili da trovare.

### Mandare i dati su plot.ly
Benissimo.
Adesso avete un piccolo PC collegato ad internet! Se conoscete il protocollo SSH [potete usarlo per connettervi](http://codybonney.com/connecting-to-the-arduino-yun-via-ssh/), per esempio, e vedere com'è fatto il sistema operativo dentro la scheda.

Oppure, potete caricare lo sketch definitivo che manda i dati ad un sito molto interessante che offre un servizio gratuito di visualizzazione dati:

https://plot.ly/

Per capire come funziona dovete completare tutti gli step di questo bellissimo tutorial:

https://github.com/plotly/arduino-api/tree/master/plotly_yun

### 


 

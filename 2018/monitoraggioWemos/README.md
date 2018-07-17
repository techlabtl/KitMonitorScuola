##COMPONENTI
* Wemos
* MicroSD Shield (Optional)
* sd card (Optional)
* BME280
* TSL2561
* ANALOG LIGHT SENS BREAKOUT BOARD

## PINMAP
#### SENSORI I2C -> WEMOS
* sda -> D2
* scl -> D1
* vin -> 3.3V
* gnd -> GND
#### Analog light -> WEMOS
* vcc -> 3.3V
* gnd -> GND
* out -> A0

## Descrizione
la wemos espone una rete wifi chiamata __"Kit Monitoraggio"__

all'indirizzo __192.168.4.1 è presente un webserver che comunica i dati istantanei e quelli medi rilevati da quando la wemos è stata accesa l'ultima volta

## Backup
aggiungendo una shield wemos SD e una scheda SD, vengono automaticamente registrati i dati raccolti.
per risparmiare risorse viene salvata solo la media dei dati degli ultimi 10 minuti.

## Customization
* modificando la variabile __LIGHT_THRESHOLD__ è possibile indicare la soglia sopra la quale la luce viene considerata accesa
* modificando la variabile __SAVE_TIME__ è possibile cambiare la frequenza con cui vengono salvati i dati su sd
* modificando la variabile __SENSOR_TIME__ è possibile cambiare la frequenza con cui vengono letti i sensori
* modificando la variabile __*ssid__ è possibile cambiare il nome della rete wifi esposta

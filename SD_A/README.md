#Guida per la configurazione della yun per renderla un bel client per plotly

##Aggiorna il firmware della yun
Trovi qua la guida:

https://www.arduino.cc/en/Tutorial/YunSysupgrade

##Espandi lo spazio del sistema operativo sulla SD

https://www.arduino.cc/en/Tutorial/ExpandingYunDiskSpace

Se no, puoi collegarti in ssh e poi eseguire i seguenti comandi (per avere un disco da 1Gb):


      opkg update
      opkg install e2fsprogs mkdosfs fdisk rsync

      rm -rf /mnt/sda?
      umount /dev/sda?

      dd if=/dev/zero of=/dev/sda bs=4096 count=10

      (echo n; echo p; echo 1; echo; echo 1048576; echo w) | fdisk /dev/sda

      rm -rf /mnt/sda?
      umount /dev/sda?

      (echo n; echo p; echo 2; echo; echo; echo w) | fdisk /dev/sda

      rm -rf /mnt/sda?
      umount /dev/sda?

      (echo t; echo 1; echo c; echo w) | fdisk /dev/sda

      mkfs.vfat /dev/sda1
      mkfs.ext4 /dev/sda2

      mkdir -p /mnt/sda1
      mount /dev/sda1 /mnt/sda1
      mkdir -p /mnt/sda1/arduino/www

      rm -rf /mnt/sda?
      umount /dev/sda?

      mkdir -p /mnt/sda2
      mount /dev/sda2 /mnt/sda2
      rsync -a --exclude=/mnt/ --exclude=/www/sd /overlay/ /mnt/sda2/

      rm -rf /mnt/sda?
      umount /dev/sda?

      uci add fstab mount
      uci set fstab.@mount[0].target=/overlay
      uci set fstab.@mount[0].device=/dev/sda2
      uci set fstab.@mount[0].fstype=ext4
      uci set fstab.@mount[0].enabled=1
      uci set fstab.@mount[0].enabled_fsck=0
      uci set fstab.@mount[0].options=rw,sync,noatime,nodiratime
      uci commit

##Clona la cartella git sulla yun

opkg update
opkg install python-openssl
opkg install distribute
easy_install pip

pip install plotly
python -c "import plotly; plotly.tools.set_credentials_file(username='bonello_chieri', api_key='qto5asnqjx', stream_ids=['ljduvo6kaf', 'jerzvovazd', 't35impwed5', 'rjqldo03di', '8v5xitfha4', '0ewlqffol7', '1smk5j4q77', 'omtrfaaoe7', '11xeu4xw13', 'fdbw3gy0ym', 'f20mk3c28t', 'bkm1ma4p44', 'p1vyxgufp2', '3q1m5tltp2', 'phl8ijw3zd', 'yfaxa4zagd', 'aq3bkssbw7', '9s2h27psht'])"
scp -r ~/Desktop/YUN_code_fanelli/.../A root@...

scaricare sketch arduino

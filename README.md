
# MODBUS

Modbus je serijski komunikacioni protokol predstavljen 1979. godine od strane kompanije Modicon.
Modbus je aplikacioni protokol koji dodjejuje načine upravljanja i prosljedjivanja podataka izmedju različitih slojeva bez uticaja protokola koji koristi sljedeći neporedni sloj.
Ovaj protokol je master/slave protokol sa half-duplex prenosom iz grupe Filedbus protokola.
Koriste se tri tipa Modbus prenosa:
1. Modbus RTU- poruke se sastoje od binarnih bajtova
2. Modbus TCP
3. ASCII

## MODBUS-TCP

Modbus TCP/IP (Transmission Control protocol/Internet protocol) je jednostavan Modbus protokol koji radi na Eternetu preko TCP interfejsa.
Modbus TCP/IP je takodje poznat kao Modbus TCP.
Kada se Modbus informacije šalju korišćenjem ovih protokola, podaci se prosleđuju u TCP gdje se dodatne informacije prilažu i daju IP-u.IP zatim stavlja podatke u paket i prenosi ih.
Ovaj protokol je zapravo verzija Modbus RTU transfera koja je prilagodjena Ethernet okruženju.
Modbus TCP koristi RTU binarni prenos sa TCP/IP detekcijom greške u poruci ili transferu.

##Libmodbus 

Za slanje i primanje podataka pomoću uredjaja koji koriste Modbus prokol koriste se biblioteka [libmodbus](https://libmodbus.org/).
Ova biblioteka sadrži različite pozadine za komunikaciju preko raličitih mreža.
http://www.modbus.org stranica pruža dokumentaciju o Modbus specifikacijama i vodičima za implemetaciju.

''' ctx = modbus_new_tcp("192.168.100.102", 502);  
if (ctx == NULL) {
   fprintf(stderr, "Unable to allocate libmodbus context\n");
   return -1;
 } '''




## Instalacija Operativnog sistema i osnovna konfiguracija Raspberry Pi platforme
Prvi korak je formatiranje SD kartice pomoću aplikacije SD Card Formatter. Aplikaciju možemo preuzeti sa linka https://www.sdcard.org/downloads/formatter/sd-memory-card-formatter-for-windows-download/.  SD kartici pristupamo preko čitača SD kartica koji je priključen na računar. 
Sljedeći korak je instalacija operativnog sistema. Potrebno je instalirati softver Raspberry Pi Imager koji možemo preuzeti sa linka https://www.raspberrypi.com/software/. Bitno je odabrati varijantu Raspberry Pi OS (other) -> Raspberry Pi OS Lite (32-bit) operativnog sistema. Nakon što smo selektovali našu SD karticu u polju Storage prelazimo na podešavanje pristupa Raspberry Pi-a. Podešavamo hostname, username, password i potrebno je omogućiti SSH pristup. 
Treći korak jeste da podesimo statičku IP adresu na Raspberry Pi platformi, jer je pristup pomoću IP adrese mnogo pouzdanije rješenje. Prije svega treba da podesimo staticku IP adresu na računaru. Osim toga problem pristupa stvara Windows Firewall pa je potrebno sve isključiti. Iz Command Prompt-a pomoću komande ssh pi@hostname.local pristupamo Raspberri Pi. Zatim komandom sudo nano /etc/dhcpcd.conf mijenjamo postojeću adresu.

## Ciljevi i ishodi

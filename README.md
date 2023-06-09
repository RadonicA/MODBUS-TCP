
# MODBUS

Modbus je serijski komunikacioni protokol predstavljen 1979. godine od strane kompanije Modicon.
Modbus je aplikacioni protokol koji dodjejuje načine upravljanja i prosljeđivanja podataka između različitih slojeva bez uticaja protokola koji koristi sljedeći neposredni sloj.
Ovaj protokol je master/slave protokol sa half-duplex prenosom iz grupe Filedbus protokola.
Osnovne prednosti Modbus protokola su to da je razvijen za industrijske primjene, ima otvorenu specifikaciju bez troškova licence i jednostavan je za implementaciju i održavanje.
Koriste se tri tipa Modbus prenosa:
- Modbus RTU - koristi RS-485, binarna verzija razmjene podataka
- Modbus TCP
- ASCII

## MODBUS-TCP

Modbus TCP/IP (Transmission Control protocol/Internet protocol) je jednostavan Modbus protokol koji radi na Eternetu preko TCP interfejsa.
Modbus TCP/IP je takođe poznat kao Modbus TCP.
Kada se Modbus informacije šalju korišćenjem ovih protokola, podaci se prosleđuju u TCP gdje se dodatne informacije prilažu i daju IP-u. IP zatim stavlja podatke u paket i prenosi ih.
Ovaj protokol je zapravo verzija Modbus RTU transfera koja je prilagođena Ethernet okruženju.
Modbus TCP koristi RTU binarni prenos sa TCP/IP detekcijom greške u poruci ili transferu.

## Ciljevi zadatka

 - Uspostavljanje komunikacije između Mastera i Slejva pomoću Modbus TCP protokola
 - Savladavanje API biblioteke libmodbus 
 - Upoznavanje sa radom Wireshark softvera



## Eskperimentalna realizacija MODBUS TCP protokola 

Jedan Raspberry Pi je u ulozi klijenta (Modbus Master), a drugi Raspberry Pi ima ulogu servera (Modbus Slave). Na zahtjev Mastera, Slave očitava vrijednost temperature procesora Rpi-a, mapira tu vrijednost u prostor modbus adresa i šalje ga u odgovoru Masteru.   


## Libmodbus 

Za slanje i primanje podataka pomoću uređaja koji koriste Modbus protokol koriste se biblioteka [libmodbus](https://libmodbus.org/).
Ova biblioteka sadrži različite pozadine za komunikaciju preko raličitih mreža.
http://www.modbus.org stranica pruža dokumentaciju o Modbus specifikacijama i vodičima za implemetaciju.
U nastavku su navedene neke od funkcija koje smo koristili za uspostavljanje Modbus TCP komunikacije, a koje se nalaze u ***libmodbus*** biblioteci.

Funkcija ***modbus_new_tcp("192.168.100.102", 502)*** kreira novi libmodbus konktekst za TCP/IPv4.
Argument IP specificira IP adresu servera sa kojim klijent želi da uspostavi vezu, a argument PORT je TCP port koji treba koristiti.
```
 ctx = modbus_new_tcp("192.168.100.102", 502);  
if (ctx == NULL) {
   fprintf(stderr, "Unable to allocate libmodbus context\n");
   return -1;
 } 
 ```
U Master skripti funkcija ***modbus_read_registers(ctx, 0, 10, tab_reg)*** će pročitati sadržaj 10 registara za čuvanje na adresu slave uredjaja.
Rezultat čitanja se čuva u nizu tab_reg.
```
rc = modbus_read_registers(ctx, 0, 10, tab_reg); 
```
Narednom funkcijom će se uspostaviti veza sa Modbus serverom.
```
modbus_connect(ctx); 
```
Funkcija koja će dodijeliti četiri niza za skladištenje bitova, ulaznih bitova, 20 registara i ulaznih registara, definisana je u nastavku.
```
mb_mapping = modbus_mapping_new(0, 0, 20, 0); 
if (mb_mapping == NULL) {
    fprintf(
       stderr, "Failed to allocate the mapping: %s\n",
        modbus_strerror(errno)
    );
    modbus_free(ctx);
    return -1;
  }
```
Funkcija ***modbus_tcp_listen(ctx, 1)*** kreira socket i sluša zahtjev od određene IP adrese.
```
socket = modbus_tcp_listen(ctx, 1); 
 if (socket == -1) {
    fprintf(stderr, "%s\n", modbus_strerror(errno));
    modbus_free(ctx);
    return -1;
  }
```
Naredna funkcija će izdvojiti prvu vezu u redu veza na čekanju, kreirati novi socket i sačuvati ga u libmodbus kontekst dat u argumentu funkcije.
```
modbus_tcp_accept(ctx, &socket);
```
U nastavku je navedena funkcija kojom se setuje podatak u tip float i smiješta se u registar.
```
modbus_set_float_dcba(temp=get_cpu_temp()+0.5, mb_mapping->tab_registers+i);
```
Naredna funkcija će primiti zahtjev za indikaciju.
Ovu funkciju koristi Modbus slave za prijem i analizu zahtjeva za indikaciju koje šalje master.
```
rc = modbus_receive(ctx, query); 
```
Funkcija ***modbus_reply(ctx,query,rc,mb_mapping)*** šalje odgovor na primljeni zahtjev.
```
modbus_reply(ctx, query, rc, mb_mapping);
```

Pomocu funkcije ***get_cpu_temp()*** očitavamo temperaturu CPU Raspberry Pi-a. 
```
float get_cpu_temp() {
   FILE *fp;
   float temp;
    fp = popen("/usr/bin/vcgencmd measure_temp | awk -F '=' '{print $2}'", "r");
   if (fp == NULL) {
      printf("Failed to get temperature\n" );
      exit(1);
 }
   fscanf(fp, "%f", &temp);
   pclose(fp);
   return temp;
}
```

## Instalacija Operativnog sistema i osnovna konfiguracija Raspberry Pi platforme
Prvi korak je formatiranje SD kartice pomoću aplikacije SD Card Formatter. Aplikaciju možemo preuzeti sa [linka](https://www.sdcard.org/downloads/formatter/sd-memory-card-formatter-for-windows-download/). SD kartici pristupamo preko čitača SD kartica koji je priključen na računar. 
Sljedeći korak je instalacija operativnog sistema. Potrebno je instalirati softver Raspberry Pi Imager koji možemo preuzeti sa [linka](https://www.raspberrypi.com/software/). Bitno je odabrati varijantu Raspberry Pi OS (other) -> Raspberry Pi OS Lite (32-bit) operativnog sistema. Nakon što smo selektovali našu SD karticu u polju Storage prelazimo na podešavanje pristupa Raspberry Pi-a. Podešavamo hostname, username, password i potrebno je omogućiti SSH pristup. 
Treći korak jeste da podesimo statičku IP adresu na Raspberry Pi platformi, jer je pristup pomoću IP adrese mnogo pouzdanije rješenje. Prije svega treba da podesimo staticku IP adresu na računaru. Osim toga problem pristupa stvara Windows Firewall pa je potrebno sve isključiti. Iz Command Prompt-a pomoću komande ssh pi@hostname.local pristupamo Raspberri Pi. Zatim komandom : sudo nano /etc/dhcpcd.conf mijenjamo postojeću adresu.

## Wireshark

Wireshark je softver za analizu mrežnog saobraćaja. Riječ je o alatu koji hvata različite mrežne pakete i detaljno ih razlaže. Kada se ovi paketi razbiju, možemo ih koristiti za analizu u realnom vremenu ili van mreže.
Za instalaciju Wireshark softvera potrebno je ispratiti korake na [link](https://www.geeksforgeeks.org/how-to-install-and-use-wireshark-on-ubuntu-linux/).
Na slici ispod je prikazan segment Modbus TCP komunikacije u Wireshark-u za jednu konekciju.
![image](https://user-images.githubusercontent.com/127748379/228214315-6a60780d-274e-45c9-83d3-5aa5ef63a6a2.png)

Modbus TCP je siguran protokol koji omogucava **klijent-server** komunikaciju između uređaja povezanih na fizičku mrežu (Ethernet). Prije prenosa podataka zahtijeva potvrdu o uspostavljanju konekcije, tako da u Wireshark-u vidimo četiri poruke:

 **Modbus Request**- klijent šalje zahtjev serveru za uspostavljanje konekcije, vrši se sinhronizacija obije strane\
 **Modbus Indication**- server prima zahtjev za uspostavljanje konekcije \
 **Modbus Response**- server šalje potvrdu(ACK) o primljenoj poruci \
 **Modbus Confirmation**- klijent prima potvrdu sa strane servera, te se uspostavlja konekcija i počinje prenos podataka
 
  Wireshark je softver koji nam omogućava da prikažemo i **throughput** (količinu korisnih podataka koji se prenose u jedinici vremena).
  
  
  ## Modbus TCP convert to RTU 
  
  Ukoliko je temperatura Servera veca od zadatog uslova, Master šalje zahtjev za uključivanje releja.
  Komunikacija sa relejom se odvija pomoću serijske linije RS485 koji je spojen na treći RPi. 
  Za serijsku komunikaciju se koristi Modbus RTU protokol.
  Naredna funkcija definiše Modbus RTU komunikaciju, tj. ima ulogu da dodijeli i inicijalizuje modbus_t strukturu za komunikaciju u RTU režimu na serijskoj liniji.
   ```
    modbus_new_rtu("/dev/ttyAMA0", 9600, 'N', 8, 1);
  ```
  Argument /dev/ttyAMA0 specificira ime serijskog porta kojim upravlja Operativni sistem.
  Drugi argument ima vrijednost 9600 i predstavlja brzinu prenosa komunikacije, dok data_bits određuje broj bitova podataka i u našem slučaju iznosi 8.
  Na kraju, imamo i jedan stop_bit.
  
   ```
    modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
 ```
  Ova funkcija postavlja serijsku liniju za RS-485 komunikaciju.
  
  ``` 
     modbus_write_bit(ctx, COIL_ADDRESS, TRUE); 
  ```
Kao krajnji uređaj koristimo relej koji se uključuje upisivanjem logičke jedinice u njegovu adresu.

Više o samoj komunikaciji uređaja pomoću Modbus TCP protokola prikazano je u videu.[Modbus TCP-video](https://youtu.be/uY2jOmN6ZY0)

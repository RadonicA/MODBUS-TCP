# MODBUS

Modbus je serijski komunikacioni protokol predstavljen 1979. godine od strane kompanije Modicon.
Modbus je aplikacioni protokol koji dodjejuje načine upravljanja i prosljedjivanja podataka izmedju različitih slojeva bez uticaja protokola koji koristi sljedeći neporedni sloj.
Ovaj protokol je master/slave protokol sa half-duplex prenosom iz grupe Filedbus protokola.
Koriste se tri tipa Modbus prenosa:
1. Modbus RTU- poruke se sastoje od binarnih bajtova
2. Modbus TCP
3. ASCII

#MODBUS-TCP

Modbus TCP/IP (Transmission Control protocol/Internet protocol) je jednostavan Modbus protokol koji radi na Eternetu preko TCP interfejsa.
Modbus TCP/IP je takodje poznat kao Modbus TCP.
Kada se Modbus informacije šalju korišćenjem ovih protokola, podaci se prosleđuju u TCP gdje se dodatne informacije prilažu i daju IP-u.IP zatim stavlja podatke u paket i prenosi ih.
Ovaj protokol je zapravo verzija Modbus RTU transfera koja je prilagodjena Ethernet okruženju.
Modbus TCP koristi RTU binarni prenos sa TCP/IP detekcijom greške u poruci ili transferu.

Za slanje i primanje podataka pomoću uredjaja koji koriste Modbus prokol koriste se biblioteka [libmodbus](https://libmodbus.org/).
Ova biblioteka sadrži različite pozadine za komunikaciju preko raličitih mreža.
http://www.modbus.org stranica pruža dokumentaciju o Modbus specifikacijama i vodičima za implemetaciju.


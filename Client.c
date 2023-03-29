#include <stdio.h>
#include <stdlib.h>
#include <modbus.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {

modbus_t *ctx;
// kreiranje novog konteksta za Modbus TCP protokol
ctx = modbus_new_tcp("192.168.100.102", 502);// IP adresa servera sa kojim klijent zeli da komunicira je 192.168.100.102
    // port 502 je predefinisan za Modbus TCP komunikaciju
if(ctx==NULL){
   // ukoliko kontekst nije kreiran potrebno je ispisati gresku
printf("Unable to allocate libmodbus context\n");
return -1;
}
uint16_t tab_reg[20];// alocira registre tipa unsigned int
int rc,i;
modbus_set_debug(ctx, 1);
modbus_connect(ctx); // ostvaruje konekciju na dati server
while(1)
{
rc = modbus_read_registers(ctx, 0, 10, tab_reg); // cita registre na udaljenom serveru

if (rc == 10) {
printf("Read registers: ");
for (i=0; i<10; i++)
printf("%f\n", *(float*)tab_reg+i); // ispisuje procitane registre u float formatu na konzolu
}
else {
   fprintf(stderr, "Failed to read registers: %s\n", modbus_strerror(errno));
}
sleep(3);
}
modbus_close(ctx);
modbus_free(ctx); // zatvaranje i oslobadjanje konteksta

return 0;
}

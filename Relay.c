#include <stdio.h>
#include <unistd.h>
#include <modbus.h>

#define SERVER_ID 1 //SERVER SA KOJIM MI KOMUNICIRAMO-SLAVE
#define COIL_ADDRESS 0 //COIL U NASEM SLUCAJU JE RELEJNI IZLAZ, ADRESA JE NULA  
#define BCM_PIN_DE 6 //PIN NA RASPBERRY-PI
#define BCM_PIN_RE 8 //PIN NA RASPBERRY-PI

int main()
{

modbus_t *ctx;

//KREIRA NOVI KONTEKS ZA MODBUS PROTOKOL
ctx = modbus_new_rtu("/dev/ttyAMA0", 9600, 'N', 8, 1); // BEZ PARNOSTI, 8 BITA DUZINA I JEDAN STOP BIT
if (ctx == NULL) {
fprintf(stderr, "Unable to create the libmodbus context!\n");
return -1;
        }

//SETOVANJE SLAVE
modbus_set_slave(ctx,SERVER_ID); //BROJ SLAVE; ADRESA SLAVE SA KOJIM KOMUNICIRAMO
modbus_set_debug(ctx, TRUE); //DETALJNIJE INFORMACIJE MODBUS KOMUNIKACIJI

//KONEKTOVANJE NA MODBUS MREZU
if (modbus_connect(ctx) == -1) { //OSTVARUJE KOMUNIKACIJU SA URAT INTERFEJSOM
fprintf(stderr, "Connection failed.\n");
modbus_free(ctx);
return -1;
}

modbus_enable_rpi(ctx, TRUE); //OMOGUCUVANJE GPIO PIN KOJIM KONTROLISEMO RESIVER
modbus_configure_rpi_bcm_pins(ctx, BCM_PIN_DE, BCM_PIN_RE); //KOJIM PINOM KONTROLISEMO
modbus_rpi_pin_export_direction(ctx);

// Do some communication over Modbus
modbus_write_bit(ctx, COIL_ADDRESS, TRUE); //
sleep(5);
modbus_write_bit(ctx, COIL_ADDRESS, FALSE);

// Closing the connection
modbus_rpi_pin_unexport_direction(ctx); //ONEMOGUCIMO PRISTUP PINOVIMA RPI
modbus_close(ctx);
modbus_free(ctx);
return 0;
}

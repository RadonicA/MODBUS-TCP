#include <modbus.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define SERVER_ID 1 //SERVER SA KOJIM MI KOMUNICIRAMO-SLAVE
#define COIL_ADDRESS 0 //COIL U NASEM SLUCAJU JE RELEJNI IZLAZ, ADRESA JE NULA  
#define BCM_PIN_DE 6 //PIN NA RASPBERRY
#define BCM_PIN_RE 8

int main()
{
 
    modbus_t *ctx;
    uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
    int rc;
    int socket;
   
    // Modbus TCP initialization
    ctx = modbus_new_tcp("192.168.100.104", 502);
    modbus_set_slave(ctx, SERVER_ID);
    socket = modbus_tcp_listen(ctx, 1);
    modbus_tcp_accept(ctx, &socket);
 
    rc = modbus_receive(ctx, query);
        if (rc > 0)
        {
                 modbus_send_raw_request(ctx, query, rc);
modbus_new_rtu("/dev/ttyAMA0", 9600, 'N', 8, 1);
    modbus_set_slave(ctx, SERVER_ID);
    modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
    modbus_set_debug(ctx, TRUE);

        modbus_enable_rpi(ctx, TRUE); //OMOGUCUVANJE GPIO PIN KOJIM KONTROLISEMO RELEJ
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
}

return 0;      
}

#include "mma8451_pi.h"
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void write_byte(mma8451 handle, int reg, char data)
{
    char write_buffer[2];
    write_buffer[0] = reg & 0xFF;
    write_buffer[1] = data;
    write(handle, write_buffer, 2);
}

char read_byte(mma8451 handle, int reg)
{
    char write_buffer[2];
    char data_read[1];
    write_buffer[0] = reg & 0xFF;
    write(handle, write_buffer, 1);
    read(handle, data_read, 1);
    return data_read[0];
}

void read_stream(mma8451 handle, int reg, char* output, size_t len)
{
    size_t i; for(i = 0; i < len; i++)
    {
        output[i] = read_byte(handle, reg + i);
    }
}

mma8451 mma8451_initialise(int device, int addr)
{
    mma8451 handle = -1;
    char buf[15];

    sprintf(buf, "/dev/i2c-%d", device);
    if ((handle = open(buf, O_RDWR)) < 0) return -2;
    if(ioctl(handle, I2C_SLAVE, addr) < 0) return -3;
    
    //Check if we read correctly from the sensor
    char value = read_byte(handle, 0x0D);
    printf("whoami read %#02x\n", value);


    //Send reset request
    write_byte(handle, 0x2B, 0x40);
    printf("Waiting for accelerometer to be reset\n");
    while(read_byte(handle, 0x2B) & 0x40); //reset done
    printf("Done\n");

    write_byte(handle, 0x0E, 0x00); //2G range
    write_byte(handle, 0x2B, 0x02); //high resolution mode
    write_byte(handle, 0x2A, 0x01 | 0x04); //high rate low noise
    //Deactivate fifo
    write_byte(handle, 0x09, 0);
    return handle;
}

void mma8451_get_raw_sample(mma8451 handle, char* output)
{
    read_stream(handle, 0x01, output, 6);
}


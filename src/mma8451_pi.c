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
    char write_buffer[3];
    write_buffer[0] = (reg >> 8) & 0xFF;
    write_buffer[1] = reg & 0xFF;
    write_buffer[2] = data;
    write(handle, write_buffer, 3);
}
int read_byte(mma8451 handle, int reg)
{
    char write_buffer[2];
    char data_read[1];
    write_buffer[0] = (reg >> 8) & 0xFF;
    write_buffer[1] = reg & 0xFF;
    read(handle, data_read, 1);
}

mma8451 mma8451_initialise(int device, int addr)
{
    mma8451 handle = -1;
    char buf[15];

    sprintf(buf, "/dev/i2c-%d", device);
    if ((handle = open(buf, O_RDWR)) < 0) return -2;
    if(ioctl(handle, I2C_SLAVE, addr) < 0) return -3;

    return handle;
}


#include "mma8451_pi.h"
#include <linux/i2c-dev.h>
//#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void write_byte(mma8451 handle, int reg, char data)
{
/*    char write_buffer[2];
    write_buffer[0] = reg & 0xFF;
    write_buffer[1] = data;
    write(handle, write_buffer, 2);

*/

    unsigned char outbuf[2];
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[1];

    messages[0].addr  = handle.address;
    messages[0].flags = 0;
    messages[0].len   = sizeof(outbuf);
    messages[0].buf   = outbuf;

    /* The first byte indicates which register we'll write */
    outbuf[0] = reg;

    /*
     *      * The second byte indicates the value to write.  Note that for many
     *           * devices, we can write multiple, sequential registers at once by
     *                * simply making outbuf bigger.
     *                     */
    outbuf[1] = data;

    /* Transfer the i2c packets to the kernel and verify it worked */
    packets.msgs  = messages;
    packets.nmsgs = 1;
    if(ioctl(handle.file, I2C_RDWR, &packets) < 0) {
        perror("Unable to send data");
    }

}

char read_byte(mma8451 handle, int reg)
{
/*    char write_buffer[2];
    char data_read[1];
    write_buffer[0] = reg & 0xFF;
    write(handle, write_buffer, 1);

    read(handle, data_read, 1);
    return data_read[0];*/


    unsigned char inbuf, outbuf;
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];

    /*
     *      * In order to read a register, we first do a "dummy write" by writing
     *           * 0 bytes to the register we want to read from.  This is similar to
     *                * the packet in set_i2c_register, except it's 1 byte rather than 2.
     *                     */
    outbuf = reg;
    messages[0].addr  = handle.address;
    messages[0].flags = 0;
    messages[0].len   = sizeof(outbuf);
    messages[0].buf   = &outbuf;

    /* The data will get returned in this structure */
    messages[1].addr  = handle.address;
    messages[1].flags = I2C_M_RD/* | I2C_M_NOSTART*/;
    messages[1].len   = sizeof(inbuf);
    messages[1].buf   = &inbuf;

    /* Send the request to the kernel and get the result back */
    packets.msgs      = messages;
    packets.nmsgs     = 2;
    if(ioctl(handle.file, I2C_RDWR, &packets) < 0) {
        perror("Unable to send data");
        return -1;
    }

    return inbuf;
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
    mma8451 handle;
    handle.file = -1;
    handle.address = addr;
    char buf[15];

    sprintf(buf, "/dev/i2c-%d", device);
    if ((handle.file = open(buf, O_RDWR)) < 0) 
    {
        handle.file = -2;
        return handle;
    }
    if(ioctl(handle.file, I2C_SLAVE, addr) < 0) 
    {
        handle.file = -3;
        return handle;
    }

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


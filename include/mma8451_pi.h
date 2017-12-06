#ifndef MMA8451_PI_INCLUDED
#define MMA8451_PI_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mma8451_
{
    int file;
    unsigned char address;
} mma8451;

#define MMA8451_DEFAULT_ADDR 0x1D
#define MMA8451_PULLDOWN_ADDR 0x1C

mma8451 mma8451_initialise(int device, int i2c_addr);
void mma8451_get_raw_sample(mma8451 handle, char* output);

typedef struct mma8451_vector3_
{
    float x, y, z;
} mma8451_vector3;


#ifdef __cplusplus
}
#endif

#endif

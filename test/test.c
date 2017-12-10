#include <stdio.h>
#include <stdlib.h>
#include <mma8451_pi/mma8451_pi.h>

int main(int argc, char** argv)
{
    mma8451 sensor = mma8451_initialise(1, MMA8451_DEFAULT_ADDR);
    mma8451_vector3 acceleration;
    size_t i; 
    for(i = 0; i < 3; ++i)
    {
        mma8451_get_acceleration(&sensor, &acceleration);
        printf("acceleration = (%f, %f, %f)\n", acceleration.x, acceleration.y, acceleration.z);
    }

    return 0; 
}

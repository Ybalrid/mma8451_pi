# mma8451_pi

Library to use the MMA8451 accelerometer sensor via I²C on a Raspberry Pi in C.

Distributed under the terms of the MIT license agreement.

Makefile and inspration from this library for another I²C sensor https://github.com/leachj/vl6180_pi

Code that does read/write in i2c with repeated start correctly is borrowed from this https://bunniestudios.com/blog/images/infocast_i2c.c

Somewhat based on the Arduino library here https://github.com/adafruit/Adafruit_MMA8451_Library

## Installation

Build and install the library with the makefile. The makefile will, by default, install everything into /usr/local/{inlcude;lib}

Note to raspbian user: with this configuration, you may need to add the "/usr/local" path to your ldconfig

```bash
# echo "/usr/local/lib" > /etc/ld.conf.so.d/local.conf;
# ldconfig #to make the system know that theses libraries are here
```

## Usage

```C

#include <mma8451_pi/mma8451_pi.h>

int main()
{
  //Initialise the sensor on the 1st i2c-dev file (/dev/i2c-1) at it's default address (0x1D)
  mma8451 sensor = mma8451_initialise(1, MMA8451_DEFAULT_ADDRESS);
  
  //configure the max range, can be 2G, 4G or 8G
  mma8451_set_range(&sensor, 4); //For a maximum of a 4G acceleartion vector
  
  //get a measurement
  mma8451_vector3 acceleration = mma8451_get_acceleration_vector(&sensor);
  
  //There's less overhead on letting the library fill in your own object
  mma8451_vector3 vect;
  mma8451_get_acceleration(&sensor, &vect);
  
  return 0;
}

```

For C++ users, an operator<< overload on std::ostream permit you to easilly print the value of a `mma8451_vector3`. This is a stuct that exposes 3 x, y and z floats. It contains the acceleration vectors wirtten in G. 1G ~= 9.81 m.s^(-2)

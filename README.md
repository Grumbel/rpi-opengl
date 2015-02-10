Raspberry Pi - OpenGL ES
========================

The version of SDL2 that is shipping with Raspbian does not support
OpenGL ES, it is necessary to compile. Prior to compilation of SDL2 it
is recomment to run:

    sudo apt-get remove mesa-common-dev

The availability of an alternative OpenGL installation seems to
confuse the configure script.

Compiling from source works like this:

    ./configure --disable-video-x11 --prefix=/home/pi/run/SDL2-2.0.3/ --disable-pulseaudio --disable-esd


References
----------

* README.raspberry-pi in SDL2 source tree


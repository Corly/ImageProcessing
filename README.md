# ImageProcessing
Photoshop no more!

First thing you need to do in order to make this project run is to compile
"corona". You already have a folder of corona binaries. You will need to
be under Linux and you will have to run the command from corona's root folder:
# make

After that you need to add the following line in /etc/environment
LD_LIBRARY_PATH="/usr/local/lib/"

This will point the the folder installation of corona lib.


To compile a c++ file you can just run the folowing:
g++ source.c -lcorona


For more details how to import an image see the documentation in the corona
folder. An actual example of how to use corona you will find in
corona-1.0.2/doc/tutorial.txt

Enjoy! ^^

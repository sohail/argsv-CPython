# Makefile

all		: argsv.c argsv.h argsv.py		
		python argsv.py build


install		:
		cp ./build/lib*/argsv.so ./../

clean		:
		rm -rf build

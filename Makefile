# DetectT compile test

all:
	gcc TestDS.c DS18B20.c -o TestDS -lwiringPi

clean:
	rm -f TestDS

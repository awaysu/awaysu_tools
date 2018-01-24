CC = arm-linux-gcc
INCLUDE = include
CFLAG = -Wall -lpthread
SRC = main.c
EXEC_FILE = cpuloop

all:make_api

make_api:$(SRC)
	$(CC) -o $(EXEC_FILE) $(CFLAG) -I$(INCLUDE) $(SRC)

clean:
	rm -f *.o $(EXEC_FILE)

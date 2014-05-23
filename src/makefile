CC=g++
CFLAGS=-I.
DEPS = MotorCommand.h VirtualMotor.h
OBJ = MotorCommand.o driver.o VirtualMotor.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

driver: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o

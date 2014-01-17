OBJ += sunplayer.o
OBJ += suntimer.o
OBJ += sunsong.o
OBJ += sunconfig.o
OBJ += main.o
OBJ += link.o

CC = arm-linux-gcc
#CC = gcc
CFLAGS = -g -Wall
EXEC_NAME = test
EXEC_PATH = ./$(EXEC_NAME)

$(EXEC_NAME):$(OBJ)
	@echo make ...
	@$(CC) $^ -o $(EXEC_PATH) $(CFLAGS)
	@echo make over
	@echo Execute target is $(EXEC_PATH)
%.o:%.c
	@$(CC) -c -o $@ $< $(CFLAGS)

.PHONY:clean
clean:
	@echo clean ...
	rm $(EXEC_PATH) *.o -rf
	@echo clean over









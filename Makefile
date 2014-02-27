CC = g++

CFLAGS = -Wall -g -O3

RM = rm -f

OBJS = ISWT.o bit_array.o wat_array.o SUWT.o main.o

MAIN = main

$(MAIN): $(OBJS)
	@echo "Compiling"
	@$(CC) $(CFLAGS) $(OBJS) -lm -o $(MAIN)

%.o: %.cpp %.h %.hpp
	@echo "Compiling Objects"
	@$(CC) $(CFLAGS) %< -c
%clean:
	$(RM) $(MAIN) *.0
	clear
run: $(MAIN)
	./$(MAIN)


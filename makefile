#makefile Hearts

TARGET = hearts.out
CC = gcc
VECTOR_PATH = .
#CFLAGS = -ansi -g -pedantic -Wall -I $(VECTOR_PATH) -I $(ADTDEFS_PATH)
CFLAGS = -ansi -g -pedantic -Wall -Werror -I $(VECTOR_PATH)
OBJS = Vector.o Game.o Player.o Round.o Deck.o Card.o HeartsUI.o GenericVectorBubbleSort.o Hearts_main.o
VFLAGS = --log-file=memleak.log --leak-check=full --track-origins=yes -s

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

Vector.o: $(VECTOR_PATH)/Vector.c $(VECTOR_PATH)/Vector.h
	$(CC) $(CFLAGS) -c $(VECTOR_PATH)/Vector.c
	
Game.o: Game.c Game.h
	$(CC) $(CFLAGS) -c Game.c

Player.o: Player.c Player.h
	$(CC) $(CFLAGS) -c Player.c

Round.o: Round.c Round.h
	$(CC) $(CFLAGS) -c Round.c

Deck.o: Deck.c Deck.h
	$(CC) $(CFLAGS) -c Deck.c

Card.o: Card.c Card.h
	$(CC) $(CFLAGS) -c Card.c
	
HeartsUI.o: HeartsUI.c HeartsUI.h
	$(CC) $(CFLAGS) -c HeartsUI.c

GenericVectorBubbleSort.o: GenericVectorBubbleSort.c GenericVectorBubbleSort.h 
	$(CC) $(CFLAGS) -c GenericVectorBubbleSort.c
	
Hearts_main.o: Hearts_main.c Game.h
	$(CC) $(CFLAGS) -c Hearts_main.c

valgrind:
	valgrind $(VFLAGS) ./$(TARGET)
	
clean:
	rm -f $(TARGET) $(OBJS)

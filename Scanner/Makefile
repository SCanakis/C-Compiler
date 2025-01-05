CC = gcc
CFLAGS = -Wall -g -I./src
LDFLAGS = -lm -lcmocka

#Production Code
SRC = src/scanner.c
OBJ = $(SRC:.c=.o)
EXEC = scanner

#Test Code
TEST_SRC = test/test_scanner.c
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST_EXEC = test_scanner

all: $(TEST_EXEC)

#$(EXEC): $(OBJ) 
#	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

$(TEST_EXEC): $(TEST_OBJ) $(OBJ) 
	$(CC) $(TEST_OBJ) $(OBJ) -o $(TEST_EXEC) $(LDFLAGS)
$(TEST_OBJ) : $(TEST_SRC)
	$(CC) $(CFLAGS) -c $(TEST_SRC) -o $(TEST_OBJ)

$(OBJ) : $(SRC)
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ)


clean: 
	rm -f $(OBJ) $(TEST_OBJ) $(EXEC) $(TEST_EXEC)




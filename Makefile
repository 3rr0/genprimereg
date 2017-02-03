PROG_NAME = primereg

INC_DIR   = inc
SRC_DIR   = src
OBJ_DIR	  = src/obj

INC		  = $(wildcard $(INC_DIR)/*.c)
SRC	   	  = $(wildcard $(SRC_DIR)/*.c)
OBJ_INC	  = $(patsubst $(INC_DIR)/%.c, $(OBJ_DIR)/%.o, $(INC))
OBJ_SRC	  = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC)) 

LLIBS	  = -lm

CC		  = gcc
CFLAGS	  = -Wall -std=c11 -I$(INC_DIR) $(LLIBS)



.PHONY: all uninstall clean
all: $(PROG_NAME)


$(PROG_NAME): $(OBJ_INC) $(OBJ_SRC)
	$(CC) $^ -o $(PROG_NAME) $(CFLAGS)

$(OBJ_INC): $(OBJ_DIR)/%.o: $(INC_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_SRC): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)


uninstall:
	rm -f $(PROG_NAME)
	make clean

clean:
	rm -f $(OBJ_DIR)/*.o


#CFLAGS += -std=c99 -Wall -Wextra -Wundef -Wshadow -Wcast-align -Wstrict-prototypes
OPT_CFLAGS += -std=gnu11 -Wextra -Wundef -Wshadow -Wcast-align -Wstrict-prototypes -DNDEBUG -O2 
CFLAGS += -std=gnu11 -Wextra -Wundef -Wshadow -Wcast-align -Wstrict-prototypes -g
FLAGS  := -I./lib $(CPPFLAGS) $(CFLAGS) $(LDFLAGS)
OPT_FLAGS  := -I./lib $(CPPFLAGS) $(OPT_CFLAGS) $(LDFLAGS)

TESTFILE= Makefile
LZ4DIR := ./lib

SOURCE=main.c $(LZ4DIR)/lz4.c utils.c
HEADER=$(shell ls *.h)
TARGET=test-main
TARGET_OPT=test-main.opt
EXT =
OBJ=.o

LIB=-lm -lcrypto -lrt
CLEAN_LIST=*.o $(TARGET) $(TARGET_OPT)

all: debug opt 

debug: $(TARGET)
$(TARGET): $(SOURCE) 
	#@rm -f $(CLEAN_LIST)
	$(CC)  $(FLAGS) $^ -o $@$(EXT) $(LIB)

opt: $(TARGET_OPT)
$(TARGET_OPT): $(SOURCE) 
	#@rm -f $(CLEAN_LIST)
	$(CC)  $(OPT_FLAGS) $^ -o $@$(EXT) $(LIB)

clean:
	@echo remove $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)
	@echo Cleaning completed


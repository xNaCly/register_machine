MANDATORY_FLAGS := -fdiagnostics-color=always  \
									 -Wall \
									 -Wpedantic \
									 -std=c99

MY_FLAGS := -Wextra \
						-Werror \
						-Wshadow \
						-Wundef \
						-fno-common \

BUILD_DIR := ./dist
SRC_DIR := .

FILES := $(shell find $(SRC_DIR) -name "*.c") 

COMPILE := $(MANDATORY_FLAGS) $(MY_FLAGS) $(FILES) -lm -o $(BUILD_DIR)/regmach.out

all: main
	$(BUILD_DIR)/regmach.out example.regmach

main: pre
	gcc $(COMPILE)

debug: pre
	gcc -g3 $(COMPILE) 
	gdb $(BUILD_DIR)/regmach.out

pre: 
	mkdir -p $(BUILD_DIR) 

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) 

CFLAGS = -o2 -Ilibs

# Добавление всех поддиректорий
INCLUDE_DIRS := $(shell find ./src -type d)
CFLAGS += $(addprefix -I,$(INCLUDE_DIRS))

OUTPUT_DIR = build
SRC_DIR = src

build:
	gcc -Ilibs $(CFLAGS) ./src/*.c ./src/*/*.c -o multi_agent_model

run: build
	./multi_agent_model 0.78 0.17 10 100 ./data/2010_ver1_01001_synth_people.txt ./result.txt

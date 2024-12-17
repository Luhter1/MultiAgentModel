CFLAGS += 


# Добавление всех поддиректорий
INCLUDE_DIRS := $(shell find ./src -type d)
CFLAGS += $(addprefix -I,$(INCLUDE_DIRS))

build:
	gcc -Ilibs $(CFLAGS) ./src/*.c ./src/*/*.c -o multi_agent_model

run: build
	./multi_agent_model ./data/2010_ver1_01001_synth_people.txt ./result.txt

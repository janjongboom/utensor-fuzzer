NAME = utensor-classifier

CC ?= g++
CFLAGS ?= -Wall

MACROS +=
CFLAGS += -std=c++11 -I. -IuTensor -IuTensor/src -IuTensor/src/uTensor -IuTensor/src/uTensor/core -IuTensor/src/uTensor/loaders -IuTensor/src/uTensor/ops -IuTensor/src/uTensor/util uTensor/libutensor.a -lc++
CFLAGS += -fprofile-arcs -ftest-coverage

LFLAGS += --coverage

all: build

.PHONY: build clean utensor-model

utensor-model:
	utensor-cli convert model/trained.pb --output-nodes=y_pred/Softmax -m source

build:
	mkdir -p build
	$(CC) $(MACROS) $(CFLAGS) $(LFLAGS) source/*.cpp -o build/$(NAME)

clean:
	rm $(NAME)

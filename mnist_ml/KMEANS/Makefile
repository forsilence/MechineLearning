CC=g++
MNIST_ML_ROOT = /home/blender/c++/MechineLearning/mnist_ml
SRC := $(MNIST_ML_ROOT)/KMEANS/src
CFLAGS := -std=c++11 -DEUCLID
INCLUDE_DIR := $(MNIST_ML_ROOT)/KMEANS/include/*

all : main

main : $(SRC)/kmeans.cc
	$(CC) $(CFLAGS) $(SRC)/kmeans.cc -o main -L$(MNIST_ML_ROOT)/lib -I$(INCLUDE_DIR) \
	-I$(MNIST_ML_ROOT)/include -ldata

clean :
	rm main
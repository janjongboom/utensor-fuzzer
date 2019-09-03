set -e

rm -f *.gcda
rm -f *.gcno
mkdir -p out
rm -f out/*

echo "Building uTensor library"

# build uTensor library file
cd uTensor
cmake .
make

echo "Building uTensor model"

# build utensor model
cd ..
make utensor-model

echo "Fix SoftmaxOp"

# fix the softmaxop (for now)
sed -i '.bak' "s/new SoftmaxOp(/new SoftmaxOp\<float,float\>(/g" ./source/trained.cpp       # note remove the '.bak' on Linux
rm -f ./source/trained.cpp.bak

echo "Building uTensor classifier"

# build the uTensor classifier
make

echo "Classifying..."

# classify using C++ and Python
./build/utensor-classifier  model/X.txt > out/utensor_y.txt
python3 model/classify.py  model/X.txt > out/tensorflow_y.txt

# clear up
rm -f *.gcda
rm -f *.gcno

echo "Comparing output"

# and compare...
node compare.js

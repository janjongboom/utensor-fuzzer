# Compare uTensor and TensorFlow outputs

This is a script that will compare the output between uTensor and TensorFlow. It takes the [trained.pb](model/trained.pb) TensorFlow model file, and the [X.txt](model/X.txt) features file, and uses both uTensor and TensorFlow to classify the items. It can be used to uncover issues where uTensor misclassifies files (e.g. [#170](https://github.com/uTensor/uTensor/issues/170)).

## How to build

1. Clone this repository:

    ```
    $ git clone --recursive https://github.com/janjongboom/utensor-fuzzer
    ```

1. Build the project and compare the output:

    ```
    $ sh build.sh
    ```

## Setting uTensor parameters

The [Makefile](Makefile) contains the uTensor parameters and transform settings.

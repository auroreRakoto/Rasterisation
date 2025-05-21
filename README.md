# Projective Rendering TP

## Compilation 

### From the Makefile 

```sh
cd projet
make
./pgm
```

From the CMakeLists.txt

```sh 
mkdir build
cd build
cmake ..
make
cd ../projet
../build/pgm
```

or

```sh 
cd projet
mkdir ../build
cmake .. -B ../build
make -C ../build
../build/pgm
```

### From an IDE (VSCode, QtCreator, etc).


The execution directory must be inside projet/

That is, the data/ directory must be accessible.

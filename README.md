# TP de rendu projectif

## Compilation 

### Depuis le Makefile 

```sh
cd projet
make
./pgm
```

### Depuis le CMakeLists.txt

```sh 
mkdir build
cd build
cmake ..
make
cd ../projet
../build/pgm
```

ou 

```sh 
cd projet
mkdir ../build
cmake .. -B ../build
make -C ../build
../build/pgm
```

### Depuis un IDE (VSCode, QtCreator, etc).

Le répertoire d'execution doit être dans projet/
C'est a dire que le répertoire data/ doit être accessible.

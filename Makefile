CC=gcc
CFLAGS=`pkg-config --cflags gtk+-3.0`
LFLAGS=`pkg-config --libs gtk+-3.0`
SRC1=proyecto1.c
SRC2=proyecto2.c
EXE1=exe_proyecto1.c
EXE2=exe_proyecto2.c


# Target para compilar proyecto1.c
p1:
      gcc $(CFLAGS) $(SRC1) -o $(EXE) $(LFLAGS)

# Target para compilar proyecto2.c
p2:
      gcc $(CFLAGS) $(SRC2) -o $(EXE) $(LFLAGS)

# Target para limpiar lo archivos generados
clean:
      rm -f $(EXE1) $(EXE2)

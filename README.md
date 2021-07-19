@cirofalsarella
@PedroMartelleto

A database management system that uses B-Trees for quickly retrieving registers.

## Folder structure
    .
    ├── bTree                   # bTree:        B-Tree indexing algorithms & data model
    ├── core                    # core:         Related to the implementation of the operations
    ├── dataModel               # dataModel:    "Constructors" and "destructors" for commonly used registers
    ├── fileIO                  # fileIO:       Binary read/write
    ├── order                   # order:     
    ├── main.c                  # main.c:       The entry point
    └── README.md               # README.md

## Supported operations
    
### Create B-Tree index (op codes 9 & 10)
Creates a B-Tree Index file (outputBTreeIndex is created).

```bash
9 [inputVehicleBinary] [outputBTreeIndex]
```
```bash
10 [inputBusLineBinary] [outputBTreeIndex]
```

Example usage:
```bash
9 vehicles.bin bTreeIndexFile.bin
```

### Get register by key (op codes 11 & 12)
Prints the register with the given key.

```bash
11 [inputVehicleBinary] [inputBTreeIndex] prefixo "value"
```
```bash
12 [inputBusLineBinary] [inputBTreeIndex] codLinha "value"
```

Example usage:
```bash
11 veiculo.bin indicePrefixo.bin prefixo "BA004"
```
```bash
Prefixo do veiculo: BA004
Modelo do veiculo: NEOBUS MEGA
Categoria do veiculo: ALIMENTADOR
Data de entrada do veiculo na frota: 29 de maio de 2009
Quantidade de lugares sentados disponiveis: campo com valor nulo
```

### Insert into the registers in binary file and the B-Tree index (op codes 13 & 14)
```bash
13 [vehicleBinary] [bTreeIndex] [numberToInsert]
prefix1 date1 numSeats1 lineCode1 model1 category1
...
```
```bash
14 [busLineBinary] [bTreeIndex] [numberToInsert]
lineCode1 acceptsCreditCard1 name1 color1
...
```

Example usage:
```bash
13 veiculo.bin indicePrefixo.bin 2
"AT090" "2019-05-20" 30 333 NULO "VERMELHO"
"XX997" "2015-09-30" 20 672 NULO NULO
```
A database management system that uses B-Trees for quickly retrieving registers. University project by @cirofalsarella @PedroMartelleto.

Each branch represents different milestones for the project, with different operations supported.

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

### Brute-force join algorithm (op code 15)
```bash
15 [vehicleBinary] [busLineBinary] [fieldNameVehicle] [fieldNameBusLine]
``` 

### Single-loop B-Tree join algorithm (op code 16)
```bash
16 [vehicleBinary] [busLineBinary] [fieldNameVehicle] [fieldNameBusLine] [busLineIndex]
``` 

### RAM sorting (op codes 17 & 18)
```bash
17 [notSortedVehicles] [sortedVehicles] [fieldNameForSort]
``` 

```bash
18 [notSortedBusLines] [sortedBusLines] [fieldNameForSort]
``` 

### Linear sorted join (op code 19)
```bash
19 [vehiclesFile] [busLinesFile] [fieldNameVehicles] [fieldNameBusLines] 
``` 

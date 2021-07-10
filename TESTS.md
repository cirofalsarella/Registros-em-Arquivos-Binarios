Create table
    - Null input files -> OK

Find by key
    - One or more null input files -> OK
    - Invalid fieldName -> OK
    - Find vehicles (several tests done on veiculo7.bin) -> OK
    - Find vehicles that do not exist -> OK
    - Find bus lines -> OK
    - Find bus lines that do not exist -> OK

* 12 bin/linha11.bin bin/indicePrefixo11.bin codLinha 52 -> Registro inexistente????

Insert
    - Insert w/ null fields -> OK
    - Normal insertion -> OK
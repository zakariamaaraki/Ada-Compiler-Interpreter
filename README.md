# Ada-Compiler-Interpreter
ada compiler + virtual machine for interpreting the pseudo code

## Compile to pseudo code
```
gcc fun_syn_test.c types.h pseudoCode.h pile.h lex.h ast.h ada.h -o compiler
./compiler // this command will read source code from dummy.txt and generate a pseudo code file named generer.txt 
```

## the pseudo code interpreter
```
gcc machine.c pile.h pseudoCode.h -o interpreter
./interpreter
```


## Authors

* **Zakaria Maaraki** - *Initial work* - [BrolyCode](https://github.com/BrolyCode)

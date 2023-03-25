compilar: 
	flex lex.l
	gcc  -Wall -o ejecutable.out *.c -lfl


cleanall: cleanObjetos cleanEjecutables

cleanEjecutables:
	rm -f *.out *.exe

cleanallFicheros:
	rm -f *.txt

cleanObjetos:
	rm -f *.o

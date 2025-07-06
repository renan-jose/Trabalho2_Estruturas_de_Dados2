# Variáveis
PROJ_NAME = ted
ALUNO = renanjose
LIBS = -lm
OBJETOS = main.o lista.o espalhamento.o #graph.o
        
# Compilador
CC = gcc

# Flags
CFLAGS = -ggdb -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration 
LDFLAGS = -O0

# Procedimentos para compilação
$(PROJ_NAME): $(OBJETOS)
	$(CC) -o $(PROJ_NAME) $(LDFLAGS) $(OBJETOS) $(LIBS)
%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

# Dependências de cada módulo
main.o: main.c espalhamento.h #graph.h 

#graph.o: graph.c graph.h lista.h
lista.o: lista.c lista.h
espalhamento.o: espalhamento.c espalhamento.h

# Empacota o projeto em um .zip para entrega, incluindo código-fonte, Makefile e LEIA-ME
pack: $(PROJ_NAME)
	rm -f ../$(ALUNO).zip
	echo $(ALUNO)
	date >> .entrega
	cd ..; zip $(ALUNO).zip -r src/*.c src/*.h src/Makefile LEIA-ME.txt .entrega

clean:
	rm -f *.o $(PROJ_NAME)


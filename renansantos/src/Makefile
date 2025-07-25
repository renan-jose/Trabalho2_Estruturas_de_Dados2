# Variáveis
PROJ_NAME = ted
ALUNO = renanjose
LIBS = -lm
OBJETOS = main.o graph.o smutreap.o lista.o fila.o espalhamento.o arquivo.o forma.o retangulo.o processamento.o tag.o prioridade.o 
        
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
main.o: main.c graph.h espalhamento.h arquivo.h processamento.h

graph.o: graph.c graph.h espalhamento.h fila.h smutreap.h
smutreap.o: smutreap.c smutreap.h lista.h fila.h
lista.o: lista.c lista.h 
fila.o: fila.c fila.h
prioridade.o: prioridade.c prioridade.h
espalhamento.o: espalhamento.c espalhamento.h

processamento.o: processamento.c processamento.h tag.h lista.h arquivo.h prioridade.h graph.h espalhamento.h smutreap.h
tag.o: tag.c tag.h
arquivo.o: arquivo.c arquivo.h tag.h smutreap.h

# Empacota o projeto em um .zip para entrega, incluindo código-fonte, Makefile e LEIA-ME
pack: $(PROJ_NAME)
	rm -f ../$(ALUNO).zip
	echo $(ALUNO)
	date >> .entrega
	cd ..; zip $(ALUNO).zip -r src/*.c src/*.h src/Makefile LEIA-ME.txt .entrega

clean:
	rm -f *.o $(PROJ_NAME)


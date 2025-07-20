# Variáveis
PROJ_NAME = ted
ALUNO = renanjose
LIBS = -lm
OBJETOS = main.o smutreap.o lista.o fila.o forma.o retangulo.o circulo.o linha.o texto.o armazenamento.o processamento.o tag.o arquivo.o
        
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
main.o: main.c armazenamento.h processamento.h smutreap.h arquivo.h

smutreap.o: smutreap.c smutreap.h lista.h fila.h
lista.o: lista.c lista.h 
fila.o: fila.c fila.h

forma.o: forma.c forma.h retangulo.h circulo.h linha.h texto.h
retangulo.o: retangulo.c retangulo.h 
circulo.o: circulo.c circulo.h 
linha.o: linha.c linha.h smutreap.h
texto.o: texto.c texto.h smutreap.h

armazenamento.o: armazenamento.c armazenamento.h forma.h smutreap.h arquivo.h
processamento.o: processamento.c processamento.h smutreap.h forma.h arquivo.h tag.h
tag.o: tag.c tag.h forma.h

arquivo.o: arquivo.c arquivo.h tag.h smutreap.h

# Empacota o projeto em um .zip para entrega, incluindo código-fonte, Makefile e LEIA-ME
pack: $(PROJ_NAME)
	rm -f ../$(ALUNO).zip
	echo $(ALUNO)
	date >> .entrega
	cd ..; zip $(ALUNO).zip -r src/*.c src/*.h src/Makefile LEIA-ME.txt .entrega

clean:
	rm -f *.o $(PROJ_NAME)


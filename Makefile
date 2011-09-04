
PROGRAMA=drawbstree
FONTES=draw_tree.c tree.c
INCLUDES=.

BIBLIOTECAS=m

INSTALL=/usr/bin/install

CC=cc
CFLAGS=-Wall -O2

default: $(PROGRAMA)

$(PROGRAMA): $(FONTES:.c=.o)
	$(CC) $(CFLAGS) -I$(INCLUDES) $(FONTES) -o$(PROGRAMA) -l$(BIBLIOTECAS)

all: $(PRORAMA)

clean:
	$(RM) -f $(PROGRAMA) $(FONTES:.c=.o)

install: all
	$(INSTALL) desenhar.sh /usr/local/bin/desenhar
	$(INSTALL) drawbstree /usr/local/bin/

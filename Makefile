CC=gcc
OUTDIR=out

.PHONY: clean outdir

$(OUTDIR)/clox: outdir $(OUTDIR)/main.o $(OUTDIR)/chunk.o $(OUTDIR)/memory.o $(OUTDIR)/debug.o $(OUTDIR)/value.o
	$(CC) -o $(OUTDIR)/clox $(OUTDIR)/main.o $(OUTDIR)/chunk.o $(OUTDIR)/memory.o $(OUTDIR)/debug.o $(OUTDIR)/value.o

outdir:
	mkdir -p $(OUTDIR)/

$(OUTDIR)/main.o: main.c common.h
	$(CC) -o $(OUTDIR)/main.o -c main.c

$(OUTDIR)/chunk.o: chunk.c chunk.h
	$(CC) -o $(OUTDIR)/chunk.o -c chunk.c

$(OUTDIR)/memory.o: memory.c memory.h
	$(CC) -o $(OUTDIR)/memory.o -c memory.c

$(OUTDIR)/debug.o: debug.c debug.h
	$(CC) -o $(OUTDIR)/debug.o -c debug.c

$(OUTDIR)/value.o: value.c value.h
	$(CC) -o $(OUTDIR)/value.o -c value.c

clean:
	rm -rf $(OUTDIR)

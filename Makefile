CC=gcc
CFLAGS=
LDFLAGS= -lncurses
SOURCES= main.c Buffer.c HandleInput.c Redraw.c UTF_8.c
OBJECTS= $(SOURCES:.c=.o)
EXECUTABLE= main.app

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
		$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.INTERMEDIATE: $(OBJECTS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
		rm -f $(OBJECTS) $(EXECUTABLE)

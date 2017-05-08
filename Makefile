SOURCES.c = bonus2.c  changedir.c  echocall.c  execute.c  getcurrdir.c  getprompt.c  replacebytilda.c  main.c segregate.c getInput.c setInput.c setOutput.c test.c checkForPipe.c
INCLUDES=
CFLAGS=
SLIBS=
PROGRAM= main

OBJECTS= $(SOURCES.c:.c=.o)

.KEEP_STATE:

debug := CFLAGS= -g

all debug: $(PROGRAM)

$(PROGRAM): $(INCLUDES) $(OBJECTS)
	$(LINK.c) -o $@ $(OBJECTS) $(SLIBS)

clean:
	rm -f $(PROGRAM) $(OBJECTS)

SRCDIR = src
BINDIR = bin
EXECUTABLE = NPuzzleSolver

CC = g++
CFLAGS = -c -O3 -std=c++11
LDFLAGS = 

TARGET = $(BINDIR)/$(EXECUTABLE)
SOURCES = $(wildcard $(addsuffix /*.cpp,$(SRCDIR)))
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

.PHONY: all
all: $(TARGET)

$(OBJECTS): %.o : %.cpp
	$(CC) $(CFLAGS) -o $@ $<

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS)

$(BINDIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -f $(SRCDIR)/*.o
	rm -rf $(BINDIR)

.PHONY: run
run: all
	@$(BINDIR)/$(EXECUTABLE)

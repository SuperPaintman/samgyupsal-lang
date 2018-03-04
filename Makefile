CC := clang
CFORMAT := clang-format

SRCDIR = src
BUILDDIR = build
BINDIR = bin

TARGET = $(BINDIR)/cli.exe

SRCEXT = c

HEADERS = $(shell find $(SRCDIR) -type f -name *.h)
SOURCES = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

CFLAGS := \
	-Wall \
	-Werror \
	-Wempty-body \
	-Wignored-qualifiers \
	-Wmissing-field-initializers \
	-Wsign-compare \
	-Wtype-limits \
	-Wuninitialized
	# -pedantic
LIB :=
INC :=

all: $(TARGET)
	@echo ""

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

rebuild: clean $(TARGET)
	@echo ""

format:
	$(CFORMAT) -i $(HEADERS) $(SOURCES)

clean:
	$(RM) -rf $(BUILDDIR)/* $(BINDIR)/*

.PHONY: rebuild, clean

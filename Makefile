DIRS := $(wildcard */.)

all: $(DIRS)
$(DIRS):
	$(MAKE) -C $@

.PHONY: all clean $(DIRS)


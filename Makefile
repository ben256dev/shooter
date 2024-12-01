FINDGOAL=$(findstring $(1),$(MAKECMDGOALS))

ifeq ($(call FINDGOAL,rel),rel)
	BINDIR	:=bin/rel
	CFLAGS	:=$(CFLAGS) -O2
	LDFLAGS	:=$(LDFLAGS)
else
	BINDIR	:=bin/dbg
	CFLAGS	:=$(CFLAGS) -O0 -g
	LDFLAGS	:=$(LDFLAGS)
endif

LDFLAGS_LIB	=$(shell pkg-config --libs sdl3)
CFLAGS_LIB	=$(shell pkg-config --cflags sdl3)
SRCS	:=$(shell find . -type f -name "*.[c|h]")
GETOBJS	=$(patsubst %.c,$(BINDIR)/%.o,$(filter %.c,$(1)))
OBJS	:=$(call GETOBJS,$(SRCS))
TARGET	:=shooter

.INTERMEDIATE: SDL3
.DEFAULT_GOAL: dbg
dbg: $(BINDIR)/$(TARGET) SDL3
rel: $(BINDIR)/$(TARGET) SDL3

SDL3:
	@if [ $$(pkg-config --exists sdl3) ]; then \
		tools/build_sdl3.sh; \
	fi
$(BINDIR)/$(TARGET): $(OBJS)
	mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) $(LDFLAGS_LIB) $^ -o $@
run: $(BINDIR)/$(TARGET)
	$<

# $(1) source file
define MAKEOBJ
$(BINDIR)/$(dir $(1))$(shell gcc -M $(1) | tr -d '\\\n')
	mkdir -p $$(dir $$@)
	$(CC) $(CFLAGS) $(CFLAGS_LIB) -c $$< -o $$@
endef

$(foreach src,$(SRCS),$(eval $(call MAKEOBJ,$(src))))

.PHONY: clean
clean:
	rm -rf bin/


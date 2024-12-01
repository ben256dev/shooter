FINDGOAL=$(findstring $(1),$(MAKECMDGOALS))

ifeq ($(call FINDGOAL,rel),rel)
	BINDIR	:=bin/rel
	CFLAGS	:=$(CFLAGS) -O2 -NDEBUG
	LDFLAGS	:=$(LDFLAGS)
else
	BINDIR	:=bin/dbg
	CFLAGS	:=$(CFLAGS) -O0 -g
	LDFLAGS	:=$(LDFLAGS)
endif

LDFLAGS_LIB	=$(shell pkg-config --libs sdl3)
CFLAGS_LIB	=$(shell pkg-config --cflags sdl3)
SDL3_EXISTS	:=$(shell pkg-config --exists sdl3; echo $$?)
SRCS	:=$(shell find src -type f -name "*.c")
GETOBJS	=$(patsubst %.c,$(BINDIR)/%.o,$(filter %.c,$(1)))
OBJS	:=$(call GETOBJS,$(SRCS))
TARGET	:=shooter

.DEFAULT_GOAL: dbg
dbg: $(BINDIR)/$(TARGET)
rel: $(BINDIR)/$(TARGET)

# It exists so run this
ifeq ($(SDL3_EXISTS),0)
$(BINDIR)/$(TARGET): $(OBJS)
	mkdir -p $(dir $@)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDFLAGS_LIB)
run: $(BINDIR)/$(TARGET)
	$<
gdb: $(BINDIR)/$(TARGET)
	gdb $<
lldb: $(BINDIR)/$(TARGET)
	lldb $<

# $(1) source file
define MAKEOBJ
$(BINDIR)/$(dir $(1))$(shell gcc -M $(1) | tr -d '\\\n')
	mkdir -p $$(dir $$@)
	$(CC) -c $$< -o $$@ $(CFLAGS) $(CFLAGS_LIB)
endef

$(foreach src,$(SRCS),$(eval $(call MAKEOBJ,$(src))))
else
# Build it first
$(BINDIR)/$(TARGET):
	# Build SDL3
	tools/build_sdl3.sh

	# Rebuild after we've built it
	$(MAKE) $(MAKECMDGOALS)
endif

# See if we are on an apple system
UNAME		:=$(shell uname)

# Build the shaders
GLSL_DIR	:=shaders/glsl
GLSL_SRCS	:=$(shell find $(GLSL_DIR) -type f -name "*.glsl")
GLSL_FLAGS	:=-x glsl -O
GET_GLSL_OUT	=$(patsubst %,res/%.spv,$(1))
METAL_DIR	:=shaders/metal
METAL_SRCS	:=$(shell find $(METAL_DIR) -type f -name "*.metal")
METAL_FLAGS	:=-Wall -O3
GET_METAL_OUT	=$(patsubst %,res/%.metallib,$(1))

# $(1) input
# $(2) output
define MAKEGLSL
$(2): $(1)
	mkdir -p $$(dir $$@)
	glslc $(GLSL_FLAGS) $$^ -o $$@
endef

# $(1) input
# $(2) output
define MAKEMETAL
$(2): $(1)
	mkdir -p $$(dir $$@)
	intermediate=$(patsubst %.metallib,%.ir,$(2));\
	xcrun -sdk macosx metal -o $$$intermediate -c $$^;\
	xcrun -sdk macosx metallib -o $$@ $$$intermediate
endef

ifeq ($(UNAME),Darwin)
# Metal

# Shader target (all shaders)
shaders: $(call GET_METAL_OUT,$(METAL_SRCS))

# Build shader sources
$(foreach src,$(METAL_SRCS),$(eval $(call MAKEMETAL,$(src),$(call GET_METAL_OUT,$(src)))))
else
# Glsl

# Shader target (all shaders)
shaders: $(call GET_GLSL_OUT,$(GLSL_SRCS))

# Build shader sources
$(foreach src,$(GLSL_SRCS),$(eval $(call MAKEGLSL,$(src),$(call GET_GLSL_OUT,$(src)))))
endif

.PHONY: clean
clean:
	rm -rf bin/ res/shaders/

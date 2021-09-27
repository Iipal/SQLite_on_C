.EXPORT_ALL_VARIABLES:

NAME := $(notdir $(CURDIR))
NPWD := $(CURDIR)/$(NAME)

# all source code .c files must to places in to "src" folder(or sub-dirs) only.
ifneq (,$(wildcard ./src))
SRCS := $(shell find src -name "*.c")
OBJS := $(SRCS:.c=.o)
ASMS := # reversed for 'assembly' and 'assembly_all' rules where ASMS:=$(OBJS:%.o=%.S) and OBJS:= sets to nothing
else
$(error "all source code .c files must to places in to "src" folder(and sub-dirs) only.")
endif

# all header .h files must be placed in to "include" folder(or sub-dirs) only.
I_PATHS := $(shell find . -name "include" 2> /dev/null)
ifneq (,$(I_PATHS))
IFLAGS := $(addprefix -I,$(foreach ip,$(I_PATHS),$(abspath $(shell find $(ip) -type d))))
else
$(error "all header .h files must be placed in to "include" folder(and sub-dirs) only.")
endif

# Set for include and to parse additional library headers dependencies.
# Set only parent-folder for all headers, all sub-dirs will be added automatically.
HEADER_DEPS :=
ifneq (,$(HEADER_DEPS))
# Checks if each path in HEADER_DEPS is valid.
 ifneq (,$(filter-out $(foreach dep,$(HEADER_DEPS),$(wildcard $(dep))),$(HEADER_DEPS)))
$(error "Some of the configs/base.mk:$$HEADER_DEPS additional library headers dependencies have are an invalid path.")
 endif
IFLAGS += $(addprefix -I,$(shell find $(foreach d,$(HEADER_DEPS),$(abspath $d)) -type d))
endif

# Compiler settings.
CC     := clang
CFLAGS := -Wall -Wextra -Werror -Wunused -MMD -std=c11

ifeq (,$(shell command -v clang 2> /dev/null))
CC     := gcc
endif

CFLAGS_OPTIMIZE       := -march=native -mtune=native -Ofast -pipe -flto -fpic
CFLAGS_DEBUG          := -g3
CFLAGS_SANITIZE       := $(CFLAGS_DEBUG) -fsanitize=address
CFLAGS_ASSEMBLY       := $(filter-out -flto -fpic,$(CFLAGS_OPTIMIZE)) -S -masm=intel
CFLAGS_DEBUG_ASSEMBLY := $(CFLAGS_DEBUG) -S -masm=intel
CFLAGS_PEDANTIC       := -Wpedantic
CFLAGS_X86            := -m32
CFLAGS_DEBUG_X86      := $(CFLAGS_DEBUG) -m32

CFLAGS_OPTIONAL       := $(CFLAGS_OPTIMIZE)

###############################################################################
#
# Makefile for Audio Bootloader Wavegenerator hex2wav
# Author: Julian Schmidt
#
#
###############################################################################

###############################################################################
# OPTIONS

BINARY ?= hex2wav
MAP ?= $(addprefix $(dir $(BINARY)), hex2wav.map)

ifeq ($(DEBUG),1)
DEFINES += -DDEBUG
else
DEFINES += -DNDEBUG
endif

ifndef OPTIMIZE
OPTIMIZE=0
endif

# if VERBOSE is defined, spam output
ifdef VERBOSE
AT :=
ECHO := @true
LDFLAGS += -v
else
AT := @
ECHO := @echo
endif

CC  =$(addprefix $(BINPATH),g++)
CXX =$(addprefix $(BINPATH),c++)
LD  =$(addprefix $(BINPATH),ld)
CP  =$(addprefix $(BINPATH),objcopy)
OD  =$(addprefix $(BINPATH),objdump)
AS  =$(addprefix $(BINPATH),as)

###############################################################################
# SOURCE FILES
SRCDIR=.
CCSRCFILES  = $(shell find $(SRCDIR) -type f -name "*.cpp" | grep -v '/\.')

vpath %.cpp ./

###############################################################################
# SETUP

OBJDIR=./build/

ELF=$(OBJDIR)hex2wav

# Build object files from source...
OBJFILES = $(addprefix $(OBJDIR),$(notdir $(CCSRCFILES:.cpp=.o)))

# Project defines
#DEFINES += -DF_CPU=20000000UL -D__PROG_TYPES_COMPAT__


CFLAGS += $(DEFINES) $(INCLUDES)
CFLAGS += -O$(OPTIMIZE)
#CFLAGS += -Wall -Wextra -Wconversion -c -funsigned-char -funsigned-bitfields -ffast-math -freciprocal-math -ffunction-sections -fdata-sections -fpack-struct -fshort-enums
CFLAGS += -Wall  -ggdb -c -funsigned-char -funsigned-bitfields -ffast-math -freciprocal-math -ffunction-sections -fdata-sections -fshort-enums
ifeq ($(PEDANTIC),1)
CFLAGS += -Werror
endif
CFLAGS +=  -MD -MP
#LDFLAGS += -Wl,-Map=$(MAP) -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,-gc-sections 

###############################################################################
# TARGETS

# Require explicit target
.PHONY: all
all:
	@echo "Valid targets are"
	@echo " avr : build AVR firmware"
	@echo " clean : clean build directory"
	@echo " printenv : print some debug variables"
	@echo " printfiles : print list of files that would be compiled"

.PHONY: clean
clean:
	@$(RM) $(BINARY)
	@$(RM) $(ELF)
	@$(RM) $(OBJDIR)/*.o

.PHONY: printenv
printenv:
	@echo "AVR_TOOLKIT_ROOT='$(AVR_TOOLKIT_ROOT)'"
	@echo "CC  = $(CC)"
	@echo "CXX = $(CXX)"
	@echo "AS  = $(AS)"

.PHONY: printfiles
printfiles:
	@echo "** C FILES **"
	@echo "$(CCSRCFILES)"
	@echo ""

	@echo "** CXX FILES **"
	@echo "$(CXXSRCFILES)"
	@echo ""

	@echo "** S FILES **"
	@echo "$(ASSRCFILES)"
	@echo ""

.PHONY: avr
avr: $(BINARY)

$(ELF): $(OBJFILES)
	@echo "Linking $@..."
	$(AT)$(CC) $(LDFLAGS) $^ -o $@

$(BINARY): $(ELF)
	@echo "Creating binary $@..."
	cp $(ELF) ./
	

$(OBJFILES) : | $(OBJDIR)

###############################################################################
# BUILD RULES
$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)%.o: %.cpp
	$(ECHO) "Compiling $<..."
	$(AT)$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)%.o: %.S
	$(ECHO) "Assembling $<..."
	$(AT)$(AS) $(ASFLAGS) $< -o $@ > $@.lst

# Automatic dependency generation
CFLAGS += -MMD
-include $(OBJFILES:.o=.d)

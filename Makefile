.SUFFIXES:

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/3ds_rules

TARGET := ctrpf

INCLUDES := include \
	include/capstone \
	$(CURDIR)/include

ARCH := -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft
DEFINES := -D__3DS__ \
	-DCOMMIT_HASH=\"unknown\" \
	-DCOMPILE_DATE=\"$(shell date +%Y-%m-%d)\"

CFLAGS_BASE := \
	-Wno-psabi -mword-relocations -ffunction-sections -fdata-sections \
	-Wno-array-bounds -Wno-stringop-overflow -Wno-stringop-overread \
	-Wno-reorder -Wno-unused-variable -Wno-unused-but-set-variable \
	$(ARCH) $(DEFINES)

ifeq ($(BUILD),debug)
	CFLAGS := $(CFLAGS_BASE) -DDEBUG=1 -g
	OUTDIR := debug
else
	CFLAGS := $(CFLAGS_BASE) -DNDEBUG=1 -Os -fomit-frame-pointer
	OUTDIR := release
endif

CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++20
ASFLAGS := $(CFLAGS)

LIBS := -lctru -lm -lcwav -lncsnd -lcurl -lcapstone -lctrpf
LIBDIRS := $(CTRULIB) $(CURDIR)/libcwav $(CURDIR)/libcwav/libncsnd $(CURDIR)/lib
LDFLAGS := $(foreach dir,$(LIBDIRS),-L$(dir)) $(LIBS)

CPPFILES := $(shell find source -type f -name "*.cpp")
OFILES := $(patsubst source/%.cpp,$(OUTDIR)/%.o,$(CPPFILES))
DEPENDS := $(OFILES:.o=.d)

INCLUDE := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
           $(foreach dir,$(LIBDIRS),-I$(dir)/include) \
           -I.

.PHONY: all clean rebuild

all: libCTRPluginFrameworkPlus/libCTRPF+.a
	@echo  libCTRPF+.a Ready !!!

rebuild: clean all

libCTRPluginFrameworkPlus/libCTRPF+.a: $(OFILES) | libCTRPluginFrameworkPlus
	@$(AR) rcs $@ $^

$(OUTDIR)/%.o: source/%.cpp
	@echo - $(notdir $<)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -MMD -MP -c $< -o $@

$(OUTDIR):
	mkdir -p $@

libCTRPluginFrameworkPlus:
	mkdir -p $@

clean:
	@echo cleaning...
	@rm -rf release debug libCTRPluginFrameworkPlus

-include $(DEPENDS)



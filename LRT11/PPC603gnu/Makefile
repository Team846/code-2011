# Wind River Workbench generated Makefile.
# Do not edit!!!
#
# The file ".wrmakefile" is the template used by the Wind River Workbench to
# generate the makefiles of this project. Add user-specific build targets and
# make rules only(!) in this project's ".wrmakefile" file. These will then be
# automatically dumped into the makefiles.

WIND_HOME := $(subst \,/,$(WIND_HOME))
WIND_BASE := $(subst \,/,$(WIND_BASE))
WIND_USR := $(subst \,/,$(WIND_USR))

all : pre_build main_all post_build

_clean ::
	@echo "make: removing targets and objects of `pwd`"

TRACE=0
TRACEON=$(TRACE:0=@)
TRACE_FLAG=$(TRACEON:1=)

MAKEFILE := Makefile

FLEXIBLE_BUILD := 1

BUILD_SPEC = PPC603gnu
DEBUG_MODE = 1
ifeq ($(DEBUG_MODE),1)
MODE_DIR := Debug
else
MODE_DIR := NonDebug
endif
OBJ_DIR := .
PRJ_ROOT_DIR := C:/Windriver/workspace/LRT11
WS_ROOT_DIR := C:/Windriver/workspace



#Global Build Macros
PROJECT_TYPE = DKM
DEFINES = 
EXPAND_DBG = 0


#BuildSpec specific Build Macros
VX_CPU_FAMILY = ppc
CPU = PPC603
TOOL_FAMILY = gnu
TOOL = gnu
TOOL_PATH = 
CC_ARCH_SPEC = -mcpu=603 -mstrict-align -mno-implicit-fp -mlongcall
LIBPATH = 
LIBS = 

IDE_INCLUDES = -I$(WIND_BASE)/target/h/WPILib -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/wrn/coreip 

IDE_LIBRARIES = $(WIND_BASE)/target/lib/WPILib.a 



#BuildTool flags
ifeq ($(DEBUG_MODE),1)
DEBUGFLAGS_C-Compiler = -g
DEBUGFLAGS_C++-Compiler = -g
DEBUGFLAGS_Linker = -g
DEBUGFLAGS_Partial-Image-Linker = 
DEBUGFLAGS_Librarian = 
DEBUGFLAGS_Assembler = -g
else
DEBUGFLAGS_C-Compiler = -O2 -fstrength-reduce -fno-builtin
DEBUGFLAGS_C++-Compiler = -O2 -fstrength-reduce -fno-builtin
DEBUGFLAGS_Linker = -O2 -fstrength-reduce -fno-builtin
DEBUGFLAGS_Partial-Image-Linker = 
DEBUGFLAGS_Librarian = 
DEBUGFLAGS_Assembler = -O2 -fstrength-reduce -fno-builtin
endif


#Project Targets
PROJECT_TARGETS = BuiltinDefaultCode/$(MODE_DIR)/BuiltinDefaultCode.out \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/BuiltinDefaultCode_partialImage.o


#Rules

# BuiltinDefaultCode
ifeq ($(DEBUG_MODE),1)
BuiltinDefaultCode/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g
BuiltinDefaultCode/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g
BuiltinDefaultCode/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g
BuiltinDefaultCode/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
BuiltinDefaultCode/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
BuiltinDefaultCode/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
else
BuiltinDefaultCode/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -O2 -fstrength-reduce -fno-builtin
BuiltinDefaultCode/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -O2 -fstrength-reduce -fno-builtin
BuiltinDefaultCode/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -O2 -fstrength-reduce -fno-builtin
BuiltinDefaultCode/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
BuiltinDefaultCode/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
BuiltinDefaultCode/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -O2 -fstrength-reduce -fno-builtin
endif
BuiltinDefaultCode/$(MODE_DIR)/% : IDE_INCLUDES = -I$(WIND_BASE)/target/h/WPILib -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/wrn/coreip 
BuiltinDefaultCode/$(MODE_DIR)/% : IDE_LIBRARIES = $(WIND_BASE)/target/lib/WPILib.a 
BuiltinDefaultCode/$(MODE_DIR)/% : PROJECT_TYPE = DKM
BuiltinDefaultCode/$(MODE_DIR)/% : DEFINES = 
BuiltinDefaultCode/$(MODE_DIR)/% : EXPAND_DBG = 0
BuiltinDefaultCode/$(MODE_DIR)/% : VX_CPU_FAMILY = ppc
BuiltinDefaultCode/$(MODE_DIR)/% : CPU = PPC603
BuiltinDefaultCode/$(MODE_DIR)/% : TOOL_FAMILY = gnu
BuiltinDefaultCode/$(MODE_DIR)/% : TOOL = gnu
BuiltinDefaultCode/$(MODE_DIR)/% : TOOL_PATH = 
BuiltinDefaultCode/$(MODE_DIR)/% : CC_ARCH_SPEC = -mcpu=603 -mstrict-align -mno-implicit-fp -mlongcall
BuiltinDefaultCode/$(MODE_DIR)/% : LIBPATH = 
BuiltinDefaultCode/$(MODE_DIR)/% : LIBS = 
BuiltinDefaultCode/$(MODE_DIR)/% : OBJ_DIR := BuiltinDefaultCode/$(MODE_DIR)

OBJECTS_BuiltinDefaultCode = BuiltinDefaultCode_partialImage/$(MODE_DIR)/BuiltinDefaultCode_partialImage.o

BuiltinDefaultCode/$(MODE_DIR)/BuiltinDefaultCode.out : $(OBJECTS_BuiltinDefaultCode)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@";rm -f "$@";nmppc $(OBJECTS_BuiltinDefaultCode) $(IDE_LIBRARIES) | tclsh $(WIND_BASE)/host/resource/hutils/tcl/munch.tcl -c ppc > $(OBJ_DIR)/ctdt.c; $(TOOL_PATH)ccppc $(DEBUGFLAGS_Linker) $(CC_ARCH_SPEC) -fdollars-in-identifiers -Wall $(ADDED_CFLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES)  -o $(OBJ_DIR)/ctdt.o -c $(OBJ_DIR)/ctdt.c; $(TOOL_PATH)ccppc -r -nostdlib -Wl,-X -T $(WIND_BASE)/target/h/tool/gnu/ldscripts/link.OUT -o "$@" $(OBJ_DIR)/ctdt.o $(OBJECTS_BuiltinDefaultCode) $(IDE_LIBRARIES) $(LIBPATH) $(LIBS)  $(ADDED_LIBPATH) $(ADDED_LIBS) && if [ "$(EXPAND_DBG)" = "1" ]; then plink "$@";fi

BuiltinDefaultCode/$(MODE_DIR)/BuiltinDefaultCode_compile_file : $(FILE) ;

_clean :: BuiltinDefaultCode/$(MODE_DIR)/BuiltinDefaultCode_clean

BuiltinDefaultCode/$(MODE_DIR)/BuiltinDefaultCode_clean : 
	$(TRACE_FLAG)if [ -d "BuiltinDefaultCode" ]; then cd "BuiltinDefaultCode"; rm -rf $(MODE_DIR); fi


# BuiltinDefaultCode_partialImage
ifeq ($(DEBUG_MODE),1)
BuiltinDefaultCode_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g
BuiltinDefaultCode_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g
BuiltinDefaultCode_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g
BuiltinDefaultCode_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
BuiltinDefaultCode_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
BuiltinDefaultCode_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
else
BuiltinDefaultCode_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -O2 -fstrength-reduce -fno-builtin
BuiltinDefaultCode_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -O2 -fstrength-reduce -fno-builtin
BuiltinDefaultCode_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -O2 -fstrength-reduce -fno-builtin
BuiltinDefaultCode_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
BuiltinDefaultCode_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
BuiltinDefaultCode_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -O2 -fstrength-reduce -fno-builtin
endif
BuiltinDefaultCode_partialImage/$(MODE_DIR)/% : IDE_INCLUDES = -I$(WIND_BASE)/target/h/WPILib -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/wrn/coreip 
BuiltinDefaultCode_partialImage/$(MODE_DIR)/% : IDE_LIBRARIES = $(WIND_BASE)/target/lib/WPILib.a 
BuiltinDefaultCode_partialImage/$(MODE_DIR)/% : PROJECT_TYPE = DKM
BuiltinDefaultCode_partialImage/$(MODE_DIR)/% : DEFINES = 
BuiltinDefaultCode_partialImage/$(MODE_DIR)/% : EXPAND_DBG = 0
BuiltinDefaultCode_partialImage/$(MODE_DIR)/% : VX_CPU_FAMILY = ppc
BuiltinDefaultCode_partialImage/$(MODE_DIR)/% : CPU = PPC603
BuiltinDefaultCode_partialImage/$(MODE_DIR)/% : TOOL_FAMILY = gnu
BuiltinDefaultCode_partialImage/$(MODE_DIR)/% : TOOL = gnu
BuiltinDefaultCode_partialImage/$(MODE_DIR)/% : TOOL_PATH = 
BuiltinDefaultCode_partialImage/$(MODE_DIR)/% : CC_ARCH_SPEC = -mcpu=603 -mstrict-align -mno-implicit-fp -mlongcall
BuiltinDefaultCode_partialImage/$(MODE_DIR)/% : LIBPATH = 
BuiltinDefaultCode_partialImage/$(MODE_DIR)/% : LIBS = 
BuiltinDefaultCode_partialImage/$(MODE_DIR)/% : OBJ_DIR := BuiltinDefaultCode_partialImage/$(MODE_DIR)

BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/ActionData.o : C:/Windriver/workspace/LRT11/ActionData.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/AutomatedRoutines.o : C:/Windriver/workspace/LRT11/Brain/AutomatedRoutines.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/Auton.o : C:/Windriver/workspace/LRT11/Brain/Auton.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/Brain.o : C:/Windriver/workspace/LRT11/Brain/Brain.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/Common.o : C:/Windriver/workspace/LRT11/Brain/Common.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/Disabled.o : C:/Windriver/workspace/LRT11/Brain/Disabled.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/Teleop.o : C:/Windriver/workspace/LRT11/Brain/Teleop.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/TeleopArm.o : C:/Windriver/workspace/LRT11/Brain/TeleopArm.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/TeleopDriveTrain.o : C:/Windriver/workspace/LRT11/Brain/TeleopDriveTrain.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/TeleopLift.o : C:/Windriver/workspace/LRT11/Brain/TeleopLift.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/TeleopMinibot.o : C:/Windriver/workspace/LRT11/Brain/TeleopMinibot.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/TeleopRoller.o : C:/Windriver/workspace/LRT11/Brain/TeleopRoller.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/TeleopShifter.o : C:/Windriver/workspace/LRT11/Brain/TeleopShifter.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/CAN/VirtualCANBusController.o : C:/Windriver/workspace/LRT11/CAN/VirtualCANBusController.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/Arm.o : C:/Windriver/workspace/LRT11/Components/Arm.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/ComponentFactory.o : C:/Windriver/workspace/LRT11/Components/ComponentFactory.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/ConfigLoader.o : C:/Windriver/workspace/LRT11/Components/ConfigLoader.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/DriveTrain/CLPositionDriveTrain.o : C:/Windriver/workspace/LRT11/Components/DriveTrain/CLPositionDriveTrain.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/DriveTrain/CLRateTrain.o : C:/Windriver/workspace/LRT11/Components/DriveTrain/CLRateTrain.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/DriveTrain/DitheredBrakeTrain.o : C:/Windriver/workspace/LRT11/Components/DriveTrain/DitheredBrakeTrain.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/Lift.o : C:/Windriver/workspace/LRT11/Components/Lift.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/MinibotDeployer.o : C:/Windriver/workspace/LRT11/Components/MinibotDeployer.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/ModifiedDriveTrain.o : C:/Windriver/workspace/LRT11/Components/ModifiedDriveTrain.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/Roller.o : C:/Windriver/workspace/LRT11/Components/Roller.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/Shifter/LRTServo.o : C:/Windriver/workspace/LRT11/Components/Shifter/LRTServo.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/Shifter/VirtualLRTServo.o : C:/Windriver/workspace/LRT11/Components/Shifter/VirtualLRTServo.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/Shifter.o : C:/Windriver/workspace/LRT11/Components/Shifter.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Config/Build.o : C:/Windriver/workspace/LRT11/Config/Build.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Config/Config.o : C:/Windriver/workspace/LRT11/Config/Config.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Config/Configurable.o : C:/Windriver/workspace/LRT11/Config/Configurable.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Config/DriverStationButtons.o : C:/Windriver/workspace/LRT11/Config/DriverStationButtons.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/DriverInputs/DebouncedJoystick.o : C:/Windriver/workspace/LRT11/DriverInputs/DebouncedJoystick.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/DriverInputs/ProcessedInputs.o : C:/Windriver/workspace/LRT11/DriverInputs/ProcessedInputs.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Jaguar/CANJaguarBrake.o : C:/Windriver/workspace/LRT11/Jaguar/CANJaguarBrake.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Jaguar/Esc.o : C:/Windriver/workspace/LRT11/Jaguar/Esc.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Jaguar/LRTCANJaguar.o : C:/Windriver/workspace/LRT11/Jaguar/LRTCANJaguar.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Jaguar/ProxiedCANJaguar.o : C:/Windriver/workspace/LRT11/Jaguar/ProxiedCANJaguar.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/LRTRobot11.o : C:/Windriver/workspace/LRT11/LRTRobot11.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/LRTRobotBase.o : C:/Windriver/workspace/LRT11/LRTRobotBase.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Sensors/DriveEncoders.o : C:/Windriver/workspace/LRT11/Sensors/DriveEncoders.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Sensors/LRTDigitalOutput.o : C:/Windriver/workspace/LRT11/Sensors/LRTDigitalOutput.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Sensors/LRTEncoder.o : C:/Windriver/workspace/LRT11/Sensors/LRTEncoder.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Sensors/LineSensor.o : C:/Windriver/workspace/LRT11/Sensors/LineSensor.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Sensors/VirtualLRTEncoder.o : C:/Windriver/workspace/LRT11/Sensors/VirtualLRTEncoder.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Sensors/VirtualPot.o : C:/Windriver/workspace/LRT11/Sensors/VirtualPot.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Util/AsynchronousFileLogger.o : C:/Windriver/workspace/LRT11/Util/AsynchronousFileLogger.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Util/AsynchronousPrinter.o : C:/Windriver/workspace/LRT11/Util/AsynchronousPrinter.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Util/Console.o : C:/Windriver/workspace/LRT11/Util/Console.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Util/DutyCycleSubscriber.o : C:/Windriver/workspace/LRT11/Util/DutyCycleSubscriber.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Util/LCD.o : C:/Windriver/workspace/LRT11/Util/LCD.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Util/PrintInConstructor.o : C:/Windriver/workspace/LRT11/Util/PrintInConstructor.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Util/Profiler.o : C:/Windriver/workspace/LRT11/Util/Profiler.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Util/Util.o : C:/Windriver/workspace/LRT11/Util/Util.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o "$@" -c "$<"


OBJECTS_BuiltinDefaultCode_partialImage = BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/ActionData.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/AutomatedRoutines.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/Auton.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/Brain.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/Common.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/Disabled.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/Teleop.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/TeleopArm.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/TeleopDriveTrain.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/TeleopLift.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/TeleopMinibot.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/TeleopRoller.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/TeleopShifter.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/CAN/VirtualCANBusController.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/Arm.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/ComponentFactory.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/ConfigLoader.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/DriveTrain/CLPositionDriveTrain.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/DriveTrain/CLRateTrain.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/DriveTrain/DitheredBrakeTrain.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/Lift.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/MinibotDeployer.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/ModifiedDriveTrain.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/Roller.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/Shifter/LRTServo.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/Shifter/VirtualLRTServo.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/Shifter.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Config/Build.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Config/Config.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Config/Configurable.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Config/DriverStationButtons.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/DriverInputs/DebouncedJoystick.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/DriverInputs/ProcessedInputs.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Jaguar/CANJaguarBrake.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Jaguar/Esc.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Jaguar/LRTCANJaguar.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Jaguar/ProxiedCANJaguar.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/LRTRobot11.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/LRTRobotBase.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Sensors/DriveEncoders.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Sensors/LRTDigitalOutput.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Sensors/LRTEncoder.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Sensors/LineSensor.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Sensors/VirtualLRTEncoder.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Sensors/VirtualPot.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Util/AsynchronousFileLogger.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Util/AsynchronousPrinter.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Util/Console.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Util/DutyCycleSubscriber.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Util/LCD.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Util/PrintInConstructor.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Util/Profiler.o \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Util/Util.o

BuiltinDefaultCode_partialImage/$(MODE_DIR)/BuiltinDefaultCode_partialImage.o : $(OBJECTS_BuiltinDefaultCode_partialImage)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccppc -r -nostdlib -Wl,-X  -o "$@" $(OBJECTS_BuiltinDefaultCode_partialImage) $(LIBPATH) $(LIBS) $(ADDED_LIBPATH) $(ADDED_LIBS)  && if [ "$(EXPAND_DBG)" = "1" ]; then plink "$@";fi

BuiltinDefaultCode_partialImage/$(MODE_DIR)/BuiltinDefaultCode_partialImage_compile_file : $(FILE) ;

_clean :: BuiltinDefaultCode_partialImage/$(MODE_DIR)/BuiltinDefaultCode_partialImage_clean

BuiltinDefaultCode_partialImage/$(MODE_DIR)/BuiltinDefaultCode_partialImage_clean : 
	$(TRACE_FLAG)if [ -d "BuiltinDefaultCode_partialImage" ]; then cd "BuiltinDefaultCode_partialImage"; rm -rf $(MODE_DIR); fi

DEP_FILES := BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/ActionData.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/AutomatedRoutines.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/Auton.d \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/Brain.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/Common.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/Disabled.d \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/Teleop.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/TeleopArm.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/TeleopDriveTrain.d \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/TeleopLift.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/TeleopMinibot.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/TeleopRoller.d \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Brain/TeleopShifter.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/CAN/VirtualCANBusController.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/Arm.d \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/ComponentFactory.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/ConfigLoader.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/DriveTrain/CLPositionDriveTrain.d \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/DriveTrain/CLRateTrain.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/DriveTrain/DitheredBrakeTrain.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/Lift.d \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/MinibotDeployer.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/ModifiedDriveTrain.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/Roller.d \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/Shifter/LRTServo.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/Shifter/VirtualLRTServo.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Components/Shifter.d \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Config/Build.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Config/Config.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Config/Configurable.d \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Config/DriverStationButtons.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/DriverInputs/DebouncedJoystick.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/DriverInputs/ProcessedInputs.d \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Jaguar/CANJaguarBrake.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Jaguar/Esc.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Jaguar/LRTCANJaguar.d \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Jaguar/ProxiedCANJaguar.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/LRTRobot11.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/LRTRobotBase.d \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Sensors/DriveEncoders.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Sensors/LRTDigitalOutput.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Sensors/LRTEncoder.d \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Sensors/LineSensor.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Sensors/VirtualLRTEncoder.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Sensors/VirtualPot.d \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Util/AsynchronousFileLogger.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Util/AsynchronousPrinter.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Util/Console.d \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Util/DutyCycleSubscriber.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Util/LCD.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Util/PrintInConstructor.d \
	 BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Util/Profiler.d BuiltinDefaultCode_partialImage/$(MODE_DIR)/Objects/LRT11/Util/Util.d
-include $(DEP_FILES)



-include $(PRJ_ROOT_DIR)/*.makefile

-include *.makefile

main_all : external_build $(PROJECT_TARGETS)
	@echo "make: built targets of `pwd`"

# entry point for extending the build
external_build ::
	@echo ""

# main entry point for pre processing prior to the build
pre_build :: $(PRE_BUILD_STEP) generate_sources
	@echo ""

# entry point for generating sources prior to the build
generate_sources ::
	@echo ""

# main entry point for post processing after the build
post_build :: $(POST_BUILD_STEP) deploy_output
	@echo ""

# entry point for deploying output after the build
deploy_output ::
	@echo ""

clean :: external_clean $(CLEAN_STEP) _clean

# entry point for extending the build clean
external_clean ::
	@echo ""

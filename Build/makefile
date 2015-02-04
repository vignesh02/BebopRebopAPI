################################################################################
# Automatically-generated file. Do not edit!
################################################################################

RM := rm -rf

LIBS :=
O_SRCS :=
CPP_SRCS :=
C_UPPER_SRCS :=
C_SRCS :=
S_UPPER_SRCS :=
OBJ_SRCS :=
ASM_SRCS :=
CXX_SRCS :=
C++_SRCS :=
CC_SRCS :=
OBJS :=
C++_DEPS :=
C_DEPS :=
CC_DEPS :=
CPP_DEPS :=
EXECUTABLES :=
CXX_DEPS :=
C_UPPER_DEPS := 


# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Source/CBebopInterface.cpp \
../Source/CCommandPacket.cpp \
../Source/CNetworkInterface.cpp \
../Source/CNetworkSettings.cpp \
../Source/CVehicleInterface.cpp 

OBJS += \
./CBebopInterface.o \
./CCommandPacket.o \
./CNetworkInterface.o \
./CNetworkSettings.o \
./CVehicleInterface.o 

CPP_DEPS += \
./CBebopInterface.d \
./CCommandPacket.d \
./CNetworkInterface.d \
./CNetworkSettings.d \
./CVehicleInterface.d 

# All Target
all: libBebopRebopAPI.a

# Tool invocations
libBebopRebopAPI.a: $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC Archiver'
	ar -r  "libBebopRebopAPI.a" $(OBJS)
	@echo 'Finished building target: $@'
	@echo 'Running post-build script to move files into their proper locations'
	-../Scripts/postbuild.sh
	
%.o: ../Source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../ThirdPartyLibs/ArDroneSDK3/include -I../ThirdPartyLibs/rapidjson/include -I../ThirdPartyLibs/easyloggingpp/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(ARCHIVES)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS) libBebopRebopAPI.a include/ lib/ ThirdPartyLibs/
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets

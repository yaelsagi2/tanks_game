# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Werror -pedantic -I../common -rdynamic -O0 -g

# Common sources live one level up
COMMON_SRCS := \
        Task.cpp \
        ConcurrentQueue.cpp \
        TankAlgorithmRegistration.cpp \
        PlayerRegistration.cpp \
        MapParser.cpp \
        MapDataSatelliteView.cpp \
        Loader.cpp \
        GameResultWriter.cpp \
        GameManagerRegistration.cpp \
        GameManagerRegistrar.cpp \
        AlgorithmRegistrar.cpp \
        ArgsParser.cpp \
        CharMatrix.cpp \
        Simulator.cpp \
        ../UserCommon/TimeStamp.cpp

# Target-specific
SIM_BIN := simulator_206480972_206899163

.PHONY: all clean

all: $(SIM_BIN)

$(SIM_BIN): $(COMMON_SRCS)
	$(CXX) $(CXXFLAGS) $(COMMON_SRCS) -o $@

clean:
	$(RM) $(SIM_BIN)


# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Werror -pedantic -fPIC -I../common -I../UserCommon -O0 -g
LDFLAGS := -shared

# Sources (use forward slashes; no trailing spaces after backslashes)
USERCOMMON_SRCS := \
    ../UserCommon/Direction.cpp \
    ../UserCommon/Mine.cpp \
    ../UserCommon/Point.cpp \
    ../UserCommon/Shell.cpp \
    ../UserCommon/Tank.cpp \
    ../UserCommon/Wall.cpp

ALG_SRCS := \
    YaelNogaPlayer.cpp \
    SimpleBattleInfo.cpp \
    algorithm.cpp \
    HybridTankAlgorithm.cpp

SRCS := $(USERCOMMON_SRCS) $(ALG_SRCS)

ALG_BIN := Algorithm_206480972_206899163.so

.PHONY: all clean

all: $(ALG_BIN)

$(ALG_BIN): $(SRCS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(SRCS)

clean:
	$(RM) $(ALG_BIN)


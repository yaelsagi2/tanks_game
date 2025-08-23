# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Werror -shared -fPIC -pedantic -I../common -I../common -I../UserCommon -I../GameManager -O0 -g

# Common sources live one level up
COMMON_SRCS := \
	../UserCommon/Direction.cpp \
	GameManager.cpp \
	Logger.cpp \
	MatrixRecorder.cpp \
	../UserCommon/Mine.cpp \
	../UserCommon/Point.cpp \
	../UserCommon/Shell.cpp \
	../UserCommon/Tank.cpp \
	../UserCommon/Wall.cpp \
	../UserCommon/TimeStamp.cpp \
	GameBoard.cpp \
	GameBoardSatelliteView.cpp \

# Target-specific
GM_SRC := game_manager.cpp
GM_BIN := GameManager_206480972_206899163.so

.PHONY: all clean

all: $(GM_BIN)

$(GM_BIN): $(COMMON_SRCS) $(GM_SRC)
	$(CXX) $(CXXFLAGS) $(COMMON_SRCS) $(GM_SRC) -o $@

clean:
	$(RM) $(GM_BIN)


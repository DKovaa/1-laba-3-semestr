CXX = g++
CXXFLAGS_BASE = -Wall -Wextra -std=c++17 -O2 -I./src

WX_CXXFLAGS = $(shell wx-config --cxxflags)
WX_LIBS = $(shell wx-config --libs)

TARGET_CONSOLE = console_app
TARGET_GUI = hashtable_gui

CONSOLE_SRC = tests/main_console.cpp tests/tests.cpp
GUI_SRC = gui/main.cpp

all: console

console: $(CONSOLE_SRC)
	$(CXX) $(CXXFLAGS_BASE) -o $(TARGET_CONSOLE) $(CONSOLE_SRC)

gui: $(GUI_SRC)
	$(CXX) $(CXXFLAGS_BASE) $(WX_CXXFLAGS) -o $(TARGET_GUI) $(GUI_SRC) $(WX_LIBS)

run-console: console
	./$(TARGET_CONSOLE)

run-gui: gui
	./$(TARGET_GUI)

clean:
	rm -f $(TARGET_CONSOLE) $(TARGET_GUI) *.o

.PHONY: all console gui run-console run-gui clean
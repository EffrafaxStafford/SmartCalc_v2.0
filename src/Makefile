CC=gcc
CPPFLAGS=-c -Wall -Wextra -Werror -std=c++17 $(GTESTFLAGS)
LDFLAGS=-lstdc++ -lgtest $(GTESTLDFLAGS) $(SANFLAGS)
GTESTFLAGS="-I/opt/homebrew/include"
GTESTLDFLAGS="-L/opt/homebrew/lib"
SANFLAGS=#-fsanitize=address -fsanitize=leak

# Используемые папки
SOURCE_DIR=sources
HEADER_DIR=headers
TSOURCE_DIR=tests
OBJ_DIR=build
INSTALL_DIR=bin

# Списки файлов
SOURCES:=$(SOURCE_DIR)/model.cpp
TSOURCES:=$(TSOURCE_DIR)/tests.cpp
OBJECTS=$(OBJ_DIR)/*.o

# Исполняемые файлы
EXECUTABLE:=main
TESTEXECUTABLE:=test

.PHONY: tests
 
all: tests

clean: uninstall
	@rm -rf $(OBJ_DIR) $(TESTEXECUTABLE) 

install: uninstall
	@mkdir $(INSTALL_DIR) \
	&& cd ./$(INSTALL_DIR) \
	&& cmake ../sources \
	&& cmake --build . \
	&& ./$(EXECUTABLE)

uninstall:
	@rm -rf $(INSTALL_DIR)

dvi:

dist:

tests: 
	@rm -rf $(OBJ_DIR) $(TESTEXECUTABLE) 
	@$(CC) $(CPPFLAGS) $(SOURCES) $(TSOURCES) 
	@mkdir $(OBJ_DIR) && mv *.o $(OBJ_DIR)
	@$(CC) $(LDFLAGS) $(OBJECTS) -o $(TESTEXECUTABLE)
	@./$(TESTEXECUTABLE)

rebuild: clean all

lintc:
	@clang-format -n *.cpp

linth:
	@clang-format -n *.h

leaks:
	@leaks -atExit -- ./$(INSTALL_DIR)/$(EXECUTABLE) | grep LEAK:


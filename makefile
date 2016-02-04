# To run this program from the command line, type:
#
#   make run
#
# The program will wait for input. Type whatever you want, then press CTRL-D to
# send the end-of-file (EOF) character to stop reading. The program will then
# try to partition the input and print the results.
#
# Alternatively, you can save your command to a file so you don't have to keep
# typing the same thing again and again while you work. Assuming the file is
# called 'input.txt', first run 'make' and then do:
#
#   ./partition_tokens < input.txt
#

NAME = partition_tokens

all: $(NAME)

$(NAME): main.cpp command.o
	$(CXX) -Wall $^ -o $@

run: $(NAME)
	./$(NAME)

test: command.test
	./command.test

clean:
	rm -f $(NAME) command.test gtest_main.a *.o


# Points to the root of Google Test, relative to where this file is.
GTEST_DIR = googletest/googletest

# Flags passed to the preprocessor.
CPPFLAGS += -isystem $(GTEST_DIR)/include

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread

# All Google Test headers.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# Init the googletest submodule if needed.
$(GTEST_DIR):
	git submodule update --init

$(GTEST_HEADERS): | $(GTEST_DIR)

$(GTEST_SRCS_): | $(GTEST_DIR)

# Builds gtest_main.a.
gtest-all.o: $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o: $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest_main.a: gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# Builds the tests.
command.o: command.cpp command.h
	$(CXX) -Wall -c command.cpp

command_test.o: command_test.cpp command.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c command_test.cpp

command.test: command.o command_test.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# To build the program (which is called golf-sim by default), simply type:
#   make
#
# To clean up and remove the compiled binary and other generated files, type:
#   make clean
#
# To build AND run the program, type:
#   make run
#

# The name of your binary.
NAME = golf-sim

# Flags passed to the preprocessor.
CPPFLAGS += -Wall -MMD -MP -Isrc -g -std=c++11

# ALL .cpp files.
SRCS = $(shell find src -name '*.cpp')
OBJS = $(SRCS:src/%.cpp=bin/%.o)
DEPS = $(SRCS:src/%.cpp=bin/%.d)

# Libraries needed by the binary.
ifeq ($(shell uname),Darwin)
	LIBS = -lboost_thread-mt -lboost_system
else
	LIBS = -lboost_thread -lboost_system
endif

# Default target, which builds your binary.
$(NAME): $(OBJS)
	$(CXX) $(CPP_FLAGS) $^ -o $(NAME) $(LIBS)

# Build and run the program.
run: $(NAME)
	./$(NAME) input_files/easy

# Remove all generated files.
clean:
	rm -rf $(NAME)* bin/

# Ensure the bin/ directories are created.
$(SRCS): | bin

# Mirror the directory structure of src/ under bin/
bin:
	mkdir -p $(shell find src -type d | sed "s/src/bin/")

# Build objects.
bin/%.o: src/%.cpp
	$(CXX) $(CPPFLAGS) $< -c -o $@

# Auto dependency management.
-include $(DEPS)

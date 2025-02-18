NAME = webserv

SRC-PATH = src

HEADER-PATH = include

SRCS = $(SRC-PATH)/main.cpp\
$(SRC-PATH)/Server.cpp\
$(SRC-PATH)/Logger.cpp\
$(SRC-PATH)/Request.cpp\
$(SRC-PATH)/AuxFunc.cpp\

OBJS = $(SRCS:.cpp=.o)

HEADER = $(HEADER-PATH)/main.hpp\
$(HEADER-PATH)/Server.hpp\
$(HEADER-PATH)/Logger.hpp\
$(HEADER-PATH)/Request.hpp\
$(HEADER-PATH)/AuxFunc.hpp\


# TO DO: Remember to change the order of compilers to c++ clang++ g++ when evaluating project
CXX = $(shell which g++ c++ clang++  | head -n 1)

CXXFLAGS = -Wall -Werror -Wextra -std=c++98

RM = rm -rf

all: $(NAME)

$(NAME): $(OBJS) $(HEADER)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)


re: fclean
	$(MAKE) all

debug: CXXFLAGS += -g
debug: $(NAME)
NAME = webserv

SRC-PATH = src

HEADER-PATH = include

SRCS = $(SRC-PATH)/main.cpp\
$(SRC-PATH)/Server.cpp\
$(SRC-PATH)/Logger.cpp\
$(SRC-PATH)/Request.cpp\

OBJS = $(SRCS:.cpp=.o)

HEADER = $(HEADER-PATH)/main.hpp\
$(HEADER-PATH)/Server.hpp\
$(HEADER-PATH)/Logger.hpp\
$(HEADER-PATH)/Request.hpp\

CXX = $(shell which c++ clang++ g++ | head -n 1)

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
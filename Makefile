NAME = webserv

SRC-PATH = src

HEADER-PATH = include

SRCS = $(SRC-PATH)/main.cpp\
$(SRC-PATH)/Server.cpp\
$(SRC-PATH)/Logger.cpp\
$(SRC-PATH)/Request.cpp\
$(SRC-PATH)/ConfigParser.cpp\
$(SRC-PATH)/ConfigServer.cpp\
$(SRC-PATH)/Location.cpp\
$(SRC-PATH)/ConfigFile.cpp\
$(SRC-PATH)/AuxFunc.cpp\
$(SRC-PATH)/ResourceReq.cpp\
$(SRC-PATH)/PostUpload.cpp\
$(SRC-PATH)/HttpResponse.cpp\
$(SRC-PATH)/ConcreteBuilder.cpp\
$(SRC-PATH)/Director.cpp\
$(SRC-PATH)/Cluster.cpp\

OBJS = $(SRCS:.cpp=.o)

HEADER = $(HEADER-PATH)/main.hpp\
$(HEADER-PATH)/Server.hpp\
$(HEADER-PATH)/Logger.hpp\
$(HEADER-PATH)/Request.hpp\
$(HEADER-PATH)/ConfigParser.hpp\
$(HEADER-PATH)/ConfigServer.hpp\
$(HEADER-PATH)/Location.hpp\
$(HEADER-PATH)/ConfigFile.hpp\
$(HEADER-PATH)/AuxFunc.hpp\
$(HEADER-PATH)/ResourceReq.hpp\
$(HEADER-PATH)/PostUpload.hpp\
$(HEADER-PATH)/HttpResponse.hpp\
$(HEADER-PATH)/Builder.hpp\
$(HEADER-PATH)/ConcreteBuilder.hpp\
$(HEADER-PATH)/Director.hpp\
$(HEADER-PATH)/Cluster.hpp\


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

val:
	valgrind --track-fds=yes --leak-check=full ./$(NAME)
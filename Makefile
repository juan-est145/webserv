NAME = webserv

SRC-PATH = src

HEADER-PATH = include

SRCS = $(SRC-PATH)/main.cpp\
$(SRC-PATH)/ConfigParser.cpp\
$(SRC-PATH)/ConfigServer.cpp\
$(SRC-PATH)/AServerAction.cpp\
$(SRC-PATH)/PostUpload.cpp\
$(SRC-PATH)/ResourceReq.cpp\
$(SRC-PATH)/Delete.cpp\
$(SRC-PATH)/ARequest.cpp\
$(SRC-PATH)/Request.cpp\
$(SRC-PATH)/Cluster.cpp\
$(SRC-PATH)/Logger.cpp\
$(SRC-PATH)/Location.cpp\
$(SRC-PATH)/ConfigFile.cpp\
$(SRC-PATH)/AuxFunc.cpp\
$(SRC-PATH)/HttpResponse.cpp\
$(SRC-PATH)/ConcreteBuilder.cpp\
$(SRC-PATH)/Director.cpp\
$(SRC-PATH)/IServer.cpp\
$(SRC-PATH)/Server.cpp\
$(SRC-PATH)/Cgi.cpp\
$(SRC-PATH)/Cookie.cpp\
$(SRC-PATH)/CgiReq.cpp\
$(SRC-PATH)/ServerActionGenerator.cpp\


OBJS = $(SRCS:.cpp=.o)

HEADER = $(HEADER-PATH)/main.hpp\
$(HEADER-PATH)/ConfigParser.hpp\
$(HEADER-PATH)/ConfigServer.hpp\
$(HEADER-PATH)/AServerAction.hpp\
$(HEADER-PATH)/PostUpload.hpp\
$(HEADER-PATH)/ResourceReq.hpp\
$(HEADER-PATH)/Delete.hpp\
$(HEADER-PATH)/IBuilder.hpp\
$(HEADER-PATH)/ARequest.hpp\
$(HEADER-PATH)/Request.hpp\
$(HEADER-PATH)/ICluster.hpp\
$(HEADER-PATH)/Cluster.hpp\
$(HEADER-PATH)/Logger.hpp\
$(HEADER-PATH)/Location.hpp\
$(HEADER-PATH)/ConfigFile.hpp\
$(HEADER-PATH)/AuxFunc.hpp\
$(HEADER-PATH)/HttpResponse.hpp\
$(HEADER-PATH)/ConcreteBuilder.hpp\
$(HEADER-PATH)/Director.hpp\
$(HEADER-PATH)/IServer.hpp\
$(HEADER-PATH)/Server.hpp\
$(HEADER-PATH)/Cgi.hpp\
$(HEADER-PATH)/FirstHeader.hpp\
$(HEADER-PATH)/Cookie.hpp\
$(HEADER-PATH)/CgiReq.hpp\
$(HEADER-PATH)/RequestData.hpp\
$(HEADER-PATH)/ServerActionGenerator.hpp\

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

val:
	valgrind --track-fds=yes --leak-check=full ./$(NAME)
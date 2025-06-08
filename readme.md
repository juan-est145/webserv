# Webserv

A repository containing the webserv project of 42 Malaga. The aim of this project is to create from scratch a fully-functioning http server inspired by nginx. It support's GET, POST and DELETE methods, can process cgi scripts written with Python or shell scripts and it's also able to upload files. The server is also non-blocking thanks to the use of epoll. You can find all the information about the project in the subject pdf. This shell is compatible will require a Linux kernel to work. 
 
## Authors

- [@juan-est145](https://www.github.com/juan-est145)
- [@MiguelAngel456](https://www.github.com/MiguelAngel456)

## Dependencies
In order to be able to compile this project, you will need a C++ compiler (clang++ or g++ are recommended), the Make tool and be using a Linux kernel version greater than 2.5.44. It's also highly recommend to have python3 installed in your system.

To install all of these dependencies in Debian-based distros, you can use the following command
```bash
sudo apt update && apt install python3 clang++ g++ make
```
## Run Locally

Clone the project

```bash
  git clone https://github.com/juan-est145/webserv.git
```

Go to the project directory

```bash
  cd webserv
```

Compile the project
```bash
  make
```

Run the project
```bash
  ./webserv
```

Optionally, you can pass your own configuration file as the first argument to the program. If you do not provide an argument, the program will use the default.conf file located in config

To delete the .o files and the executable
```bash
  make fclean
```
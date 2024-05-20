#include <string>
#include <iostream>

#include "fpkg.hpp"
#include "fs.hpp"

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"
#define ORANGE "\033[1;38;2;255;165;0m"
#define WHITE "\033[1;37m"
#define RESET "\033[0m"

namespace fpkg {
  void debug(std::string s) {
#ifdef FDEBUG
    std::cerr << ORANGE << "debug" << RESET << ": " << s << '\n';
#endif
  }
  
  void log(std::string s) {
    std::cout << '[' << MAGENTA << "fpkg" << RESET << "]: " << s << '\n';
  }
  
  void warn(std::string s) {
    std::cout << '[' << MAGENTA << "fpkg" << RESET << "] " << ORANGE << "warning: " << RESET << s << '\n';
  }

  void error(std::string s) {
    std::cout << '[' << MAGENTA << "fpkg" << RESET << "] " << RED << "warning: " << RESET << s << '\n';
  }
}

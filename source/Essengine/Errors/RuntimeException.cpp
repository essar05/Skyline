#ifndef CRUNTIMEEXCEPTION_H
#define CRUNTIMEEXCEPTION_H

#include <iostream>
#include <string>
#include <exception>

namespace Essengine {
  class ERuntimeException: public std::exception {

    private:
      const char* ErrorMessage;

    public:

      ERuntimeException(std::string anErrorMessage) throw() {
        std::cout << "Runtime Exception: " << anErrorMessage << std::endl;
        this->ErrorMessage = anErrorMessage.c_str();
      }

      ~ERuntimeException() throw() {

      };

      virtual const char* what() const throw() {
        return this->ErrorMessage;
      }
  };
}

#endif

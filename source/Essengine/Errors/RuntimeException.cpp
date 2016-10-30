#ifndef CRUNTIMEEXCEPTION_H
#define CRUNTIMEEXCEPTION_H

#include <iostream>
#include <string>
#include <exception>

namespace Essengine {
  class ERuntimeException: public std::exception {

    private:
      const char* ErrorMessage;
      std::string message;

    public:

      ERuntimeException(std::string anErrorMessage) throw() {
        this->message = anErrorMessage;
        this->ErrorMessage = anErrorMessage.c_str();
      }

      ~ERuntimeException() throw() {

      };

      virtual const char* what() const throw() {
        return this->ErrorMessage;
      }

      std::string getMessage() {
        return this->message;
      }

  };
}

#endif

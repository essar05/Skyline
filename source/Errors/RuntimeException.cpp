#ifndef CGAME_H
#define CGAME_H

#include <stdio.h>
#include <exception>
using namespace std;

namespace Engine {
  class ERuntimeException: public exception {

    private:
      const char* ErrorMessage;

    public:

      ERuntimeException(const char* anErrorMessage) throw() {
        this->ErrorMessage = anErrorMessage;
      }

      ~ERuntimeException() throw() {
        delete this->ErrorMessage;
      };

      virtual const char* what() const throw() {
        char* error = new char[100];
        sprintf(error, "Runtime Exception: %s\n", this->ErrorMessage);
        return error;
      }
  };
}

#endif

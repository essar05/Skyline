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
        char* theFormat = "Runtime Exception: %s\n";
        char* theError = new char[strlen(theFormat) + strlen(anErrorMessage)];
        _snprintf_s(theError, strlen(theError), 200, theFormat, anErrorMessage);
        this->ErrorMessage = theError;
      }

      ~ERuntimeException() throw() {

      };

      virtual const char* what() const throw() {
        return this->ErrorMessage;
      }
  };
}

#endif

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
        char* theError = new char[200];
		sprintf_s(theError, sizeof(theError) + 23, "Runtime Exception: %s\n", anErrorMessage);
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

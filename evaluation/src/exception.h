#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

class parseException : public exception
{
  public:
    parseException(const std::string &msg);
    std::string what() const;

    virtual ~parseException();

  private:
    std::string _msg;
};

#endif /* #ifndef EXCEPTION_H */

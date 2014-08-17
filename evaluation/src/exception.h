#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>
#include <exception>

class parseException : public std::exception
{
  public:
    parseException(const std::string &msg);
    std::string what();

    virtual ~parseException() noexcept;

  private:
    std::string _msg;
};

#endif /* #ifndef EXCEPTION_H */

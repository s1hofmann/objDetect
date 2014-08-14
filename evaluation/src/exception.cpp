#include "exception.h"
#include <string>

// 2014-08-14 Simon Hofmann <mail@simon-hofmann.org>

parseException::parseException(const std::string &msg) : _msg(msg)
{
}

std::string parseException::what()
{
    return _msg;
}

parseException::~parseException() throw()
{
  // cleanup
}

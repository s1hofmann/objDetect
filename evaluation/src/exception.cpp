#include "exception.h"

// 2014-08-14 Simon Hofmann <mail@simon-hofmann.org>

parseException::parseException(const std::string &msg) : _msg(msg)
{
}

std::string parseException::what() const
{
    return _msg;
}

parseException::~parseException()
{
  // cleanup
}


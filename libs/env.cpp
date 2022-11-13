#include "env.hpp"

ENV::ENV()
{
}

const char *ENV::operator[](const char *str)
{
    return getenv(str);
}

int ENV::getErrCode()
{
    return errno;
}

int ENV::put(char *str)
{
    return putenv(str);
}
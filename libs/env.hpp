#ifndef _ENV_H_
#define _ENV_H_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

class ENV
{
private:
protected:
public:
    ENV();

    const char *operator[](const char *str);
    int getErrCode();

    // char *str = "KEY=VALUE"
    int put(char *str);
};

#endif
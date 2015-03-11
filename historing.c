/*! \file ********************************************************************
 *
 *  \brief      BRIEF DESCRIPTION
 *  \author     Dan Fekete <thefekete@gmail.com>
 *  \date       March 10, 2015
 *
 *  \copyright  Copyright 2015, Dan Fekete <thefekete@gmail.com>,
 *              GNU GPL version 3
 *
 *  FULL DESCRIPTION
 *
 *****************************************************************************/
#include "historing.h"
//#include <stdlib.h>


char* _itoa(uint32_t val, int base)
{
    if (!val) {
        return "0";
    } else {
        static char buf[34] = {0};
        int i = 32;
        for(; val && i ; --i, val /= base)
            buf[i] = "0123456789abcdef"[val % base];
        return &buf[i + 1];
    }
}

void _puts(const char *s, putchar_cb p)
{
    while(*s) {
        p(*s++);
    }
}

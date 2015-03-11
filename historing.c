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
#include <assert.h>
#include <stdlib.h>

struct HistItem {
    uint32_t timestamp;
    const char *file;
    const char *function;
    const char *message;
    uint32_t data;
} HistItem;

static struct HistItem *buf;
static int buf_len;
static timestamp_cb _time;


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


int HistoRing_init(int len, timestamp_cb t_fn)
{
    assert(buf == NULL);  // don't double init

    /* sanity check on buf size */
    if (len < 1) {
        return 1;
    }

    /* allocate history buffer */
    buf = calloc(len, sizeof(buf));
    if (!buf) {
        return 1;
    }

    buf_len = len;
    _time = t_fn;

    return 0;  // everything's ok ;)
}

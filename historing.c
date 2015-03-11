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
    const char *function;
    const char *message;
    uint32_t data;
} HistItem;

static struct HistItem *buf;
static int buf_len;
static int buf_cnt;
static int buf_head;
static int buf_tail;
static timestamp_cb _time;
static uint32_t serial;


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

void _puts_P(const char *s, putchar_cb p)
{
    while(pgm_read_byte(s)) {
        p(pgm_read_byte(s++));
    }
}

void _teardown(void)
{
    free(buf);
    buf = NULL;
    buf_len = 0;
    buf_cnt = 0;
    buf_head = 0;
    buf_tail = 0;
    _time = NULL;
    serial = 0;
}


int HistoRing_init(int len, timestamp_cb t_fn)
{
    /* sanity check on buf size */
    if (len < 1)
        return 1;

    /* if already initialized, destroy before re-init */
    if (buf)
        _teardown();

    /* allocate history buffer */
    buf = calloc(len, sizeof(buf));
    if (!buf) {
        return 1;
    }

    buf_len = len;
    _time = t_fn;
    HistoRing_add_full(__func__, "history initialized", HIST_NOVAL);

    return 0;
}

void HistoRing_add_full(
        const char * function, const char * message, uint32_t data)
{
    /* if no buffer, do nothing */
    if (!buf)
        return;

    if (buf_cnt == buf_len) {
        /* buffer is full, drop oldest item */
        /* increment tail and decrement count */
        buf_tail++;
        buf_tail %= buf_len;
        buf_cnt--;
    }

    /* alias buffer slot and increment head */
    struct HistItem *p = &buf[buf_head++];
    buf_head %= buf_len;
    buf_cnt++;

    /* fill in struct */
    p->timestamp = (_time) ? _time() : serial++;   // serial num if no callback
    p->function = function;
    p->message = message;
    p->data = data;
}

/** \brief Prints history using _putchar, clears it if NULL */
int HistoRing_print(putchar_cb _putchar)
{
    int items = 0;
    while (buf_cnt) {
        /* alias tail */
        struct HistItem *p = &buf[buf_tail];

        if (_putchar) {
            /* we have a putchar callback, print to it */
            _puts(_itoa(p->timestamp, 10), _putchar);
            _putchar(' ');
            _puts(p->function, _putchar);
            _puts("() ", _putchar);
            _puts_P(p->message, _putchar);
            if (p->data != HIST_NOVAL) {
                _puts(" 0x", _putchar);
                _puts(_itoa(p->data, 16), _putchar);
            }
            _putchar('\n');
        }

        /* increment tail and decrement count */
        buf_tail++;
        buf_tail %= buf_len;
        buf_cnt--;

        items++;
    }

    return items;
}

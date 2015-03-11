/*! \file ********************************************************************
 *
 *  \brief      Testing app for Historing
 *  \author     Dan Fekete <thefekete@gmail.com>
 *  \date       March 10, 2015
 *
 *  \copyright  Copyright 2015, Dan Fekete <thefekete@gmail.com>,
 *              GNU GPL version 3
 *
 *  Tests functionality of CharRing module
 *
 *****************************************************************************/
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include "../historing.h"

#define BUF_LEN 1024

/*
 * historing.c private function declarations for testing
 */

char* _itoa(uint32_t, int);
void _puts(const char *, putchar_cb);
void _puts_P(const char *, putchar_cb);
void _teardown(void);


/*
 * TEST FIXTURE AND LOCAL FUNCTIONS
 */

typedef struct {
    int init_result;
} Fixture;


uint32_t my_timestamp(void)
{
    return 123456789;
}

void my_putchar(char c)
{
    putchar(c);
}

void setup(Fixture *f, gconstpointer test_data)
{
    f->init_result = HistoRing_init(BUF_LEN, NULL);
}

void teardown(Fixture *f, gconstpointer test_data)
{
    _teardown();
}


/*
 * UNIT TESTS
 */

void test_itoa_zero(void)
{
    g_assert_cmpstr(_itoa(0,2), ==, "0");
    g_assert_cmpstr(_itoa(0,8), ==, "0");
    g_assert_cmpstr(_itoa(0,10), ==, "0");
    g_assert_cmpstr(_itoa(0,16), ==, "0");
}

void test_itoa_bin(void)
{
    g_assert_cmpstr(_itoa(1,2), ==, "1");
    g_assert_cmpstr(_itoa(2,2), ==, "10");
    g_assert_cmpstr(_itoa(3,2), ==, "11");
    g_assert_cmpstr(_itoa(123456789,2), ==,
            "111010110111100110100010101");
    g_assert_cmpstr(_itoa((uint32_t) -1,2), ==,
            "11111111111111111111111111111111");
}

void test_itoa_hex_dec(void)
{
    char buf[33];
    uint32_t nums[] = {0, 1, 2, 3, 42, 123456789, (uint32_t) -1};
    for (int i=0; i < sizeof(nums)/sizeof(*nums); i++) {
        sprintf(buf, "%u", nums[i]);
        g_assert_cmpstr(_itoa(nums[i],10), ==, buf);
        sprintf(buf, "%x", nums[i]);
        g_assert_cmpstr(_itoa(nums[i],16), ==, buf);
    }
}

void test_puts_const(void)
{
    if (g_test_trap_fork(0, G_TEST_TRAP_SILENCE_STDOUT)) {
        _puts("Hello World!", my_putchar);
        exit(0);
    }
    // rerun this test in a subprocess
    g_test_trap_assert_passed();
    g_test_trap_assert_stdout("Hello World!");
}

void test_puts_ptr(void)
{
    char buf[] = "Hallo Welte!";
    if (g_test_trap_fork(0, G_TEST_TRAP_SILENCE_STDOUT)) {
        _puts(buf, my_putchar);
        exit(0);
    }
    // rerun this test in a subprocess
    g_test_trap_assert_passed();
    g_test_trap_assert_stdout(buf);
}

void test_puts_P(void)
{
    if (g_test_trap_fork(0, G_TEST_TRAP_SILENCE_STDOUT)) {
        _puts_P(PSTR("Hello World!"), my_putchar);
        exit(0);
    }
    // rerun this test in a subprocess
    g_test_trap_assert_passed();
    g_test_trap_assert_stdout("Hello World!");
}

void test_itoa_puts(void)
{
    if (g_test_trap_fork(0, G_TEST_TRAP_SILENCE_STDOUT)) {
        _puts(_itoa(123456789, 10), my_putchar);
        exit(0);
    }
    // rerun this test in a subprocess
    g_test_trap_assert_passed();
    g_test_trap_assert_stdout("123456789");
}

void test_init(Fixture *f, gconstpointer ignored)
{
    g_assert(f->init_result == 0);
}

void test_init_fails(void)
{
    if (g_test_trap_fork(0, G_TEST_TRAP_SILENCE_STDERR)) {
        int result = HistoRing_init(0, NULL);
        exit(result);
    }
    // rerun this test in a subprocess
    g_test_trap_assert_failed();
}

void test_print(Fixture *f, gconstpointer ignored)
{
    if (g_test_trap_fork(0, G_TEST_TRAP_SILENCE_STDOUT)) {
        int messages = HistoRing_print(my_putchar);
        g_assert_cmpint(messages, ==, 1);  // the init message
        messages = HistoRing_print(my_putchar);
        g_assert_cmpint(messages, ==, 0);  // no messages
        exit(0);
    }
    // rerun this test in a subprocess
    g_test_trap_assert_passed();
    g_test_trap_assert_stdout("*\n");  // something printed
}

void test_add_full(Fixture *f, gconstpointer ignored)
{
    if (g_test_trap_fork(0, G_TEST_TRAP_SILENCE_STDOUT)) {
        HistoRing_add_full(__func__, "first message", HIST_NOVAL);
        HistoRing_print(my_putchar);
        exit(0);
    }
    // rerun this test in a subprocess
    g_test_trap_assert_passed();
    g_test_trap_assert_stdout("0*\n"  // the init message
                              "1*test_add_full()*first message\n");
}

void test_add_full_data(Fixture *f, gconstpointer ignored)
{
    if (g_test_trap_fork(0, G_TEST_TRAP_SILENCE_STDOUT)) {
        HistoRing_add_full(__func__, "data message", 0xabcd);
        HistoRing_print(my_putchar);
        exit(0);
    }
    // rerun this test in a subprocess
    g_test_trap_assert_passed();
    g_test_trap_assert_stdout("0*\n"  // the init message
                              "1*data message*0xabcd\n");
}

void test_add_macros(Fixture *f, gconstpointer ignored)
{
    if (g_test_trap_fork(0, G_TEST_TRAP_SILENCE_STDOUT)) {
        HIST("message");
        HISTX("data message", 0x12345678);
        HIST("another message");
        HISTX("another data message", 0x7f);
        HistoRing_print(my_putchar);
        exit(0);
    }
    // rerun this test in a subprocess
    g_test_trap_assert_passed();
    g_test_trap_assert_stdout("0*\n"  // the init message
                              "1*message\n"
                              "2*data message*0x12345678\n"
                              "3*another message\n"
                              "4*data message*0x7f\n");
}


/*
 * TEST MAIN
 */
int main(int argc, char** argv)
{
    g_test_init(&argc, &argv, NULL);

    /* _itoa tests */
    g_test_add_func("/_itoa/zero", test_itoa_zero);
    g_test_add_func("/_itoa/binary", test_itoa_bin);
    g_test_add_func("/_itoa/hex_dec", test_itoa_hex_dec);

    /* _puts tests */
    g_test_add_func("/_puts/const", test_puts_const);
    g_test_add_func("/_puts/ptr", test_puts_ptr);
    g_test_add_func("/_puts/itoa_puts", test_itoa_puts);
    g_test_add_func("/_puts/PSTR", test_puts_P);

    /* main tests */
    g_test_add("/HistoRing/init",
            Fixture, NULL, setup, test_init, teardown);;
    g_test_add_func("/HistoRing/init_fails", test_init_fails);
    g_test_add("/HistoRing/print",
            Fixture, NULL, setup, test_print, teardown);
    g_test_add("/HistoRing/add_full",
            Fixture, NULL, setup, test_add_full, teardown);
    g_test_add("/HistoRing/add_full_data",
            Fixture, NULL, setup, test_add_full_data, teardown);
    g_test_add("/HistoRing/add_macros",
            Fixture, NULL, setup, test_add_macros, teardown);

    return g_test_run();
}

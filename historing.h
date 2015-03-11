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
#ifndef _HISTORING_H_
#define _HISTORING_H_

#include <stdint.h>

typedef uint32_t (*timestamp_cb)(void);
typedef void (*putchar_cb)(char);

int HistoRing_init(int, timestamp_cb);

#endif /* _HISTORING_H_ */

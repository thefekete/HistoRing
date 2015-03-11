/** \file
 *  dummy avr/pgmspace header for unit testing
 */
#ifndef _PGMSPACE_H_
#define _PGMSPACE_H_

#define PSTR(S)             (S)
#define pgm_read_byte(p)    *(p)

#endif /* _PGMSPACE_H_ */

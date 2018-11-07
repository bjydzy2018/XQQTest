/*
 * VAKey.h
 *
 *  Created on: 2013-7-3
 *      Author: pffang
 */

#ifndef VAKEY_H_
#define VAKEY_H_
#if defined (__linux)
#include <termios.h>

namespace VA {
extern struct termios g_stored_settings;
void SetAnyKeyPress(void);
void ResetAnyKeyPress(void);
}
#endif

#endif /* VAKEY_H_ */

/*
 * VAKey.cpp
 *
 *  Created on: 2013-7-3
 *      Author: pffang
 */
#include "VAKey.h"

#if defined (__linux)
namespace VA {

struct termios g_VA_stored_termios_settings;
void SetAnyKeyPress(void) {
  struct termios new_settings;
  tcgetattr(0, &g_VA_stored_termios_settings);
  new_settings = g_VA_stored_termios_settings;
  /* Disable canonical mode, and set buffer size to 1 byte */
  new_settings.c_lflag &= (~ICANON);
  new_settings.c_cc[VTIME] = 0;
  new_settings.c_cc[VMIN] = 1;
  tcsetattr(0, TCSANOW, &new_settings);
  return;
}
void ResetAnyKeyPress(void) {
  tcsetattr(0, TCSANOW, &g_VA_stored_termios_settings);
  return;
}

}
#endif
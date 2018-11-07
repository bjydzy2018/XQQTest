/*
 * CmdTextProgressBar.cpp
 *
 *  Created on: 2013年9月24日
 *      Author: pffang
 */

#include "CmdTextProgressBar.h"
#if VA_GNUC && !VA_ANDROID
#include <curses.h>
#include <sys/ioctl.h>
namespace VA {

using namespace std;

void CmdTextProgressBar(int pct) {     // pct -> Percentage
    string bar;
    struct winsize uk;
    if (ioctl(0, TIOCGWINSZ, &uk) != 0) {
        exit(1);
    }
    //int wdt = uk.ws_col - 30;  // create some space for the other text
    int wdt = uk.ws_col - 10;    // create some space for the other text
    if (wdt < 5) {
        wdt = 5;      // minimum width for progress bar 5 char
    }

    for (int i = 0; i < wdt; i++) {
        if (i < (pct * wdt / 100)) {
            bar.replace(i, 1, "=");
        } else if (i == (pct * wdt / 100)) {
            bar.replace(i, 1, "|");
        } else {
            bar.replace(i, 1, " ");
        }
    }
    cout << "\r";          // go to the first character in terminal
    cout.width(3);          // set some space for percentage
    cout << pct;          //<< "%";
    cout << "[\e[1;32m" << bar << "\e[0m] ";     // progress bar

    /* here is some sample for text after progress bar
     * this is sample for displaying terminal width.
     * you can replace it with whatever you want
     */
    //cout << "width ";
    //cout.width(3);
    //cout << uk.ws_col << " char" << flush;
    cout << flush;
}

} /* namespace VA */

#endif

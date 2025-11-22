/* hal_buttons.c
 * Simulated button input using keyboard (for PC testing)
 * Press 'm' → toggle mode
 * Press 't' → manual watering (only in MANUAL mode)
 */

#include "hal_buttons.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

static int getch_noblock(void)
{
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

/* -------------------------------------------------------------------------- */
uint8_t HAL_IsModeButtonPressed(void)
{
    int c = getch_noblock();
    if (c == 'm' || c == 'M') {
        printf("\n>>> MODE BUTTON PRESSED <<<\n");
        return 1;
    }
    return 0;
}

/* -------------------------------------------------------------------------- */
uint8_t HAL_IsManualButtonPressed(void)
{
    static uint8_t lastState = 0;
    int c = getch_noblock();

    if ((c == 't' || c == 'T') && !lastState) {
        lastState = 1;
        printf("\n>>> MANUAL WATERING BUTTON PRESSED <<<\n");
        return 1;
    }
    if (c == EOF || (c != 't' && c != 'T')) {
        lastState = 0;
    }
    return 0;
}
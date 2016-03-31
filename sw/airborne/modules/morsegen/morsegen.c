/*
 * Copyright (C) Rijesh Augustine
 *
 * This file is part of paparazzi
 *
 * paparazzi is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * paparazzi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with paparazzi; see the file COPYING.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

/**
 * @file "modules/morsegen/morsegen.c"
 * @author Rijesh Augustine
 * This module is designed to send a morse code signal via a GPIO port.
 * The application is particularily developed to toggle and E/D pin of an RF TX
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "modules/morsegen/morsegen.h"
#include "mcu_periph/gpio.h"


/** GPIO settings **/

#ifndef ENABLE
#define ENABLE    gpio_set
#endif

#ifndef DISABLE
#define DISABLE    gpio_clear
#endif

#ifndef MORSE_GPIO_PIN
#define MORSE_GPIO_PIN    GPIO12
#endif

#ifndef GPIO_CLC
#define GPIO_CLC    GPIOC
#endif


/** Input String **/

#ifndef MESSAGE
#define MESSAGE "VE6RSW"
#endif

#ifndef MESSAGE_LENGTH
#define MESSAGE_LENGTH    25
#endif

#ifndef MORSE_STRING_LENGTH
#define MORSE_STRING_LENGTH    MESSAGE_LENGTH*6
#endif

/** Morse timing */

#ifndef DOT_LENGTH
#define DOT_LENGTH    1
#endif

#ifndef DASH_LENGTH
#define DASH_LENGTH    3
#endif

#ifndef SILENCE_LENGTH
#define SILENCE_LENGTH    1
#endif

#ifndef LETTER_LENGTH
#define LETTER_LENGTH    3
#endif

#ifndef WORD_LENGTH
#define WORD_LENGTH    7
#endif

// void 30min_signal() {}
// void sendmorse() {}



static char message[MESSAGE_LENGTH];
static char morsemsg[MORSE_STRING_LENGTH];
static int  status;      // if hi
static unsigned int  arrpos;
static int  enabled;
static int  running;

void morse_init()
{
    strncpy(message, "hello", MESSAGE_LENGTH);
    gpio_setup_output(GPIO_CLC, MORSE_GPIO_PIN);
    ENABLE(GPIO_CLC,MORSE_GPIO_PIN);
    enabled = 1;
    txt2morse();
    running = 0;
    arrpos  = 0;
}


void runner()
{
    if (running > 1)
    {
        running--;
    }
    else if (running == 1)
    {
        if (enabled == 1)
        {
            toggle();
        }
        running = 0;
    }
    else if (running == 0)
    {
        switch (status)
        {
            case 0: //first run
                toggle();
                status = 1;


            case 1: //currently running
                if (arrpos <= strlen(morsemsg))
                {
                    if (morsemsg[arrpos] == '.')
                    {
                        //run a dot
                        toggle();             //turns on
                        running = DOT_LENGTH; //now we need to indicate that it should only be on for dot length
                    }
                    else if (morsemsg[arrpos] == '-')
                    {
                        //run a dot
                        toggle(); //turns on
                        running = DASH_LENGTH;
                        //now we need to indicate that it should only be on for dot length
                    }
                    else if (morsemsg[arrpos] == '2')
                    {
                        //run a dot
                        running = LETTER_LENGTH - 1;
                        //now we need to indicate that it should only be on for dot length
                    }
                    else if (morsemsg[arrpos] == '1')
                    {
                        running = WORD_LENGTH - LETTER_LENGTH - 1;
                        //now we need to indicate that it should only be on for dot length
                    }

                    arrpos++;
                }
                if (arrpos == strlen(morsemsg))
                {
                    status = 2;
                }

            default: //alreadyrun
                break;
        }
    }
}


void toggle()
{
    if (enabled == 1)
    {
        DISABLE(GPIO_CLC,MORSE_GPIO_PIN);
        enabled = 0;
    }
    else
    {
        ENABLE(GPIO_CLC,MORSE_GPIO_PIN);
        enabled = 1;
    }
}


void txt2morse()
{
    unsigned int i;
    int j = 0;

    for (i = 0; i <= strlen(message); i++)
    {
        switch (toupper(message[i]))
        {
            case 'A':
                morsemsg[j++] = '-';
                morsemsg[j]   = '.';
                break;

            case 'B':
                morsemsg[j++] = '-';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j]   = '.';
                break;

            case 'c':
                morsemsg[j++] = '-';
                morsemsg[j++] = '.';
                morsemsg[j++] = '-';
                morsemsg[j]   = '.';
                break;

            case 'D':
                morsemsg[j++] = '-';
                morsemsg[j++] = '.';
                morsemsg[j]   = '.';
                break;

            case 'E':
                morsemsg[j] = '.';
                break;

            case 'F':
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '-';
                morsemsg[j]   = '.';
                break;

            case 'G':
                morsemsg[j++] = '-';
                morsemsg[j++] = '-';
                morsemsg[j]   = '.';
                break;

            case 'H':
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j]   = '.';
                break;

            case 'I':
                morsemsg[j++] = '.';
                morsemsg[j]   = '.';
                break;

            case 'J':
                morsemsg[j++] = '.';
                morsemsg[j++] = '-';
                morsemsg[j++] = '-';
                morsemsg[j]   = '-';
                break;

            case 'K':
                morsemsg[j++] = '-';
                morsemsg[j++] = '.';
                morsemsg[j]   = '-';
                break;

            case 'L':
                morsemsg[j++] = '.';
                morsemsg[j++] = '-';
                morsemsg[j++] = '.';
                morsemsg[j]   = '.';
                break;

            case 'M':
                morsemsg[j++] = '-';
                morsemsg[j]   = '-';
                break;

            case 'N':
                morsemsg[j++] = '-';
                morsemsg[j]   = '.';
                break;

            case 'O':
                morsemsg[j++] = '-';
                morsemsg[j++] = '-';
                morsemsg[j]   = '-';
                break;

            case 'P':
                morsemsg[j++] = '.';
                morsemsg[j++] = '-';
                morsemsg[j++] = '-';
                morsemsg[j]   = '.';
                break;

            case 'Q':
                morsemsg[j++] = '-';
                morsemsg[j++] = '-';
                morsemsg[j++] = '.';
                morsemsg[j]   = '-';
                break;

            case 'R':
                morsemsg[j++] = '.';
                morsemsg[j++] = '-';
                morsemsg[j]   = '.';
                break;

            case 'S':
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j]   = '.';
                break;

            case 'T':
                morsemsg[j] = '-';
                break;

            case 'U':
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j]   = '-';
                break;

            case 'V':
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j]   = '-';
                break;

            case 'W':
                morsemsg[j++] = '.';
                morsemsg[j++] = '-';
                morsemsg[j]   = '-';
                break;

            case 'X':
                morsemsg[j++] = '-';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j]   = '-';
                break;

            case 'y':
                morsemsg[j++] = '-';
                morsemsg[j++] = '.';
                morsemsg[j++] = '-';
                morsemsg[j]   = '-';
                break;

            case 'Z':
                morsemsg[j++] = '-';
                morsemsg[j++] = '-';
                morsemsg[j++] = '.';
                morsemsg[j]   = '.';
                break;

            case '0':
                morsemsg[j++] = '-';
                morsemsg[j++] = '-';
                morsemsg[j++] = '-';
                morsemsg[j++] = '-';
                morsemsg[j]   = '-';
                break;

            case '1':
                morsemsg[j++] = '.';
                morsemsg[j++] = '-';
                morsemsg[j++] = '-';
                morsemsg[j++] = '-';
                morsemsg[j]   = '-';
                break;

            case '2':
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '-';
                morsemsg[j++] = '-';
                morsemsg[j]   = '-';
                break;

            case '3':
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '-';
                morsemsg[j]   = '-';
                break;

            case '4':
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j]   = '-';
                break;

            case '5':
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j]   = '.';
                break;

            case '6':
                morsemsg[j++] = '-';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j]   = '.';
                break;

            case '7':
                morsemsg[j++] = '-';
                morsemsg[j++] = '-';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j]   = '.';
                break;

            case '8':
                morsemsg[j++] = '-';
                morsemsg[j++] = '-';
                morsemsg[j++] = '-';
                morsemsg[j++] = '.';
                morsemsg[j]   = '.';
                break;

            case '9':
                morsemsg[j++] = '-';
                morsemsg[j++] = '-';
                morsemsg[j++] = '-';
                morsemsg[j++] = '-';
                morsemsg[j]   = '.';
                break;

            case '.':
                morsemsg[j++] = '.';
                morsemsg[j++] = '-';
                morsemsg[j++] = '.';
                morsemsg[j++] = '-';
                morsemsg[j++] = '.';
                morsemsg[j]   = '-';
                break;

            case ',':
                morsemsg[j++] = '-';
                morsemsg[j++] = '-';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '-';
                morsemsg[j]   = '-';
                break;

            case ':':
                morsemsg[j++] = '-';
                morsemsg[j++] = '-';
                morsemsg[j++] = '-';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j]   = '.';
                break;

            case '?':
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '-';
                morsemsg[j++] = '-';
                morsemsg[j++] = '.';
                morsemsg[j]   = '.';
                break;

            /* case '\';
            * morsemsg[j++]='.';
            * morsemsg[j++]='.';
            * morsemsg[j++]='.';
            * morsemsg[j++]='.';
            * morsemsg[j++]='.';
            * morsemsg[j]='-';
            *  break; */

            case '-':
                morsemsg[j++] = '-';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j]   = '-';
                break;

            case ';':
                morsemsg[j++] = '-';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '-';
                morsemsg[j]   = '.';
                break;

            case '(':
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j]   = '-';
                break;

            case ')':
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j]   = '-';
                break;

            case ']':
                morsemsg[j++] = '-';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '-';
                morsemsg[j++] = '-';
                morsemsg[j]   = '-';
                break;

            case '[':
                morsemsg[j++] = '-';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '-';
                morsemsg[j++] = '-';
                morsemsg[j]   = '.';
                break;

            case '}':
                morsemsg[j++] = '-';
                morsemsg[j++] = '.';
                morsemsg[j++] = '-';
                morsemsg[j++] = '-';
                morsemsg[j++] = '.';
                morsemsg[j]   = '-';
                break;

            case '{':
                morsemsg[j++] = '-';
                morsemsg[j++] = '.';
                morsemsg[j++] = '-';
                morsemsg[j++] = '-';
                morsemsg[j++] = '.';
                morsemsg[j]   = '-';
                break;

            case '"':
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j]   = '-';
                break;

            case '+':
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j]   = '-';
                break;

            case '/':
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j]   = '-';
                break;

            case '%':
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j]   = '-';
                break;

            case '&':
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j]   = '-';
                break;

            case '$':
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j]   = '-';
                break;

            case '*':
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j]   = '-';
                break;

            case '^':
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j]   = '-';
                break;

            case '#':
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j]   = '-';
                break;

            case '@':
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j]   = '-';
                break;

            case '=':
                morsemsg[j++] = '-';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j++] = '.';
                morsemsg[j]   = '-';
                break;

            case ' ':
                morsemsg[j] = '1';
                break;
        }
        morsemsg[++j] = '2';         //indicating end of letter
        j++;
    }
    morsemsg[j - 1] = '\0';
}

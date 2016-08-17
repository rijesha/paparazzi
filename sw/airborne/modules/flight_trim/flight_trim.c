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
 * @file "modules/flight_trim/flight_trim.c"
 * @author Rijesh Augustine
 * This module is designed to allow engaging AUTO2 on the first flight of you aircraft. It will fix a poorly calibrated transmitter and wi.
 */

#include "modules/flight_trim/flight_trim.h"
#include "subsystems/radio_control.h"

struct FlightTrim flight_trim;

void flight_trim_init(void)
{
  flight_trim.calib_rc = FALSE;
  flight_trim.trim_ac = FALSE;
}
void flight_trim_event(void)
{
  if (flight_trim.calib_rc)
  {
    flight_trim_rc();
  }
  if (flight_trim.trim_ac)
  {
    flight_trim_ac();
  }
}
void flight_trim_rc(void)
{
  uint8_t i;
  for (i = 0; i < 5; i++) {
    if (i == RADIO_THROTTLE) {
      radio_control.neutrals[i] += -radio_control.values[i];
    } else {
      radio_control.neutrals[i] += -radio_control.values[i];
    }
  }
  flight_trim.calib_rc = FALSE;
}
void flight_trim_ac(void)
{
  flight_trim.trim_ac = FALSE;
}

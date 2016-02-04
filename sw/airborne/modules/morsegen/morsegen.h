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
 * @file "modules/morsegen/morsegen.h"
 * @author Rijesh Augustine
 * This module is designed to send a morse code signal via a GPIO port. The application is particularily developed to tog
 */

#ifndef MORSEGEN_H
#define MORSEGEN_H

 extern void morse_init();
 extern void txt2morse();
 extern void runner();
 extern void toggle();
// extern void sendmorse();

#endif

/* Copyright (C) 2021 U. Niethammer,  http://helmparms3d.sourceforge.net/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef HELMERT3D_H
#define HELMERT3D_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define VERS "1.0.7"
#define C_AUTHOR "U. Niethammer"
#define C_YEAR "2022"
#define DEBUG 0
#define DEG2RAD (M_PI / 180.0)
#define RAD2DEG (180 / M_PI)
#define RAD2SEC (RAD2DEG * 3600)
#define DEFAULT_A 6378137.0
#define DEFAULT_B 6356752.3142

#endif // HELMERT3D_H

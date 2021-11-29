/**
 *    termux-usb is used for communicating with usb devices on android
 *    Copyright (C) 2021 Henrik Grimler
 *
 *    This file is part of termux-api.
 *
 *    termux-api is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    fooddiary is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with termux-api. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef TERMUX_USB_H
#define TERMUX_USB_H

#if defined(__cplusplus)
extern "C" {
#endif

intptr_t termux_usb_get_fd_from_usbfs(char *);
intptr_t termux_usb_get_fd_from_ids(char *, char *);

#if defined(__cplusplus)
}
#endif

#endif /* TERMUX_USB_H */

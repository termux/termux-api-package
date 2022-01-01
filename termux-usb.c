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

#include <stdio.h>

#include "termux-api.h"
#include "termux-usb.h"

intptr_t termux_usb_get_fd_from_usbfs(char *usbfs_addr)
{
    /* Check so usbfs path seems reasonable */
    if (!usbfs_addr || strncmp(usbfs_addr, "/dev/bus/usb/", 13) != 0) {
        fprintf(stderr, "Error: Invalid usbfs path\n");
        return -1;
    }

    char *argv[] = {
        "termux-api",
        "Usb",
        "-a",
        "open",
        "--ez",
        "request",
        "true",
        "--es",
        "device",
        usbfs_addr,
    };
    int argc = sizeof(argv)/sizeof(argv[0]) - 1;
    int buf_len = 1024;
    char buf[buf_len];

    api_command mode = TERMUX_USB_GET_FD;

    int fd = run_api_command(argc, argv, mode, buf, buf_len);

    return (intptr_t) fd;
};

intptr_t termux_usb_get_fd_from_ids(char *vendor_id, char *product_id)
{
    /* Check so vendor_id and product_id seems reasonable */
    if (!vendor_id || strlen(vendor_id) != 6
        || strncmp(vendor_id, "0x", 2) != 0) {
        fprintf(stderr, "Error: vendorId doesn't match 0xab12 format\n");
        return -1;
    }
    if (!product_id || strlen(product_id) != 6
        || strncmp(product_id, "0x", 2) != 0) {
        fprintf(stderr, "Error: productId doesn't match 0xab12 format\n");
        return -1;
    }

    char *argv[] = {
        "termux-api",
        "Usb",
        "-a",
        "open",
        "--ez",
        "request",
        "true",
        "--es",
        "vendorId",
        vendor_id,
        "--es",
        "productId",
        product_id,
    };

    int argc = sizeof(argv)/sizeof(argv[0]) - 1;
    api_command mode = TERMUX_USB_GET_FD;
    int buf_len = 1024;
    char buf[buf_len];

    int fd = run_api_command(argc, argv, mode, buf, buf_len);

    return (intptr_t) fd;
};

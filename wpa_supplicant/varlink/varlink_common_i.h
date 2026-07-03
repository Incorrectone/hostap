/*
 * wpa_supplicant Varlink control interface - internal definitions
 * Copyright (c) 2026, Kartik Rajput <incorrectmail101@gmail.com>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
*/

#ifndef HOSTAP_VARLINK_COMMON_I_H
#define HOSTAP_VARLINK_COMMON_I_H

#include <varlink.h>

struct wpas_varlink_priv {
	struct wpa_global * global;
	VarlinkService * service;
	int varlink_fd;
};

#endif /* HOSTAP_VARLINK_COMMON_I_H */ 

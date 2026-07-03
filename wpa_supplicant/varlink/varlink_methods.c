/*
 * wpa_supplicant D-Bus control interface - common definitions
 * Copyright (c) 2026, Kartik Rajput <incorrectmail101@gmail.com>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
*/

#include "utils/includes.h"
#include <varlink.h>

#include "utils/common.h"
#include "utils/wpa_debug.h"
#include "utils/eloop.h"
#include "varlink_common_i.h"
#include "varlink_common.h"
#include "../wpa_supplicant_i.h"

long wpas_varlink_get_state(VarlinkService *service,
								   VarlinkCall *call,
								   VarlinkObject *parameters,
								   uint64_t flags,
								   void *userdata)
{
	struct wpas_varlink_priv *priv = userdata;
	const char *ifname;
	struct wpa_supplicant *wpa_s;
	VarlinkObject *out = NULL;

	if (varlink_object_get_string(parameters, "ifname", &ifname) < 0) {
		return varlink_call_reply_invalid_parameter(call, "ifname");
	}

	wpa_s = wpa_supplicant_get_iface(priv->global, ifname);
	if (wpa_s == NULL) {
		/* If it doesn't exist, return our custom Varlink error */
		VarlinkObject *err_params;
		varlink_object_new(&err_params);
		varlink_object_set_string(err_params, "ifname", ifname);
		long ret = varlink_call_reply_error(call, WPAS_VARLINK_INTERFACE ".InterfaceUnknown", err_params);
		varlink_object_unref(err_params);
		return ret;
	}

	varlink_object_new(&out);
	
	varlink_object_set_string(out, "state", wpa_supplicant_state_txt(wpa_s->wpa_state));
	
	varlink_call_reply(call, out, 0);
	varlink_object_unref(out);

	return 0;
}
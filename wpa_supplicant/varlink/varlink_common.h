/*
 * wpa_supplicant Varlink control interface - common definitions
 * Copyright (c) 2026, Kartik Rajput <incorrectmail101@gmail.com>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
*/

#ifndef HOSTAP_VARLINK_COMMON_H
#define HOSTAP_VARLINK_COMMON_H

#define WPAS_VARLINK_ADDRESS       "unix:/run/wpa_supplicant/varlink"
#define WPAS_VARLINK_FILE_PATH		"/run/wpa_supplicant/varlink"
#define WPAS_VARLINK_PARENT_FOLDER       "/run/wpa_supplicant"

#define WPAS_VARLINK_INTERFACE     "fi.w1.wpasupplicant"

#define WPAS_VARLINK_VENDOR        "fi.w1"
#define WPAS_VARLINK_PRODUCT       "wpa_supplicant"
#define WPAS_VARLINK_VERSION       "1"
#define WPAS_VARLINK_VENDOR_URL    "https://w1.fi/"

struct wpas_varlink_priv;
struct wpa_global;

#ifdef CONFIG_CTRL_IFACE_VARLINK
	struct wpas_varlink_priv * wpas_varlink_init(struct wpa_global * global);
	void wpas_varlink_deinit(struct wpas_varlink_priv * priv);
#else
	static inline struct wpas_varlink_priv * wpas_varlink_init(struct wpa_global *global)
	{
		return NULL;
	}

	static inline void wpas_varlink_deinit(struct wpas_varlink_priv *priv)
	{
	}
#endif

#endif /* HOSTAP_VARLINK_COMMON_H */

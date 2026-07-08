/*
 * wpa_supplicant Varlink control interface - Server
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
#include "varlink_methods.h"
#include "../wpa_supplicant_i.h"

#include <sys/stat.h>
#include <unistd.h>

void wpas_varlink_receive_handler(int sock, void *eloop_ctx, void *sock_ctx)
{
	struct wpas_varlink_priv *priv = eloop_ctx;
	
	varlink_service_process_events(priv->service);
}

void wpas_varlink_deinit(struct wpas_varlink_priv * priv){
	if (priv == NULL)
		return;
		
	if (priv->varlink_fd >= 0) {
		eloop_unregister_read_sock(priv->varlink_fd);
	}
	
	if (priv->service) {
		varlink_service_free(priv->service);
	}

	wpa_printf(MSG_DEBUG, "varlink: Unlink previous socket '%s'", WPAS_VARLINK_FILE_PATH);
	unlink("/run/wpa_supplicant/varlink");
	
	wpa_printf(MSG_DEBUG, "varlink: De-Register varlink object '%s'", WPAS_VARLINK_ADDRESS);

	os_free(priv);
}

struct wpas_varlink_priv * wpas_varlink_init(struct wpa_global * global){
	struct wpas_varlink_priv * priv;

	priv = os_zalloc(sizeof(*priv));
	if (priv == NULL)
		return NULL;
	priv->global = global;
	priv->varlink_fd = -1;

    /* rwxr-xr-x */
	wpa_printf(MSG_DEBUG, "varlink: Creating varlink folder '%s'", WPAS_VARLINK_PARENT_FOLDER);
    mkdir(WPAS_VARLINK_PARENT_FOLDER, 0755);
	/* Clear previous socket if still there */
	wpa_printf(MSG_DEBUG, "varlink: Unlink previous socket '%s'", WPAS_VARLINK_FILE_PATH);
    unlink(WPAS_VARLINK_FILE_PATH);

	if(varlink_service_new(&priv->service, 
						WPAS_VARLINK_VENDOR, 
						WPAS_VARLINK_PRODUCT, 
						WPAS_VARLINK_VERSION,
						WPAS_VARLINK_VENDOR_URL, 
						WPAS_VARLINK_ADDRESS, 
						-1) != 0){
		wpas_varlink_deinit(priv);
		return NULL;
	}

	const char *schema = 
		"interface " WPAS_VARLINK_INTERFACE "\n"
		"method GetState(ifname: string) -> (state: string)\n"
		"error InterfaceUnknown(ifname: string)\n";

	if (varlink_service_add_interface(priv->service, schema,
									"GetState", 
									wpas_varlink_get_state, 
									priv,
									NULL) != 0) {
		wpa_printf(MSG_ERROR, "varlink: Failed to add interface");
		wpas_varlink_deinit(priv);
		return NULL;
	}

	priv->varlink_fd = varlink_service_get_fd(priv->service);
	if(priv->varlink_fd < 0){
		wpas_varlink_deinit(priv);
		return NULL;
	}

	eloop_register_read_sock(priv->varlink_fd, wpas_varlink_receive_handler, priv, NULL);

	wpa_printf(MSG_DEBUG, "varlink: Register varlink object '%s'", WPAS_VARLINK_ADDRESS);

	return priv;
}


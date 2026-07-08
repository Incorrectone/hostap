/*
 * wpa_supplicant Varlink control interface - method definitions
 * Copyright (c) 2026, Kartik Rajput <incorrectmail101@gmail.com>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
*/

#ifndef HOSTAP_VARLINK_METHODS_H
#define HOSTAP_VARLINK_METHODS_H

#include <varlink.h>

long wpas_varlink_get_state(VarlinkService *service,
                            VarlinkCall *call,
                            VarlinkObject *parameters,
                            uint64_t flags,
                            void *userdata);

#endif /* HOSTAP_VARLINK_METHODS_H */
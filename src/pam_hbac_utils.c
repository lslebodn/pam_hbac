/*
    Copyright (C) 2015 Jakub Hrozek <jakub.hrozek@posteo.se>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <security/pam_modules.h>
#include <syslog.h>

#include "pam_hbac.h"
#include "pam_hbac_compat.h"

void
free_string_clist(const char **list)
{
    size_t i;

    if (list == NULL) {
        return;
    }

    for (i = 0; list[i]; i++) {
        free_const(list[i]);
    }
    free(list);
}

void
free_string_list(char **list)
{
    size_t i;

    if (list == NULL) {
        return;
    }

    for (i = 0; list[i]; i++) {
        free(list[i]);
    }
    free(list);
}

size_t
null_string_array_size(char *arr[])
{
    size_t nelem;

    if (arr == NULL) {
        return 0;
    }

    for (nelem = 0; arr[nelem] != NULL; nelem++);

    return nelem;
}

size_t
null_cstring_array_size(const char *arr[])
{
    size_t nelem;

    if (arr == NULL) {
        return 0;
    }

    for (nelem = 0; arr[nelem] != NULL; nelem++);

    return nelem;
}

void logger(pam_handle_t *pamh, int level, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);

#ifdef DEBUG
    va_list apd;
    char debug_msg[DEBUG_MGS_LEN];
    int ret;
    va_copy(apd, ap);

    ret = vsnprintf(debug_msg, DEBUG_MGS_LEN, fmt, apd);
    if (ret >= DEBUG_MGS_LEN) {
        D(("the following message is truncated: %s", debug_msg));
    } else if (ret < 0) {
        D(("vsnprintf failed to format debug message!"));
    } else {
        D((debug_msg));
    }

    va_end(apd);
#endif

    pam_vsyslog(pamh, LOG_AUTHPRIV|level, fmt, ap);

    va_end(ap);
}


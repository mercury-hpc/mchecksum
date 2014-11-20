/*
 * Copyright (C) 2013-2014 Argonne National Laboratory, Department of Energy,
 *                    UChicago Argonne, LLC and The HDF Group.
 * All rights reserved.
 *
 * The full copyright notice, including terms governing use, modification,
 * and redistribution, is contained in the COPYING file that can be
 * found at the root of the source code distribution tree.
 */

#ifndef MCHECKSUM_ERROR_H
#define MCHECKSUM_ERROR_H

#include "mchecksum_config.h"

#define MCHECKSUM_SUCCESS  1
#define MCHECKSUM_FAIL    -1
#define MCHECKSUM_TRUE     1
#define MCHECKSUM_FALSE    0

/* For compatibility */
#if defined(__STDC_VERSION__) &&  (__STDC_VERSION__ < 199901L)
  #if defined(__GNUC__) && (__GNUC__ >= 2)
    #define __func__ __FUNCTION__
  #else
    #define __func__ "<unknown>"
  #endif
#elif defined(_WIN32)
  #define __func__ __FUNCTION__
#endif

/* Default error macro */
#ifdef MCHECKSUM_HAVE_VERBOSE_ERROR
  #include <stdio.h>
  #define MCHECKSUM_ERROR_DEFAULT(x) {              \
        fprintf(stderr, "Error "                  \
                "in %s:%d (%s): "                 \
                "%s.\n",                          \
                __FILE__, __LINE__, __func__, x); \
  }
#else
  #define MCHECKSUM_ERROR_DEFAULT(x)
#endif

#endif /* MCHECKSUM_ERROR_H */

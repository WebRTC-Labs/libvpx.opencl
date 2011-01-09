/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "vp8_opencl.h"

#define CLAMP(x,min,max) if (x < min) x = min; else if ( x > max ) x = max;

const char *idctllm_cl_file_name = "vp8/common/opencl/idctllm_cl.cl";

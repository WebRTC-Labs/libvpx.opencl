/*
 *  Copyright (c) 2011 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef VP8_OPENCL_H
#define	VP8_OPENCL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "vpx_config.h"

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#if HAVE_DLOPEN
#include "dynamic_cl.h"
#endif

extern char *cl_read_file(const char* file_name);
extern int cl_init();
extern void cl_destroy();

#define MAX_NUM_PLATFORMS 4

#define CL_TRIED_BUT_FAILED 1
#define CL_NOT_INITIALIZED -1
extern int cl_initialized;

#define CL_CHECK_SUCCESS(cond,msg,alt,retCode) \
    if ( cond ){ \
        printf(msg);  \
        cl_destroy(); \
        cl_initialized = CL_TRIED_BUT_FAILED; \
        alt; \
        return retCode; \
    }

#define CL_ENSURE_BUF_SIZE(bufRef, bufType, needSize, curSize, dataPtr, altPath) \
    if ( needSize > curSize || bufRef == NULL){ \
        if (bufRef != NULL) \
            clReleaseMemObject(bufRef); \
        if (dataPtr != NULL){ \
            bufRef = clCreateBuffer(cl_data.context, bufType, needSize, dataPtr, &err); \
            CL_CHECK_SUCCESS( \
                err != CL_SUCCESS, \
                "Error copying data to buffer! Using CPU path!\n", \
                altPath, \
                CL_TRIED_BUT_FAILED \
            ); \
        } else {\
            bufRef = clCreateBuffer(cl_data.context, bufType, needSize, NULL, NULL);\
        } \
        CL_CHECK_SUCCESS(!bufRef, \
            "Error: Failed to allocate buffer. Using CPU path!\n", \
            altPath, \
            CL_TRIED_BUT_FAILED \
        ); \
        curSize = needSize; \
    } else { \
        if (dataPtr != NULL){\
            err = clEnqueueWriteBuffer(cl_data.commands, bufRef, CL_FALSE, 0, \
                needSize, dataPtr, 0, NULL, NULL); \
            \
            CL_CHECK_SUCCESS( err != CL_SUCCESS, \
                "Error: Failed to write to buffer!\n", \
                printf("srcData = %p, intData = %p, destData = %p, bufRef = %p\n", cl_data.srcData, cl_data.intData, cl_data.destData, bufRef);\
                altPath, \
                CL_TRIED_BUT_FAILED \
            ); \
        }\
    }

typedef struct VP8_COMMON_CL {
    cl_device_id device_id; // compute device id
    cl_context context; // compute context
    cl_command_queue commands; // compute command queue
    cl_program program; // compute program
    cl_kernel filter_block2d_first_pass_kernel; // compute kernel
    cl_kernel filter_block2d_second_pass_kernel; // compute kernel
    cl_mem srcData; //Source frame data
    size_t srcAlloc; //Amount of allocated CL memory for srcData
    cl_mem intData; //Intermediate data passed from 1st to 2nd pass
    size_t intAlloc; //Amount of allocated CL memory for intData
    size_t intSize; //Size of intermediate data.
    cl_mem destData; //Destination data for 2nd pass.
    size_t destAlloc; //Amount of allocated CL memory for destData
    cl_mem filterData; //vp8_filter row
} VP8_COMMON_CL;

extern VP8_COMMON_CL cl_data;

#ifdef	__cplusplus
}
#endif

#endif	/* VP8_OPENCL_H */

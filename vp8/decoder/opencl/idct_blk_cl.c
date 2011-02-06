/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "vp8/decoder/onyxd_int.h"
#include "vpx_ports/config.h"
#include "../../common/idct.h"
#include "vp8/common/opencl/blockd_cl.h"
#include "dequantize_cl.h"

//change q/dq/pre/eobs/dc to offsets
void vp8_dequant_dc_idct_add_y_block_cl(
    BLOCKD *b,
    short *q,           //xd->qcoeff
    short *dq,          //xd->block[0].dequant
    unsigned char *pre, //xd->predictor
    unsigned char *dst, //xd->dst.y_buffer
    int stride,         //xd->dst.y_stride
    char *eobs,         //xd->eobs
    int dc_offset       //xd->block[24].diff_offset
)
{
    int i, j;
    short *dc = b->diff_base;
    int q_offset = 0;
    int pre_offset = 0;
    int dst_offset = 0;
    //dc_offset = 0;
    size_t dst_size = 16*(stride+1);

    CL_FINISH(b->cl_commands);

    vp8_cl_block_prep(b, QCOEFF|DEQUANT|DIFF|PREDICTOR);
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (*eobs++ > 1){
                CL_FINISH(b->cl_commands);
                vp8_dequant_dc_idct_add_cl (b, q_offset, pre_offset, dst+dst_offset, 16, stride, dc_offset);
            }
            else{
                vp8_dc_only_idct_add_cl(b, CL_TRUE, dc_offset, 0, pre_offset, dst, dst_offset, dst_size, 16, stride);
            }

            q_offset   += 16;
            pre_offset += 4;
            dst_offset += 4;
            dc_offset++;
        }

        pre_offset += 64 - 16;
        dst_offset += 4*stride - 16;
    }

    vp8_cl_block_finish(b, QCOEFF);
    CL_FINISH(b->cl_commands);

}

void vp8_dequant_idct_add_y_block_cl (VP8D_COMP *pbi, MACROBLOCKD *xd)
{
    int i, j;

    short *q = xd->qcoeff;
    int q_offset = 0;
    short *dq = xd->block[0].dequant;
    unsigned char *pre = xd->predictor;
    int pre_offset = 0;
    unsigned char *dsty = xd->dst.y_buffer;
    int dst_offset = 0;
    int stride = xd->dst.y_stride;
    char *eobs = xd->eobs;
    int dst_size = 16 * (stride + 1);

    CL_FINISH(xd->cl_commands);

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (*eobs++ > 1){
                vp8_cl_block_prep(&xd->block[0], QCOEFF|DEQUANT|PREDICTOR);
                vp8_dequant_idct_add_cl(&xd->block[0], dsty, dst_offset, dst_size, q_offset, pre_offset, 16, stride, pbi->dequant.idct_add);
                vp8_cl_block_finish(&xd->block[0], QCOEFF);
                CL_FINISH(xd->cl_commands);
            }
            else
            {
                vp8_cl_block_prep(&xd->block[0], PREDICTOR|BLOCK_COPY_ALL);
                vp8_dc_only_idct_add_cl(&xd->block[0], CL_FALSE, 0, q_offset, pre_offset, dsty, dst_offset, dst_size, 16, stride);
                CL_FINISH(xd->cl_commands);
                ((int *)(q+q_offset))[0] = 0;
            }

            q_offset   += 16;
            pre_offset += 4;
            dst_offset += 4;
        }

        pre_offset += 64 - 16;
        dst_offset += 4*stride - 16;
    }

    CL_FINISH(xd->cl_commands);
}

void vp8_dequant_idct_add_uv_block_cl(VP8D_COMP *pbi, MACROBLOCKD *xd,
        vp8_dequant_idct_add_uv_block_fn_t idct_add_uv_block
)
{
    int i, j;

    int block_num = 16;
    BLOCKD b = xd->block[block_num];

    short *q = xd->qcoeff;
    short *dq = b.dequant;
    unsigned char *pre = xd->predictor;
    unsigned char *dstu = xd->dst.u_buffer;
    unsigned char *dstv = xd->dst.v_buffer;
    int stride = xd->dst.uv_stride;
    size_t dst_size = 8*(stride+1);
    size_t cur_size = 0;
    char *eobs = xd->eobs+16;

    int pre_offset = block_num*16;
    int q_offset = block_num*16;
    int dst_offset = 0;
    int err;

    cl_mem dest_mem = NULL;

    //Initialize destination memory
    CL_ENSURE_BUF_SIZE(b.cl_commands, dest_mem, CL_MEM_READ_WRITE|CL_MEM_COPY_HOST_PTR,
            dst_size, cur_size, dstu,
    );

    if (cl_initialized != CL_SUCCESS){
        DEQUANT_INVOKE (&pbi->dequant, idct_add_uv_block)
        (xd->qcoeff+block_num*16, b.dequant,
         xd->predictor+block_num*16, xd->dst.u_buffer, xd->dst.v_buffer,
         xd->dst.uv_stride, xd->eobs+block_num);
        return;
    }

    CL_FINISH(xd->cl_commands);

    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
        {
            if (*eobs++ > 1){
                //vp8_dequant_idct_add_cl (xd->block[16], q, dq, pre, dstu, 8, stride);
                CL_FINISH(xd->cl_commands);
                vp8_cl_block_prep(&xd->block[0], QCOEFF|DEQUANT|PREDICTOR);
                vp8_dequant_idct_add_cl(&b, dstu, dst_offset, dst_size, q_offset, pre_offset, 8, stride, DEQUANT_INVOKE (&pbi->dequant, idct_add));
                vp8_cl_block_finish(&xd->block[0], QCOEFF);
                CL_FINISH(xd->cl_commands);
            }
            else
            {
                CL_FINISH(xd->cl_commands);
                vp8_cl_block_prep(&xd->block[block_num], PREDICTOR|BLOCK_COPY_ALL);
                vp8_dc_only_idct_add_cl (&b, CL_FALSE, 0, q_offset, pre_offset, dstu, dst_offset, dst_size, 8, stride);
                CL_FINISH(xd->cl_commands);
                ((int *)(q+q_offset))[0] = 0;
            }

            q_offset    += 16;
            pre_offset  += 4;
            dst_offset += 4;
        }

        pre_offset  += 32 - 8;
        dst_offset += 4*stride - 8;
    }

    //Swap dstu out of cl_mem and dstv into it

    dst_offset = 0;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
        {
            if (*eobs++ > 1){
                vp8_cl_block_prep(&b, QCOEFF|DEQUANT|PREDICTOR);
                vp8_dequant_idct_add_cl (&b, dstv,dst_offset, dst_size, q_offset, pre_offset, 8, stride, DEQUANT_INVOKE (&pbi->dequant, idct_add));
                vp8_cl_block_finish(&b, QCOEFF);
                CL_FINISH(xd->cl_commands);
            }
            else
            {
                vp8_cl_block_prep(&b, PREDICTOR|BLOCK_COPY_ALL);
                vp8_dc_only_idct_add_cl (&b, CL_FALSE, 0, q_offset, pre_offset, dstv, dst_offset, dst_size, 8, stride);
                CL_FINISH(xd->cl_commands);
                ((int *)(q+q_offset))[0] = 0;
            }

            q_offset    += 16;
            pre_offset  += 4;
            dst_offset += 4;
        }

        pre_offset  += 32 - 8;
        dst_offset += 4*stride - 8;
    }

    clReleaseMemObject(dest_mem);

    CL_FINISH(xd->cl_commands);
}

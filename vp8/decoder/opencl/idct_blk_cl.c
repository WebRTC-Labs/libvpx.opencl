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
#include "vp8/common/blockd.h"
#include "dequantize_cl.h"


void vp8_dequant_dc_idct_add_y_block_cl
            (BLOCKD *b, short *q, short *dq, unsigned char *pre,
             unsigned char *dst, int stride, char *eobs, short *dc)
{
    int i, j;
    int q_offset = 0;
    int pre_offset = 0;
    int dst_offset = 0;
    int dc_offset = 0;

    printf("vp8_dequant_dc_idct_add_y_block_cl\n");
    CL_FINISH(b->cl_commands);

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (*eobs++ > 1){
                vp8_dequant_dc_idct_add_cl (b, q+q_offset, dq, pre+pre_offset, dst+dst_offset, 16, stride, dc[dc_offset]);
            }
            else{
                //Note: dc[0] needs to be either verified for unchanging value,
                //      or this needs to become an offset just like everything else
                vp8_dc_only_idct_add_cl(b, dc[dc_offset], pre+pre_offset, dst+dst_offset, 16, stride);
            }

            q_offset   += 16;
            pre_offset += 4;
            dst_offset += 4;
            dc_offset++;
        }

        pre_offset += 64 - 16;
        dst_offset += 4*stride - 16;
    }

    CL_FINISH(b->cl_commands);

}

void vp8_dequant_idct_add_y_block_cl (VP8D_COMP *pbi, MACROBLOCKD *xd, unsigned char *dst)
{
    int i, j;

    short *q = xd->qcoeff;
    int q_offset = 0;
    short *dq = xd->block[0].dequant;
    unsigned char *pre = xd->predictor;
    int pre_offset = 0;
    unsigned char *dest_base = xd->dst.y_buffer;
    int dest_offset = 0;
    int stride = xd->dst.y_stride;
    char *eobs = xd->eobs;


    CL_FINISH(xd->cl_commands);

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            printf("vp8_dequant_idct_add_y_block_cl\n");
            if (*eobs++ > 1){
                vp8_dequant_idct_add_cl(&xd->block[0],dst, dest_offset, q_offset, pre_offset, 16, stride, pbi->dequant.idct_add);
            }
            else
            {
                //Another case where (q+offset)[0] and dq[0] need to become references
                //to cl_mem locations.
                vp8_dc_only_idct_add_cl (&xd->block[0], (q+q_offset)[0]*dq[0], pre+pre_offset, dst+dest_offset, 16, stride);
                ((int *)(q+q_offset))[0] = 0;
            }

            q_offset   += 16;
            pre_offset += 4;
            dest_offset += 4;
        }

        pre_offset += 64 - 16;
        dest_offset += 4*stride - 16;
    }
    CL_FINISH(xd->cl_commands);
}

void vp8_dequant_idct_add_uv_block_cl(VP8D_COMP *pbi, MACROBLOCKD *xd,
        vp8_dequant_idct_add_uv_block_fn_t idct_add_uv_block
)
{
    int i, j;

    int block_num = 16;

    short *q = xd->qcoeff;
    short *dq = xd->block[block_num].dequant;
    unsigned char *pre = xd->predictor;
    unsigned char *dstu = xd->dst.u_buffer;
    unsigned char *dstv = xd->dst.v_buffer;
    int stride = xd->dst.uv_stride;
    char *eobs = xd->eobs+16;

    int pre_offset = block_num*16;
    int q_offset = block_num*16;

    
    if (cl_initialized != CL_SUCCESS){
        DEQUANT_INVOKE (&pbi->dequant, idct_add_uv_block)
        (xd->qcoeff+block_num*16, xd->block[block_num].dequant,
         xd->predictor+block_num*16, xd->dst.u_buffer, xd->dst.v_buffer,
         xd->dst.uv_stride, xd->eobs+block_num);
        return;
    }

    CL_FINISH(xd->cl_commands);

    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
        {
            printf("vp8_dequant_idct_add_uv_block_cl\n");
            if (*eobs++ > 1){
                //vp8_dequant_idct_add_cl (xd->block[16], q, dq, pre, dstu, 8, stride);
                vp8_dequant_idct_add_cl(&xd->block[block_num], dstu, 0, q_offset, pre_offset, 8, stride, DEQUANT_INVOKE (&pbi->dequant, idct_add));
            }
            else
            {
                //Another case where (q+offset)[0] and dq[0] need to become references
                //to cl_mem locations.
                vp8_dc_only_idct_add_cl (&xd->block[block_num], *(q+q_offset)*dq[0], pre+pre_offset, dstu, 8, stride);
                ((int *)(q+q_offset))[0] = 0;
            }

            q_offset    += 16;
            pre_offset  += 4;
            dstu += 4;
        }

        pre_offset  += 32 - 8;
        dstu += 4*stride - 8;
    }

    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
        {
            if (*eobs++ > 1){
                vp8_dequant_idct_add_cl (&xd->block[block_num], dstv, 0, q_offset, pre_offset, 8, stride, DEQUANT_INVOKE (&pbi->dequant, idct_add));
            }
            else
            {
                //Another case where (q+offset)[0] and dq[0] need to become references
                //to cl_mem locations.
                vp8_dc_only_idct_add_cl (&xd->block[block_num], (q+q_offset)[0]*dq[0], pre+pre_offset, dstv, 8, stride);
                ((int *)(q+q_offset))[0] = 0;
            }

            q_offset    += 16;
            pre_offset  += 4;
            dstv += 4;
        }

        pre_offset  += 32 - 8;
        dstv += 4*stride - 8;
    }

    CL_FINISH(xd->cl_commands);
}
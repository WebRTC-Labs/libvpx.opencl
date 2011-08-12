/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */


#include "vpx_ports/config.h"
#include "loopfilter.h"
#include "onyxc_int.h"

#if CONFIG_OPENCL
#include "opencl/loopfilter_cl.h"
#endif

typedef unsigned char uc;


prototype_loopfilter(vp8_loop_filter_horizontal_edge_c);
prototype_loopfilter(vp8_loop_filter_vertical_edge_c);
prototype_loopfilter(vp8_mbloop_filter_horizontal_edge_c);
prototype_loopfilter(vp8_mbloop_filter_vertical_edge_c);
prototype_loopfilter(vp8_loop_filter_simple_horizontal_edge_c);
prototype_loopfilter(vp8_loop_filter_simple_vertical_edge_c);

/* Horizontal MB filtering */
void vp8_loop_filter_mbh_c(unsigned char *y_ptr, unsigned char *u_ptr, unsigned char *v_ptr,
                           int y_stride, int uv_stride, loop_filter_info *lfi, int simpler_lpf)
{
    (void) simpler_lpf;
    vp8_mbloop_filter_horizontal_edge_c(y_ptr, y_stride, lfi->mbflim, lfi->lim, lfi->thr, 2);

    if (u_ptr)
        vp8_mbloop_filter_horizontal_edge_c(u_ptr, uv_stride, lfi->mbflim, lfi->lim, lfi->thr, 1);

    if (v_ptr)
        vp8_mbloop_filter_horizontal_edge_c(v_ptr, uv_stride, lfi->mbflim, lfi->lim, lfi->thr, 1);
}

void vp8_loop_filter_mbhs_c(unsigned char *y_ptr, unsigned char *u_ptr, unsigned char *v_ptr,
                            int y_stride, int uv_stride, loop_filter_info *lfi, int simpler_lpf)
{
    (void) u_ptr;
    (void) v_ptr;
    (void) uv_stride;
    (void) simpler_lpf;
    vp8_loop_filter_simple_horizontal_edge_c(y_ptr, y_stride, lfi->mbflim, lfi->lim, lfi->thr, 2);
}

/* Vertical MB Filtering */
void vp8_loop_filter_mbv_c(unsigned char *y_ptr, unsigned char *u_ptr, unsigned char *v_ptr,
                           int y_stride, int uv_stride, loop_filter_info *lfi, int simpler_lpf)
{
    (void) simpler_lpf;
    vp8_mbloop_filter_vertical_edge_c(y_ptr, y_stride, lfi->mbflim, lfi->lim, lfi->thr, 2);

    if (u_ptr)
        vp8_mbloop_filter_vertical_edge_c(u_ptr, uv_stride, lfi->mbflim, lfi->lim, lfi->thr, 1);

    if (v_ptr)
        vp8_mbloop_filter_vertical_edge_c(v_ptr, uv_stride, lfi->mbflim, lfi->lim, lfi->thr, 1);
}

void vp8_loop_filter_mbvs_c(unsigned char *y_ptr, unsigned char *u_ptr, unsigned char *v_ptr,
                            int y_stride, int uv_stride, loop_filter_info *lfi, int simpler_lpf)
{
    (void) u_ptr;
    (void) v_ptr;
    (void) uv_stride;
    (void) simpler_lpf;
    vp8_loop_filter_simple_vertical_edge_c(y_ptr, y_stride, lfi->mbflim, lfi->lim, lfi->thr, 2);
}

/* Horizontal B Filtering */
void vp8_loop_filter_bh_c(unsigned char *y_ptr, unsigned char *u_ptr, unsigned char *v_ptr,
                          int y_stride, int uv_stride, loop_filter_info *lfi, int simpler_lpf)
{
    (void) simpler_lpf;
    vp8_loop_filter_horizontal_edge_c(y_ptr + 4 * y_stride, y_stride, lfi->flim, lfi->lim, lfi->thr, 2);
    vp8_loop_filter_horizontal_edge_c(y_ptr + 8 * y_stride, y_stride, lfi->flim, lfi->lim, lfi->thr, 2);
    vp8_loop_filter_horizontal_edge_c(y_ptr + 12 * y_stride, y_stride, lfi->flim, lfi->lim, lfi->thr, 2);

    if (u_ptr)
        vp8_loop_filter_horizontal_edge_c(u_ptr + 4 * uv_stride, uv_stride, lfi->flim, lfi->lim, lfi->thr, 1);

    if (v_ptr)
        vp8_loop_filter_horizontal_edge_c(v_ptr + 4 * uv_stride, uv_stride, lfi->flim, lfi->lim, lfi->thr, 1);
}

void vp8_loop_filter_bhs_c(unsigned char *y_ptr, unsigned char *u_ptr, unsigned char *v_ptr,
                           int y_stride, int uv_stride, loop_filter_info *lfi, int simpler_lpf)
{
    (void) u_ptr;
    (void) v_ptr;
    (void) uv_stride;
    (void) simpler_lpf;
    vp8_loop_filter_simple_horizontal_edge_c(y_ptr + 4 * y_stride, y_stride, lfi->flim, lfi->lim, lfi->thr, 2);
    vp8_loop_filter_simple_horizontal_edge_c(y_ptr + 8 * y_stride, y_stride, lfi->flim, lfi->lim, lfi->thr, 2);
    vp8_loop_filter_simple_horizontal_edge_c(y_ptr + 12 * y_stride, y_stride, lfi->flim, lfi->lim, lfi->thr, 2);
}

/* Vertical B Filtering */
void vp8_loop_filter_bv_c(unsigned char *y_ptr, unsigned char *u_ptr, unsigned char *v_ptr,
                          int y_stride, int uv_stride, loop_filter_info *lfi, int simpler_lpf)
{
    (void) simpler_lpf;
    vp8_loop_filter_vertical_edge_c(y_ptr + 4, y_stride, lfi->flim, lfi->lim, lfi->thr, 2);
    vp8_loop_filter_vertical_edge_c(y_ptr + 8, y_stride, lfi->flim, lfi->lim, lfi->thr, 2);
    vp8_loop_filter_vertical_edge_c(y_ptr + 12, y_stride, lfi->flim, lfi->lim, lfi->thr, 2);

    if (u_ptr)
        vp8_loop_filter_vertical_edge_c(u_ptr + 4, uv_stride, lfi->flim, lfi->lim, lfi->thr, 1);

    if (v_ptr)
        vp8_loop_filter_vertical_edge_c(v_ptr + 4, uv_stride, lfi->flim, lfi->lim, lfi->thr, 1);
}

void vp8_loop_filter_bvs_c(unsigned char *y_ptr, unsigned char *u_ptr, unsigned char *v_ptr,
                           int y_stride, int uv_stride, loop_filter_info *lfi, int simpler_lpf)
{
    (void) u_ptr;
    (void) v_ptr;
    (void) uv_stride;
    (void) simpler_lpf;
    vp8_loop_filter_simple_vertical_edge_c(y_ptr + 4, y_stride, lfi->flim, lfi->lim, lfi->thr, 2);
    vp8_loop_filter_simple_vertical_edge_c(y_ptr + 8, y_stride, lfi->flim, lfi->lim, lfi->thr, 2);
    vp8_loop_filter_simple_vertical_edge_c(y_ptr + 12, y_stride, lfi->flim, lfi->lim, lfi->thr, 2);
}

void vp8_init_loop_filter(VP8_COMMON *cm)
{
    loop_filter_info *lfi = cm->lf_info;
    LOOPFILTERTYPE lft = cm->filter_type;
    int sharpness_lvl = cm->sharpness_level;
    int frame_type = cm->frame_type;
    int i, j;

    int block_inside_limit = 0;
    int HEVThresh;

    /* For each possible value for the loop filter fill out a "loop_filter_info" entry. */
    for (i = 0; i <= MAX_LOOP_FILTER; i++)
    {
        int filt_lvl = i;

        if (frame_type == KEY_FRAME)
        {
            if (filt_lvl >= 40)
                HEVThresh = 2;
            else if (filt_lvl >= 15)
                HEVThresh = 1;
            else
                HEVThresh = 0;
        }
        else
        {
            if (filt_lvl >= 40)
                HEVThresh = 3;
            else if (filt_lvl >= 20)
                HEVThresh = 2;
            else if (filt_lvl >= 15)
                HEVThresh = 1;
            else
                HEVThresh = 0;
        }

        /* Set loop filter paramaeters that control sharpness. */
        block_inside_limit = filt_lvl >> (sharpness_lvl > 0);
        block_inside_limit = block_inside_limit >> (sharpness_lvl > 4);

        if (sharpness_lvl > 0)
        {
            if (block_inside_limit > (9 - sharpness_lvl))
                block_inside_limit = (9 - sharpness_lvl);
        }

        if (block_inside_limit < 1)
            block_inside_limit = 1;

        for (j = 0; j < 16; j++)
        {
            lfi[i].lim[j] = block_inside_limit;
            lfi[i].mbflim[j] = filt_lvl + 2;
            lfi[i].flim[j] = filt_lvl;
            lfi[i].thr[j] = HEVThresh;
        }

    }

    /* Set up the function pointers depending on the type of loop filtering selected */
    if (lft == NORMAL_LOOPFILTER)
    {
        cm->lf_mbv = LF_INVOKE(&cm->rtcd.loopfilter, normal_mb_v);
        cm->lf_bv  = LF_INVOKE(&cm->rtcd.loopfilter, normal_b_v);
        cm->lf_mbh = LF_INVOKE(&cm->rtcd.loopfilter, normal_mb_h);
        cm->lf_bh  = LF_INVOKE(&cm->rtcd.loopfilter, normal_b_h);
    }
    else
    {
        cm->lf_mbv = LF_INVOKE(&cm->rtcd.loopfilter, simple_mb_v);
        cm->lf_bv  = LF_INVOKE(&cm->rtcd.loopfilter, simple_b_v);
        cm->lf_mbh = LF_INVOKE(&cm->rtcd.loopfilter, simple_mb_h);
        cm->lf_bh  = LF_INVOKE(&cm->rtcd.loopfilter, simple_b_h);
    }
}

/* Put vp8_init_loop_filter() in vp8dx_create_decompressor(). Only call vp8_frame_init_loop_filter() while decoding
 * each frame. Check last_frame_type to skip the function most of times.
 */
void vp8_frame_init_loop_filter(loop_filter_info *lfi, int frame_type)
{
    int HEVThresh;
    int i, j;

    /* For each possible value for the loop filter fill out a "loop_filter_info" entry. */
    for (i = 0; i <= MAX_LOOP_FILTER; i++)
    {
        int filt_lvl = i;

        if (frame_type == KEY_FRAME)
        {
            if (filt_lvl >= 40)
                HEVThresh = 2;
            else if (filt_lvl >= 15)
                HEVThresh = 1;
            else
                HEVThresh = 0;
        }
        else
        {
            if (filt_lvl >= 40)
                HEVThresh = 3;
            else if (filt_lvl >= 20)
                HEVThresh = 2;
            else if (filt_lvl >= 15)
                HEVThresh = 1;
            else
                HEVThresh = 0;
        }

        for (j = 0; j < 16; j++)
        {
            /*lfi[i].lim[j] = block_inside_limit;
            lfi[i].mbflim[j] = filt_lvl+2;*/
            /*lfi[i].flim[j] = filt_lvl;*/
            lfi[i].thr[j] = HEVThresh;
        }
    }
}


int vp8_adjust_mb_lf_value(MACROBLOCKD *mbd, int filter_level)
{
    MB_MODE_INFO *mbmi = &mbd->mode_info_context->mbmi;

    if (mbd->mode_ref_lf_delta_enabled)
    {
        /* Apply delta for reference frame */
        filter_level += mbd->ref_lf_deltas[mbmi->ref_frame];

        /* Apply delta for mode */
        if (mbmi->ref_frame == INTRA_FRAME)
        {
            /* Only the split mode BPRED has a further special case */
            if (mbmi->mode == B_PRED)
                filter_level +=  mbd->mode_lf_deltas[0];
        }
        else
        {
            /* Zero motion mode */
            if (mbmi->mode == ZEROMV)
                filter_level +=  mbd->mode_lf_deltas[1];

            /* Split MB motion mode */
            else if (mbmi->mode == SPLITMV)
                filter_level +=  mbd->mode_lf_deltas[3];

            /* All other inter motion modes (Nearest, Near, New) */
            else
                filter_level +=  mbd->mode_lf_deltas[2];
        }

        /* Range check */
        if (filter_level > MAX_LOOP_FILTER)
            filter_level = MAX_LOOP_FILTER;
        else if (filter_level < 0)
            filter_level = 0;
    }
    return filter_level;
}

void vp8_loop_filter_macroblock(int mb_row, int mb_col, VP8_COMMON *cm,
        MACROBLOCKD *mbd, int baseline_filter_level[],
        YV12_BUFFER_CONFIG *post) {

    int Segment;
    int mb_cols = cm->mb_cols;
    int alt_flt_enabled = mbd->segmentation_enabled;
    loop_filter_info *lfi = cm->lf_info;
    int filter_level;

    unsigned char *y_ptr, *u_ptr, *v_ptr;
    int y_offset = 16 * (mb_col + (mb_row*mb_cols)) + mb_row * (post->y_stride * 16 - post->y_width);
    int uv_offset = 8 * (mb_col + (mb_row*mb_cols)) + mb_row * (post->uv_stride * 8 - post->uv_width);

    mbd->mode_info_context = cm->mi + ((mb_row * (mb_cols+1) + mb_col));
    Segment = (alt_flt_enabled) ? mbd->mode_info_context->mbmi.segment_id : 0;
    filter_level = baseline_filter_level[Segment];

    y_ptr = post->y_buffer + y_offset;
    u_ptr = post->u_buffer + uv_offset;
    v_ptr = post->v_buffer + uv_offset;

    /* Distance of Mb to the various image edges.
     * These specified to 8th pel as they are always compared to values that are in 1/8th pel units
     * Apply any context driven MB level adjustment
     */
    filter_level = vp8_adjust_mb_lf_value(mbd, filter_level);

    if (filter_level) {
        if (mb_col > 0)
            cm->lf_mbv(y_ptr, u_ptr, v_ptr, post->y_stride, post->uv_stride, &lfi[filter_level], cm->simpler_lpf);

        if (mbd->mode_info_context->mbmi.dc_diff > 0)
            cm->lf_bv(y_ptr, u_ptr, v_ptr, post->y_stride, post->uv_stride, &lfi[filter_level], cm->simpler_lpf);

        /* don't apply across umv border */
        if (mb_row > 0)
            cm->lf_mbh(y_ptr, u_ptr, v_ptr, post->y_stride, post->uv_stride, &lfi[filter_level], cm->simpler_lpf);

        if (mbd->mode_info_context->mbmi.dc_diff > 0)
            cm->lf_bh(y_ptr, u_ptr, v_ptr, post->y_stride, post->uv_stride, &lfi[filter_level], cm->simpler_lpf);
    }

}

void vp8_loop_filter_frame
(
    VP8_COMMON *cm,
    MACROBLOCKD *mbd,
    int default_filt_lvl
)
{
    YV12_BUFFER_CONFIG *post = cm->frame_to_show;
    loop_filter_info *lfi = cm->lf_info;
    FRAME_TYPE frame_type = cm->frame_type;

    int mb_row;
    int mb_col;

    int baseline_filter_level[MAX_MB_SEGMENTS];
    int filter_level;
    int alt_flt_enabled = mbd->segmentation_enabled;

    int i;
    unsigned char *y_ptr, *u_ptr, *v_ptr;

#if CONFIG_OPENCL && ENABLE_CL_LOOPFILTER
    if ( cl_initialized == CL_SUCCESS ){
        vp8_loop_filter_frame_cl(cm,mbd,default_filt_lvl);
        return;
    }
#endif

    mbd->mode_info_context = cm->mi;          /* Point at base of Mb MODE_INFO list */

    /* Note the baseline filter values for each segment */
    if (alt_flt_enabled)
    {
        for (i = 0; i < MAX_MB_SEGMENTS; i++)
        {
            /* Abs value */
            if (mbd->mb_segement_abs_delta == SEGMENT_ABSDATA)
                baseline_filter_level[i] = mbd->segment_feature_data[MB_LVL_ALT_LF][i];
            /* Delta Value */
            else
            {
                baseline_filter_level[i] = default_filt_lvl + mbd->segment_feature_data[MB_LVL_ALT_LF][i];
                baseline_filter_level[i] = (baseline_filter_level[i] >= 0) ? ((baseline_filter_level[i] <= MAX_LOOP_FILTER) ? baseline_filter_level[i] : MAX_LOOP_FILTER) : 0;  /* Clamp to valid range */
            }
        }
    }
    else
    {
        for (i = 0; i < MAX_MB_SEGMENTS; i++)
            baseline_filter_level[i] = default_filt_lvl;
    }

    /* Initialize the loop filter for this frame. */
    if ((cm->last_filter_type != cm->filter_type) || (cm->last_sharpness_level != cm->sharpness_level))
        vp8_init_loop_filter(cm);
    else if (frame_type != cm->last_frame_type)
        vp8_frame_init_loop_filter(lfi, frame_type);

    /* Set up the buffer pointers */
    y_ptr = post->y_buffer;
    u_ptr = post->u_buffer;
    v_ptr = post->v_buffer;

#if 0

    /* vp8_filter each macro block */
    for (mb_row = 0; mb_row < cm->mb_rows; mb_row++)
    {
        for (mb_col = 0; mb_col < cm->mb_cols; mb_col++)
        {
            int Segment = (alt_flt_enabled) ? mbd->mode_info_context->mbmi.segment_id : 0;

            filter_level = baseline_filter_level[Segment];
            
            printf("mode_info_offset = %d\n", mbd->mode_info_context - cm->mi);
            printf("row = %d, col = %d\n", mb_row, mb_col);
            printf("y_offset = %d, uv_offset = %d\n", y_ptr - post->y_buffer, u_ptr - post->u_buffer);

            /* Distance of Mb to the various image edges.
             * These specified to 8th pel as they are always compared to values that are in 1/8th pel units
             * Apply any context driven MB level adjustment
             */
            filter_level = vp8_adjust_mb_lf_value(mbd, filter_level);

            printf("filter_level = %d\n", filter_level);
            

            if (filter_level)
            {
                if (mb_col > 0)
                    cm->lf_mbv(y_ptr, u_ptr, v_ptr, post->y_stride, post->uv_stride, &lfi[filter_level], cm->simpler_lpf);

                if (mbd->mode_info_context->mbmi.dc_diff > 0)
                    cm->lf_bv(y_ptr, u_ptr, v_ptr, post->y_stride, post->uv_stride, &lfi[filter_level], cm->simpler_lpf);

                /* don't apply across umv border */
                if (mb_row > 0)
                    cm->lf_mbh(y_ptr, u_ptr, v_ptr, post->y_stride, post->uv_stride, &lfi[filter_level], cm->simpler_lpf);

                if (mbd->mode_info_context->mbmi.dc_diff > 0)
                    cm->lf_bh(y_ptr, u_ptr, v_ptr, post->y_stride, post->uv_stride, &lfi[filter_level], cm->simpler_lpf);
            }

            y_ptr += 16;
            u_ptr += 8;
            v_ptr += 8;

            mbd->mode_info_context++;     /* step to next MB */
        }

        y_ptr += post->y_stride  * 16 - post->y_width;
        u_ptr += post->uv_stride *  8 - post->uv_width;
        v_ptr += post->uv_stride *  8 - post->uv_width;

        mbd->mode_info_context++;         /* Skip border mb */
    }

#else
#define VP8_LOOP_FILTER_RASTER_SCAN 0
#if VP8_LOOP_FILTER_RASTER_SCAN
    for (mb_row = 0; mb_row < cm->mb_rows; mb_row++) {
        for (mb_col = 0; mb_col < cm->mb_cols; mb_col++) {
#else
    //Maximum priority 
    for (i = 0; i < 2 * (cm->mb_rows - 1) + cm->mb_cols ; i++){
        printf("priority = %d\n", i);
        //Process all MBs in current priority
        for (mb_row = 0; mb_row <= i && mb_row < cm->mb_rows; mb_row++){
        //for (mb_row = (i > cm->mb_rows ? cm->mb_rows : i); mb_row >= 0; mb_row--){

            //First row is done left to right, subsequent rows are offset two
            //to the left to prevent corruption of a pure diagonal scan that
            //is offset by 1.
            if (mb_row == 0){
                mb_col = i - mb_row;
            } else {
                mb_col = i - 2 * mb_row;
            }

            //Skip non-existant MBs
            if ((mb_col > -1 && (mb_col < cm->mb_cols)) && (mb_row < cm->mb_rows)){
                printf("row = %d, col = %d\n", mb_row, mb_col);
#endif
                vp8_loop_filter_macroblock(mb_row, mb_col, cm, mbd, baseline_filter_level, post);
#if !VP8_LOOP_FILTER_RASTER_SCAN
            }
#endif
        }
    }
#endif //if !RASTER_SCAN
}


/* Encoder only... */
void vp8_loop_filter_frame_yonly
(
    VP8_COMMON *cm,
    MACROBLOCKD *mbd,
    int default_filt_lvl,
    int sharpness_lvl
)
{
    YV12_BUFFER_CONFIG *post = cm->frame_to_show;

    int i;
    unsigned char *y_ptr;
    int mb_row;
    int mb_col;

    loop_filter_info *lfi = cm->lf_info;
    int baseline_filter_level[MAX_MB_SEGMENTS];
    int filter_level;
    int alt_flt_enabled = mbd->segmentation_enabled;
    FRAME_TYPE frame_type = cm->frame_type;

    (void) sharpness_lvl;

    /*MODE_INFO * this_mb_mode_info = cm->mi;*/ /* Point at base of Mb MODE_INFO list */
    mbd->mode_info_context = cm->mi;          /* Point at base of Mb MODE_INFO list */

    /* Note the baseline filter values for each segment */
    if (alt_flt_enabled)
    {
        for (i = 0; i < MAX_MB_SEGMENTS; i++)
        {
            /* Abs value */
            if (mbd->mb_segement_abs_delta == SEGMENT_ABSDATA)
                baseline_filter_level[i] = mbd->segment_feature_data[MB_LVL_ALT_LF][i];
            /* Delta Value */
            else
            {
                baseline_filter_level[i] = default_filt_lvl + mbd->segment_feature_data[MB_LVL_ALT_LF][i];
                baseline_filter_level[i] = (baseline_filter_level[i] >= 0) ? ((baseline_filter_level[i] <= MAX_LOOP_FILTER) ? baseline_filter_level[i] : MAX_LOOP_FILTER) : 0;  /* Clamp to valid range */
            }
        }
    }
    else
    {
        for (i = 0; i < MAX_MB_SEGMENTS; i++)
            baseline_filter_level[i] = default_filt_lvl;
    }

    /* Initialize the loop filter for this frame. */
    if ((cm->last_filter_type != cm->filter_type) || (cm->last_sharpness_level != cm->sharpness_level))
        vp8_init_loop_filter(cm);
    else if (frame_type != cm->last_frame_type)
        vp8_frame_init_loop_filter(lfi, frame_type);

    /* Set up the buffer pointers */
    y_ptr = post->y_buffer;

    /* vp8_filter each macro block */
    for (mb_row = 0; mb_row < cm->mb_rows; mb_row++)
    {
        for (mb_col = 0; mb_col < cm->mb_cols; mb_col++)
        {
            int Segment = (alt_flt_enabled) ? mbd->mode_info_context->mbmi.segment_id : 0;
            filter_level = baseline_filter_level[Segment];

            /* Apply any context driven MB level adjustment */
            filter_level = vp8_adjust_mb_lf_value(mbd, filter_level);

            if (filter_level)
            {
                if (mb_col > 0)
                    cm->lf_mbv(y_ptr, 0, 0, post->y_stride, 0, &lfi[filter_level], 0);

                if (mbd->mode_info_context->mbmi.dc_diff > 0)
                    cm->lf_bv(y_ptr, 0, 0, post->y_stride, 0, &lfi[filter_level], 0);

                /* don't apply across umv border */
                if (mb_row > 0)
                    cm->lf_mbh(y_ptr, 0, 0, post->y_stride, 0, &lfi[filter_level], 0);

                if (mbd->mode_info_context->mbmi.dc_diff > 0)
                    cm->lf_bh(y_ptr, 0, 0, post->y_stride, 0, &lfi[filter_level], 0);
            }

            y_ptr += 16;
            mbd->mode_info_context ++;        /* step to next MB */

        }

        y_ptr += post->y_stride  * 16 - post->y_width;
        mbd->mode_info_context ++;            /* Skip border mb */
    }

}

/* Encoder only... */
void vp8_loop_filter_partial_frame
(
    VP8_COMMON *cm,
    MACROBLOCKD *mbd,
    int default_filt_lvl,
    int sharpness_lvl,
    int Fraction
)
{
    YV12_BUFFER_CONFIG *post = cm->frame_to_show;

    int i;
    unsigned char *y_ptr;
    int mb_row;
    int mb_col;
    /*int mb_rows = post->y_height >> 4;*/
    int mb_cols = post->y_width  >> 4;

    int linestocopy;

    loop_filter_info *lfi = cm->lf_info;
    int baseline_filter_level[MAX_MB_SEGMENTS];
    int filter_level;
    int alt_flt_enabled = mbd->segmentation_enabled;
    FRAME_TYPE frame_type = cm->frame_type;

    (void) sharpness_lvl;

    /*MODE_INFO * this_mb_mode_info = cm->mi + (post->y_height>>5) * (mb_cols + 1);*/ /* Point at base of Mb MODE_INFO list */
    mbd->mode_info_context = cm->mi + (post->y_height >> 5) * (mb_cols + 1);        /* Point at base of Mb MODE_INFO list */

    linestocopy = (post->y_height >> (4 + Fraction));

    if (linestocopy < 1)
        linestocopy = 1;

    linestocopy <<= 4;

    /* Note the baseline filter values for each segment */
    if (alt_flt_enabled)
    {
        for (i = 0; i < MAX_MB_SEGMENTS; i++)
        {
            /* Abs value */
            if (mbd->mb_segement_abs_delta == SEGMENT_ABSDATA)
                baseline_filter_level[i] = mbd->segment_feature_data[MB_LVL_ALT_LF][i];
            /* Delta Value */
            else
            {
                baseline_filter_level[i] = default_filt_lvl + mbd->segment_feature_data[MB_LVL_ALT_LF][i];
                baseline_filter_level[i] = (baseline_filter_level[i] >= 0) ? ((baseline_filter_level[i] <= MAX_LOOP_FILTER) ? baseline_filter_level[i] : MAX_LOOP_FILTER) : 0;  /* Clamp to valid range */
            }
        }
    }
    else
    {
        for (i = 0; i < MAX_MB_SEGMENTS; i++)
            baseline_filter_level[i] = default_filt_lvl;
    }

    /* Initialize the loop filter for this frame. */
    if ((cm->last_filter_type != cm->filter_type) || (cm->last_sharpness_level != cm->sharpness_level))
        vp8_init_loop_filter(cm);
    else if (frame_type != cm->last_frame_type)
        vp8_frame_init_loop_filter(lfi, frame_type);

    /* Set up the buffer pointers */
    y_ptr = post->y_buffer + (post->y_height >> 5) * 16 * post->y_stride;

    /* vp8_filter each macro block */
    for (mb_row = 0; mb_row<(linestocopy >> 4); mb_row++)
    {
        for (mb_col = 0; mb_col < mb_cols; mb_col++)
        {
            int Segment = (alt_flt_enabled) ? mbd->mode_info_context->mbmi.segment_id : 0;
            filter_level = baseline_filter_level[Segment];

            if (filter_level)
            {
                if (mb_col > 0)
                    cm->lf_mbv(y_ptr, 0, 0, post->y_stride, 0, &lfi[filter_level], 0);

                if (mbd->mode_info_context->mbmi.dc_diff > 0)
                    cm->lf_bv(y_ptr, 0, 0, post->y_stride, 0, &lfi[filter_level], 0);

                cm->lf_mbh(y_ptr, 0, 0, post->y_stride, 0, &lfi[filter_level], 0);

                if (mbd->mode_info_context->mbmi.dc_diff > 0)
                    cm->lf_bh(y_ptr, 0, 0, post->y_stride, 0, &lfi[filter_level], 0);
            }

            y_ptr += 16;
            mbd->mode_info_context += 1;      /* step to next MB */
        }

        y_ptr += post->y_stride  * 16 - post->y_width;
        mbd->mode_info_context += 1;          /* Skip border mb */
    }
}

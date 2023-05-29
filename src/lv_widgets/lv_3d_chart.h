/**
 * @file lv_3d_chart.h
 *
 */

#ifndef LV_3D_CHART_H
#define LV_3D_CHART_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../lv_conf_internal.h"

#include "../lv_core/lv_obj.h"
#include "lv_line.h"

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    lv_coord_t x;
    lv_coord_t y;
    lv_coord_t z;
} lv_vector_t;

// (255,0,0) -> (255,255,0) -> (0,255,0) -> (0,255,255) -> 
// (0,0,255) -> (255,0,255) -> (255,0,0)
typedef struct {
    lv_point_t point;
    lv_color_t color;
} lv_3d_chart_point_t;

typedef struct {
    /*No inherited ext*/ /*Ext. of ancestor*/
    /*New data for this type */
    lv_ll_t points_ll;
    lv_coord_t x_max;
    lv_coord_t y_max;
    lv_coord_t z_max;

} lv_3d_chart_ext_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

lv_obj_t * lv_3d_chart_create(lv_obj_t * par, const lv_obj_t * copy);

/*=====================
 * Setter functions
 *====================*/

lv_3d_chart_point_t * lv_3d_chart_set_next(lv_obj_t * chart, lv_coord_t x, lv_coord_t y, lv_coord_t z);

/*=====================
 * Other functions
 *====================*/

/**
 * Refresh a chart if its data line has changed
 * @param chart pointer to chart object
 */
void lv_3d_chart_refresh(lv_obj_t * chart);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_3D_CHART_H*/

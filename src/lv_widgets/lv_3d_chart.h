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

#define MAX_VER 160
#define MAX_HOR 135
#define MID_VER 80
#define MID_HOR 68

/**********************
 *      TYPEDEFS
 **********************/

// (255,0,0) -> (255,255,0) -> (0,255,0) -> (0,255,255) -> 
// (0,0,255) -> (255,0,255) -> (255,0,0)
typedef struct {
    lv_point_t point;
    lv_color_t color;
} lv_3d_chart_point_t;

typedef struct {
    lv_ll_t points_ll;
} lv_3d_chart_series_t;

typedef struct {
    lv_ll_t series_ll;
    lv_ll_t cursor_ll;
    uint16_t series_cnt;
} lv_3d_chart_ext_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

lv_obj_t * lv_3d_chart_create(lv_obj_t * par, const lv_obj_t * copy);

/*======================
 * Add/remove functions
 *=====================*/

/**
 * Allocate and add a data series to the chart
 * @param chart pointer to a chart object
 * @param color color of the data series
 * @return pointer to the allocated data series
 */
lv_3d_chart_series_t * lv_3d_chart_add_series(lv_obj_t * chart);

/**
 * Deallocate and remove a data series from a chart
 * @param chart pointer to a chart object
 * @param series pointer to a data series on 'chart'
 */
void lv_3d_chart_remove_series(lv_obj_t * chart, lv_3d_chart_series_t * series);

lv_3d_chart_point_t * lv_3d_chart_add_cursor(lv_obj_t * chart, lv_coord_t x, lv_coord_t y, lv_coord_t z);


/*=====================
 * Setter functions
 *====================*/

void lv_3d_chart_set_points(lv_obj_t * chart, lv_3d_chart_series_t * ser, lv_coord_t * y, lv_coord_t * z, uint16_t len);

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

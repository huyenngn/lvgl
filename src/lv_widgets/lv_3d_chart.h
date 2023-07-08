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

/*********************
 *      DEFINES
 *********************/


/**********************
 *      TYPEDEFS
 **********************/
/* 2D point on the screen */
typedef struct {
    lv_point_t point;
    lv_color_t color; /** Point's z-value is dynamically mapped to a color */
} lv_3d_chart_point_t;

/* A series represents a series of points with a common x-value */
typedef struct {
    lv_ll_t points_ll;
} lv_3d_chart_series_t;

/*Data of chart */
typedef struct {
    lv_ll_t series_ll;
    lv_ll_t cursor_ll;
    uint16_t series_cnt;
} lv_3d_chart_ext_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a chart background objects
 * @param par pointer to an object, it will be the parent of the new chart background
 * @param copy pointer to a chart background object, if not NULL then the new object will be copied
 * from it
 * @return pointer to the created chart background
 */
lv_obj_t* lv_3d_chart_create(lv_obj_t* par, const lv_obj_t* copy);

/*======================
 * Add/remove functions
 *=====================*/

/**
 * Allocate and add a data series to the chart
 * @param chart pointer to a chart object
 * @return pointer to the allocated data series
 */
lv_3d_chart_series_t* lv_3d_chart_add_series(lv_obj_t* chart);

/**
 * Deallocate and remove a data series from a chart
 * @param chart pointer to a chart object
 * @param series pointer to a data series on 'chart'
 */
void lv_3d_chart_remove_series(lv_obj_t* chart, lv_3d_chart_series_t* series);

/**
 * Add a cursor with a given color
 * @param chart pointer to chart object
 * @param x x-value of the cursor
 * @param y y-value of the cursor
 * @param z z-value of the cursor
 * @return pointer to the created cursor
 */
lv_3d_chart_point_t* lv_3d_chart_add_cursor(lv_obj_t* chart, lv_coord_t x, lv_coord_t y, lv_coord_t z);

/*=====================
 * Setter functions
 *====================*/

/**
 * Set the value of points from two array
 * @param chart pointer to chart object
 * @param ser pointer to a x-value series on 'chart'
 * @param y array of 'lv_coord_t' y-values (with 'len' elements )
 * @param z array of 'lv_coord_t' z-values (with 'len' elements )
 */
void lv_3d_chart_set_points(lv_obj_t* chart, lv_3d_chart_series_t* ser, lv_coord_t* y, lv_coord_t* z, uint16_t len);

/*=====================
 * Other functions
 *====================*/

/**
 * Refresh a chart if its data line has changed
 * @param chart pointer to chart object
 */
void lv_3d_chart_refresh(lv_obj_t* chart);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_3D_CHART_H*/

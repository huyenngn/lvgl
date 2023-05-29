/**
 * @file lv_3d_chart.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_3d_chart.h"
#include "lv_chart.h"

#include "../lv_misc/lv_debug.h"
#include "../lv_core/lv_refr.h"
#include "../lv_core/lv_disp.h"
#include "../lv_draw/lv_draw.h"
#include "../lv_misc/lv_math.h"
#include "../lv_themes/lv_theme.h"

/*********************
 *      DEFINES
 *********************/
#define LV_OBJX_NAME "lv_3d_chart"

/**********************
 *  STATIC PROTOTYPES
 **********************/
static lv_design_res_t lv_3d_chart_design(lv_obj_t *chart, const lv_area_t *clip_area);
static void draw_points(lv_obj_t *chart, const lv_area_t *clip_area);
static void draw_grid(lv_obj_t *obj, const lv_area_t *clip_area);
static void vector_to_point(lv_vector_t * vector, lv_point_t * point);

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t *lv_3d_chart_create(lv_obj_t *par, const lv_obj_t *copy)
{
    // Create a custom widget to draw the grid
    lv_obj_t *chart = lv_obj_create(par, copy);
    LV_ASSERT_MEM(chart);
    if (chart == NULL)
        return NULL;

    /*Allocate the object type specific extended data*/
    lv_3d_chart_ext_t *ext = lv_obj_allocate_ext_attr(chart, sizeof(lv_3d_chart_ext_t));
    LV_ASSERT_MEM(ext);
    if (ext == NULL)
    {
        lv_obj_del(chart);
        return NULL;
    }

    _lv_ll_init(&ext->points_ll, sizeof(lv_3d_chart_point_t));

    lv_obj_set_design_cb(chart, lv_3d_chart_design);

    LV_LOG_INFO("chart created");

    return chart;
}

/*=====================
 * Setter functions
 *====================*/

lv_3d_chart_point_t *lv_3d_chart_set_next(lv_obj_t *chart, lv_coord_t x, lv_coord_t y, lv_coord_t z)
{
    LV_ASSERT_OBJ(chart, LV_OBJX_NAME);

    lv_3d_chart_ext_t *ext = lv_obj_get_ext_attr(chart);
    lv_3d_chart_point_t *point = _lv_ll_ins_head(&ext->points_ll);
    LV_ASSERT_MEM(point);
    if (point == NULL)
        return NULL;

    lv_vector_t * vector = lv_mem_alloc(sizeof(lv_vector_t));
    LV_ASSERT_MEM(vector);
    if(vector == NULL) {
        return NULL;
    }

    vector->x = x;
    vector->y = y;
    vector->z = z;

    vector_to_point(vector, &point->point);

    lv_mem_free(vector);
    point->color = LV_COLOR_MAKE(255, 0, 0);


    lv_3d_chart_refresh(chart);

    return point;
}

/*=====================
 * Other functions
 *====================*/

/**
 * Refresh a chart if its data line has changed
 * @param chart pointer to chart object
 */
void lv_3d_chart_refresh(lv_obj_t *chart)
{
    LV_ASSERT_OBJ(chart, LV_OBJX_NAME);

    lv_obj_invalidate(chart);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void vector_to_point(lv_vector_t * vector, lv_point_t * point) {

    double mid_y = LV_VER_RES / 3;
    double mid_x = LV_HOR_RES / 2;
    point->x = (vector->x - vector->z) / 1.41 + mid_x;
    point->y = (vector->x + 2*vector->y + vector->z) / 2.45 + mid_y;
}

static lv_design_res_t lv_3d_chart_design(lv_obj_t *chart, const lv_area_t *clip_area)
{
    draw_grid(chart, clip_area);
    draw_points(chart, clip_area);

    return LV_DESIGN_RES_OK;
}

static void draw_points(lv_obj_t *chart, const lv_area_t *clip_area)
{
    lv_3d_chart_ext_t *ext = lv_obj_get_ext_attr(chart);
    if (_lv_ll_is_empty(&ext->points_ll))
        return;

    lv_3d_chart_point_t *point;

    lv_draw_rect_dsc_t point_dsc;
    lv_draw_rect_dsc_init(&point_dsc);
    point_dsc.radius = LV_RADIUS_CIRCLE;

    lv_coord_t point_radius = 5;

    /*Go through all cursor lines*/
    _LV_LL_READ_BACK(ext->points_ll, point)
    {
        point_dsc.bg_color = point->color;

        if (point_radius)
        {
            lv_area_t point_area;

            point_area.x1 = point->point.x;
            point_area.x2 = point_area.x1 + point_radius;
            point_area.x1 -= point_radius;

            point_area.y1 = point->point.y;
            point_area.y2 = point_area.y1 + point_radius;
            point_area.y1 -= point_radius;
            lv_draw_rect(&point_area, clip_area, &point_dsc);
        }
    }
}

static void draw_grid(lv_obj_t *obj, const lv_area_t *clip_area)
{
    lv_draw_line_dsc_t line_dsc;
    lv_draw_line_dsc_init(&line_dsc);
    lv_obj_init_draw_line_dsc(obj, LV_CHART_PART_BG, &line_dsc);

    double mid_y = LV_VER_RES / 3;
    double mid_x = LV_HOR_RES / 2;
    double left_x = 1.73 * (mid_y);
    double m = mid_y / left_x;

    double x, y, n;

    for (x = 0; x < mid_x + 20; x += 10)
    {
        lv_point_t p1;
        lv_point_t p2;
        p1.x = x + mid_x;
        p1.y = 0;
        p2.x = x + mid_x;
        p2.y = m * x + mid_y;
        lv_draw_line(&p1, &p2, clip_area, &line_dsc);
        p2.x = x + mid_x;
        p2.y = m * x + mid_y;
        p1.y = LV_HOR_RES;
        n = p2.y + (m * p2.x);
        p1.x = (p1.y - n) / -m;
        lv_draw_line(&p1, &p2, clip_area, &line_dsc);
        p1.x = mid_x - x;
        p1.y = 0;
        p2.x = mid_x - x;
        p2.y = m * x + mid_y;
        lv_draw_line(&p1, &p2, clip_area, &line_dsc);
        p2.x = mid_x - x;
        p2.y = m * x + mid_y;
        p1.y = LV_HOR_RES;
        n = p2.y - (m * p2.x);
        p1.x = (p1.y - n) / m;
        lv_draw_line(&p1, &p2, clip_area, &line_dsc);
    }

    // Draw horizontal lines
    for (y = 5; y < LV_VER_RES - 10; y += 10)
    {
        lv_point_t p1;
        lv_point_t p2;
        p1.x = 0;
        p1.y = y;
        p2.x = mid_x;
        p2.y = -m * mid_x + y;
        lv_draw_line(&p1, &p2, clip_area, &line_dsc);
        p1.x = mid_x;
        p1.y = -m * mid_x + y;
        p2.x = LV_HOR_RES;
        p2.y = y;
        lv_draw_line(&p1, &p2, clip_area, &line_dsc);
    }
}

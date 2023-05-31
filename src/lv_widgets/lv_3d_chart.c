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

#define MAX_VER 160
#define MAX_HOR 135
#define MID_VER 80
#define MID_HOR 68

// Max values for all axis  (used to scale the data)
#define LV_3D_CHART_XMAX_DEF 200
#define LV_3D_CHART_YMAX_DEF 200
#define LV_3D_CHART_ZMAX_DEF 200

/**********************
 *  STATIC PROTOTYPES
 **********************/
static lv_design_res_t lv_3d_chart_design(lv_obj_t *chart, const lv_area_t *clip_area);
static void draw_points(lv_obj_t *chart, const lv_area_t *clip_area);
static void draw_grid(lv_obj_t *obj, const lv_area_t *clip_area);

/**********************
 *  STATIC VARIABLES
 **********************/
static const int16_t color_thresh[] = {1530, 1275, 1020, 765, 510, 255};

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

    lv_obj_set_size(chart, MAX_HOR, MAX_VER);
    lv_obj_set_pos(chart, 0, 0);

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

    x = 100 * x / LV_3D_CHART_XMAX_DEF;
    y = 100 * y / LV_3D_CHART_YMAX_DEF;
    z = 100 * z / LV_3D_CHART_ZMAX_DEF;

    // Convert 3D vector to 2D point on screen
    point->point.x = 0.707*x - 0.707*y + 0.0*z + MID_HOR;
    point->point.y = 0.409*x + 0.409*y - 0.816*z + MID_VER;

    // Assign color
    int16_t c = color_thresh[0] * z / LV_3D_CHART_ZMAX_DEF;
    uint8_t dif = c % 255;
    switch (c / 255)
    {
    case 0:
        point->color = LV_COLOR_MAKE(255, dif, 0);
        break;
    case 1:
        point->color = LV_COLOR_MAKE((255-dif), 255, 0);
        break;
    case 2:
        point->color = LV_COLOR_MAKE(0, 255, dif);
        break;
    case 3:
        point->color = LV_COLOR_MAKE(0, (255-dif), 255);
        break;
    case 4:
        point->color = LV_COLOR_MAKE(dif, 0, 255);
        break;
    default:
        point->color = LV_COLOR_MAKE(255, 0, (255-dif));
        break;
    }
    
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

    double left_x = 1.73 * (MID_VER);
    double m = MID_VER / left_x;

    double x, y, n;

    // Draw vertical lines
    for (x = 0; x < left_x; x += 10)
    {
        lv_point_t p1;
        lv_point_t p2;
        p1.x = x + MID_HOR;
        p1.y = 0;
        p2.x = x + MID_HOR;
        p2.y = m * x + MID_VER;
        lv_draw_line(&p1, &p2, clip_area, &line_dsc);
        p2.x = x + MID_HOR;
        p2.y = m * x + MID_VER;
        p1.y = MAX_VER;
        n = p2.y + (m * p2.x);
        p1.x = (p1.y - n) / -m;
        lv_draw_line(&p1, &p2, clip_area, &line_dsc);
        p1.x = MID_HOR - x;
        p1.y = 0;
        p2.x = MID_HOR - x;
        p2.y = m * x + MID_VER;
        lv_draw_line(&p1, &p2, clip_area, &line_dsc);
        p2.x = MID_HOR - x;
        p2.y = m * x + MID_VER;
        p1.y = MAX_VER;
        n = p2.y - (m * p2.x);
        p1.x = (p1.y - n) / m;
        lv_draw_line(&p1, &p2, clip_area, &line_dsc);
    }

    // Draw horizontal lines

    int16_t k = m * MID_HOR + MID_VER;
    for (y = 0; y < k; y += 11)
    {
        lv_point_t p1;
        lv_point_t p2;
        p1.x = 0;
        p1.y = y;
        p2.x = MID_HOR;
        p2.y = -m * MID_HOR + y;
        lv_draw_line(&p1, &p2, clip_area, &line_dsc);
        p1.x = MID_HOR;
        p1.y = -m * MID_HOR + y;
        p2.x = MAX_HOR;
        p2.y = y;
        lv_draw_line(&p1, &p2, clip_area, &line_dsc);
    }
}

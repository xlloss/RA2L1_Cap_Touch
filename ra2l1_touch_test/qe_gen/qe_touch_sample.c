/***********************************************************************
*
* FILE : qe_sample_main.c
* DATE : 2022-02-14
* DESCRIPTION : Main Program for RA
*
* NOTE:THIS IS A TYPICAL EXAMPLE.
*
***********************************************************************/
#include "qe_touch_config.h"
#include "r_rssk_touch_led.h"
#include "r_rssk_switch_led.h"
#define TOUCH_SCAN_INTERVAL_EXAMPLE (20)    /* milliseconds */

void qe_touch_main(void);
void r_rssk_initialize(void);
void r_rssk_led_test(void);

uint64_t button_status;
#if (TOUCH_CFG_NUM_SLIDERS != 0)
uint16_t slider_position[TOUCH_CFG_NUM_SLIDERS];
#endif
#if (TOUCH_CFG_NUM_WHEELS != 0)
uint16_t wheel_position[TOUCH_CFG_NUM_WHEELS];
#endif

void qe_touch_main(void)
{
    fsp_err_t err;

    r_rssk_initialize();

    r_rssk_led_test();

    /* Open Touch middleware */
    err = RM_TOUCH_Open(g_qe_touch_instance_config01.p_ctrl, g_qe_touch_instance_config01.p_cfg);
    if (FSP_SUCCESS != err)
    {
        while (true) {}
    }
    err = RM_TOUCH_Open(g_qe_touch_instance_config02.p_ctrl, g_qe_touch_instance_config02.p_cfg);
    if (FSP_SUCCESS != err)
    {
        while (true) {}
    }
    err = RM_TOUCH_Open(g_qe_touch_instance_config03.p_ctrl, g_qe_touch_instance_config03.p_cfg);
    if (FSP_SUCCESS != err)
    {
        while (true) {}
    }

    /* Open AGT module */
    R_AGT_Open(&g_timer0_ctrl, &g_timer0_cfg);
    R_AGT_Start(&g_timer0_ctrl);

    /* Main loop */
    while (true)
    {
        /* for [CONFIG01] configuration */
        err = RM_TOUCH_ScanStart(g_qe_touch_instance_config01.p_ctrl);
        if (FSP_SUCCESS != err)
        {
            while (true) {}
        }
        while (0 == g_qe_touch_flag) {}
        g_qe_touch_flag = 0;

        err = RM_TOUCH_DataGet(g_qe_touch_instance_config01.p_ctrl, &button_status, NULL, NULL);
        if (FSP_SUCCESS == err)
        {
            /* TODO: Add your own code here. */
        }

        /* for [CONFIG02] configuration */
        err = RM_TOUCH_ScanStart(g_qe_touch_instance_config02.p_ctrl);
        if (FSP_SUCCESS != err)
        {
            while (true) {}
        }
        while (0 == g_qe_touch_flag) {}
        g_qe_touch_flag = 0;

        err = RM_TOUCH_DataGet(g_qe_touch_instance_config02.p_ctrl, NULL, slider_position, NULL);
        if (FSP_SUCCESS == err)
        {
            /* TODO: Add your own code here. */
        }

        /* for [CONFIG03] configuration */
        err = RM_TOUCH_ScanStart(g_qe_touch_instance_config03.p_ctrl);
        if (FSP_SUCCESS != err)
        {
            while (true) {}
        }
        while (0 == g_qe_touch_flag) {}
        g_qe_touch_flag = 0;

        err = RM_TOUCH_DataGet(g_qe_touch_instance_config03.p_ctrl, NULL, NULL, wheel_position);
        if (FSP_SUCCESS == err)
        {
            /* TODO: Add your own code here. */
        }

        /* FIXME: Since this is a temporary process, so re-create a waiting process yourself. */
        R_BSP_SoftwareDelay(TOUCH_SCAN_INTERVAL_EXAMPLE, BSP_DELAY_UNITS_MILLISECONDS);
    }
}

void timer0_callback(timer_callback_args_t *p_args)
{
    if(p_args->event == TIMER_EVENT_CYCLE_END)
    {
        r_rssk_touch_led_control(&button_status, slider_position, wheel_position);
        r_rssk_switch_led_control();
    }
}

void r_rssk_initialize (void)
{
    r_rssk_led2_off();
    r_rssk_led3_off();

    button_status = 0;
    slider_position[0] = TOUCH_OFF_VALUE;
    wheel_position[0] = TOUCH_OFF_VALUE;
}

void r_rssk_led_test (void)
{
    r_rssk_led2_on();
    r_rssk_led3_on();

    r_rssk_touch_led_test();

    R_BSP_SoftwareDelay (1, BSP_DELAY_UNITS_SECONDS);

    r_rssk_led2_off();
    r_rssk_led3_off();

    button_status = 0;
    slider_position[0] = TOUCH_OFF_VALUE;
    wheel_position[0] = TOUCH_OFF_VALUE;
}

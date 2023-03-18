/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
* THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
************************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_rssk_touch_led.c
* Description  : Touch LED control program for Capacitive Touch RSSK System for RA2L1.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 10.02.2023 1.00     First Release
***********************************************************************************************************************/

#include "qe_touch_config.h"
#include "r_rssk_touch_led.h"

/* Port definition */
#define LED_COL_MAX (4)
#define LED_ROW_MAX (4)

#define LED_COL0        (BSP_IO_PORT_05_PIN_04)
#define LED_COL1        (BSP_IO_PORT_04_PIN_00)
#define LED_COL2        (BSP_IO_PORT_07_PIN_14)
#define LED_COL3        (BSP_IO_PORT_04_PIN_06)
#define LED_ROW0        (BSP_IO_PORT_04_PIN_14)
#define LED_ROW1        (BSP_IO_PORT_00_PIN_06)
#define LED_ROW2        (BSP_IO_PORT_07_PIN_08)
#define LED_ROW3        (BSP_IO_PORT_04_PIN_15)

#define LED_COL_OFF     (BSP_IO_LEVEL_LOW)
#define LED_COL_ON      (BSP_IO_LEVEL_HIGH)
#define LED_ROW_OFF     (BSP_IO_LEVEL_HIGH)
#define LED_ROW_ON      (BSP_IO_LEVEL_LOW)

/* Slider LED definition */
#define SLIDER_LED_NUM                  (5U)
#define SLIDER_RESOLUTION               (100)   // Default value is "100" which is defined touch middle-ware

/* Wheel LED definition */
#define WHEEL_LED_NUM                   (8U)
#define WHEEL_LED_MSB                   (1U << (WHEEL_LED_NUM - 1))
#define WHEEL_RESOLUTION_DEGREE         (360)   // Default value is "360" which is defined touch middle-ware
#define WHEEL_POSITION_OFFSET_DEGREE    (112)

/* LED Light Test definition */
#define ALL_LED_NUM                     (16U)
#define LED_TEST_INTERVAL               (100U)

/* Prototypes */
static uint8_t create_led_bitstring_button(uint64_t* button_data);
static uint8_t create_led_bitstring_wheel(uint16_t* wheel_data);
static uint8_t create_led_bitstring_slider(uint16_t* slider_data);

/* Global valuables */
static bsp_io_port_pin_t g_led_column[LED_COL_MAX] = {LED_COL0, LED_COL1, LED_COL2, LED_COL3};
static bsp_io_port_pin_t g_led_row[LED_ROW_MAX] = {LED_ROW0, LED_ROW1, LED_ROW2, LED_ROW3};
static volatile uint8_t g_led_drive_colmun = 0;
#ifndef CONFIG01_NUM_BUTTONS
#define CONFIG01_NUM_BUTTONS (TOUCH_CFG_NUM_BUTTONS + 1)
#endif
static volatile const uint8_t g_button_idx[CONFIG01_NUM_BUTTONS]
                           = {CONFIG01_INDEX_BUTTON00, CONFIG01_INDEX_BUTTON01 ,CONFIG01_INDEX_BUTTON02};

/***********************************************************************************************************************
* Function Name: r_rssk_touch_led_test
* Description  : Touch RSSK LED test at startup
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_rssk_touch_led_test (void)
{
    uint64_t button_dummy = 0;
    uint16_t slider_dummy = TOUCH_OFF_VALUE;
    uint16_t wheel_dummy  = TOUCH_OFF_VALUE;
    uint8_t  i;

    for (i = 0; i < ALL_LED_NUM; i++)
    {
        /* dummy status setting */
        if (i == 0) /* button LED test */
        {
            button_dummy = (uint64_t)(0x01 << CONFIG01_INDEX_BUTTON00);
            slider_dummy = TOUCH_OFF_VALUE;
            wheel_dummy  = TOUCH_OFF_VALUE;
        }
        else if (i == 1) /* button LED test */
        {
            button_dummy = (uint64_t)(0x01 << CONFIG01_INDEX_BUTTON01);
            slider_dummy = TOUCH_OFF_VALUE;
            wheel_dummy  = TOUCH_OFF_VALUE;
        }
        else if (i == 2) /* button LED test */
        {
            button_dummy = (uint64_t)(0x01 << CONFIG01_INDEX_BUTTON02);
            slider_dummy = TOUCH_OFF_VALUE;
            wheel_dummy  = TOUCH_OFF_VALUE;
        }
        else if (i == 3) /* slider LED test */
        {
            button_dummy = 0;
            slider_dummy = SLIDER_RESOLUTION - 1;
            wheel_dummy  = TOUCH_OFF_VALUE;
        }
        else if ((i >= 4 && i < 8)) /* slider LED test */
        {
            button_dummy = 0;
            slider_dummy = (uint16_t)((SLIDER_RESOLUTION / (SLIDER_LED_NUM - 1)) * (7 - i));
            wheel_dummy  = TOUCH_OFF_VALUE;
        }
        else if (i >= 8) /* wheel LED test */
        {
            button_dummy = 0;
            slider_dummy = TOUCH_OFF_VALUE;
            wheel_dummy  = (uint16_t)((WHEEL_RESOLUTION_DEGREE / WHEEL_LED_NUM) * (i - 8));
        }
        else
        {
        }

        /* column setting*/
        if ((i == 0) || ((i > 0) && (i < 4)))
        {
            g_led_drive_colmun = 2;
        }
        else if ((i >= 4) && (i < 8))
        {
            g_led_drive_colmun = 3;
        }
        else if ((i >= 8) && (i < 11))
        {
            g_led_drive_colmun = 0;
        }
        else if ((i >= 11) && (i < 15))
        {
            g_led_drive_colmun = 1;
        }
        else if (i == 15)
        {
            g_led_drive_colmun = 0;
        }
        else
        {
        }

        r_rssk_touch_led_control(&button_dummy, &slider_dummy ,&wheel_dummy);

        R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);
    }
}

/***********************************************************************************************************************
* Function Name: r_rssk_touch_led_control
* Description  : Touch RSSK LED control main
* Arguments    : uint64_t* button : Pointer to button data
*                uint16_t* slider : Pointer to slider data
*                uint16_t* wheel : Pointer to wheel data*
* Return Value : None
***********************************************************************************************************************/
void r_rssk_touch_led_control (uint64_t* button, uint16_t* slider, uint16_t* wheel)
{
    uint8_t led_nibble = 0x00;
    uint8_t i;

    /* Clear LED row data */
    for (i = 0; i < LED_ROW_MAX; i++)
    {
        R_IOPORT_PinWrite(&g_ioport_ctrl, g_led_row[i], LED_ROW_OFF );
    }
    
    /* Clear LED column data */
    for (i = 0; i < LED_COL_MAX; i++)
    {
        R_IOPORT_PinWrite(&g_ioport_ctrl, g_led_column[i], LED_COL_OFF );
    }
    
    /* Create touched electrodes LED data */
    switch (g_led_drive_colmun)
    {
        /* Raise the lower LED bit on the wheel (4/8) */
        case 0:
        {
            led_nibble = create_led_bitstring_wheel(wheel) & 0x0F;
            break;
        }

        /* Raise the higher LED bit on the wheel (4/8) */
        case 1:
        {
            led_nibble = (create_led_bitstring_wheel(wheel) >> 4) & 0x0F;
            break;
        }

        /* Raise the lower LED bit on the slider (4/5) */
        case 3:
        {
            led_nibble = create_led_bitstring_slider(slider) & 0x0F;
            break;
        }

        /* Raise the LED bit on the button & the higher LED bit on the slider (1/5) */
        case 2:
        {
            led_nibble = (uint8_t)(create_led_bitstring_button(button) << 1);
            led_nibble |= (uint8_t)((create_led_bitstring_slider(slider) >> 4) & 0x01);
            break;
        }

        default:
        {
            break;
        }
    }

    /* Set LED row data */
    for (i = 0; i < LED_ROW_MAX; i++)
    {
        if (0x01 == ((led_nibble >> i) & 0x01))
        {
            R_IOPORT_PinWrite(&g_ioport_ctrl, g_led_row[i], LED_ROW_ON);
        }
        else
        {
            R_IOPORT_PinWrite(&g_ioport_ctrl, g_led_row[i], LED_ROW_OFF);
        }
    }

    /* Set LED column data and lights up LED */
    for (i = 0; i < LED_COL_MAX; i++)
    {
        R_IOPORT_PinWrite(&g_ioport_ctrl, g_led_column[g_led_drive_colmun], LED_COL_ON );
    }

    if (LED_COL_MAX > g_led_drive_colmun)
    {
        g_led_drive_colmun++;
    }
    else
    {
        g_led_drive_colmun = 0;
    }
}

/***********************************************************************************************************************
* Function Name: create_led_bitstring_button
* Description  : Create button data bit string for LED
* Arguments    : uint64_t *button_data : Pointer to button data
* Return Value : Button data bit string corresponding to LED
***********************************************************************************************************************/
static uint8_t create_led_bitstring_button (uint64_t *button_data)
{
    uint8_t led_bit = 0x00;
    uint64_t tmp = 0;
    uint8_t i;

    tmp = *button_data;
    
    for (i = CONFIG01_NUM_BUTTONS; i > 0 ; i--)
    {
        if (0x00 != (tmp & (1U << g_button_idx[i - 1])))
        {
            led_bit |= (uint8_t)(0x01 << (i - 1));
        }
        else
        {
            led_bit &= (uint8_t)(~(0x01 << (i - 1)));
        }
    }

    return led_bit;
}

/***********************************************************************************************************************
* Function Name: create_led_bitstring_wheel
* Description  : Create wheel data bit string for LED
* Arguments    : uint16_t *wheel_data : Pointer to wheel data
* Return Value : Wheel data bit string corresponding to LED
***********************************************************************************************************************/
static uint8_t create_led_bitstring_wheel (uint16_t *wheel_data)
{
    const uint16_t led_reso = (WHEEL_RESOLUTION_DEGREE / WHEEL_LED_NUM);
    uint16_t tmp = 0x000;
    uint8_t led_bit = 0x00;
    
    tmp = *wheel_data;
    
    if (TOUCH_OFF_VALUE != tmp)
    {
        if (WHEEL_POSITION_OFFSET_DEGREE > tmp)
        {
            tmp = (uint16_t)((tmp + WHEEL_RESOLUTION_DEGREE) - WHEEL_POSITION_OFFSET_DEGREE);
        }
        else
        {
            tmp = (uint16_t)(tmp - WHEEL_POSITION_OFFSET_DEGREE);
        }

        tmp = (uint8_t)(tmp / led_reso);
        led_bit = (uint8_t)(WHEEL_LED_MSB >> tmp);
    }
    else
    {
        led_bit = 0x00;
    }

    return led_bit;
}

/***********************************************************************************************************************
* Function Name: create_led_bitstring_slider
* Description  : Create slider data bit string for LED
* Arguments    : uint16_t *slider_data : Pointer to slider data
* Return Value : Slider data bit string corresponding to LED
***********************************************************************************************************************/
static uint8_t create_led_bitstring_slider (uint16_t *slider_data)
{
    const uint16_t led_reso = (SLIDER_RESOLUTION / SLIDER_LED_NUM);
    uint16_t tmp = 0x0000;
    uint8_t led_bit = 0x00;

    tmp = *slider_data;

    if (TOUCH_OFF_VALUE > tmp)
    {
        tmp = (uint8_t)(tmp / led_reso);
        led_bit = (uint8_t)(1U << tmp);
    }
    else
    {
        led_bit = 0x00;
    }

    return led_bit;
}

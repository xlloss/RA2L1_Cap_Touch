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
* File Name    : r_rssk_switch_led.c
* Description  : Switch LED control program for Capacitive Touch RSSK System for RA2L1.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 10.02.2023 1.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "qe_touch_config.h"
#include "r_rssk_switch_led.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define RSSK_SW2_PORT       (BSP_IO_PORT_02_PIN_06)
#define RSSK_SW3_PORT       (BSP_IO_PORT_02_PIN_05)
#define RSSK_LED2_PORT      (BSP_IO_PORT_03_PIN_05)
#define RSSK_LED3_PORT      (BSP_IO_PORT_03_PIN_04)

#define SW_EDGE_RIZE        (0x07)    // 0000 0111b
#define SW_EDGE_FALL        (0x08)    // 0000 1000b
#define SW_EDGE_BIT_MASK    (0x0F)

#define RSSK_LED_ON         (1U)
#define RSSK_LED_OFF        (0U)
/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
static uint8_t rssk_get_sw2_status(void);
static uint8_t rssk_get_sw3_status(void);

/***********************************************************************************************************************
* Function Name: r_rssk_switch_led_control
* Description  : Switch LED control main
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_rssk_switch_led_control (void)
{

    if (RSSK_SW_ON == rssk_get_sw2_status())
    {
        r_rssk_led2_on();
    }
    else
    {
        r_rssk_led2_off();
    }

    if (RSSK_SW_ON == rssk_get_sw3_status())
    {
        r_rssk_led3_on();
    }
    else
    {
        r_rssk_led3_off();
    }
}

/***********************************************************************************************************************
* Function Name: rssk_get_sw2_status
* Description  : Get sw2 ON / OFF status
* Arguments    : None
* Return Value : Result of sw2 ON / OFF status
***********************************************************************************************************************/
static uint8_t rssk_get_sw2_status (void)
{
    static volatile uint8_t record = 0x00;
    bsp_io_level_t port_lv;
    static volatile uint8_t result = 0x00;

    R_IOPORT_PinRead (&g_ioport_ctrl, RSSK_SW2_PORT, &port_lv);

    /* remove chattering */
    record = ((record << 1) | (~port_lv & 0x01)) & SW_EDGE_BIT_MASK;

    /* change to 0->1 */
    if (SW_EDGE_RIZE == record)
    {
        result = RSSK_SW_ON;
    }

    /* change to 1->0 */
    if (SW_EDGE_FALL == record)
    {
        result = RSSK_SW_OFF;
    }

    return result;
}

/***********************************************************************************************************************
* Function Name: rssk_get_sw3_status
* Description  : Get sw3 ON / OFF status
* Arguments    : None
* Return Value : Result of sw3 ON / OFF status
***********************************************************************************************************************/
static uint8_t rssk_get_sw3_status (void)
{
    static volatile uint8_t record = 0x00;
    bsp_io_level_t port_lv;
    static volatile uint8_t result = 0x00;

    R_IOPORT_PinRead (&g_ioport_ctrl, RSSK_SW3_PORT, &port_lv);

    /* remove chattering */
    record = ((record << 1) | (~port_lv & 0x01)) & SW_EDGE_BIT_MASK;
    
    /* change to 0->1 */
    if (SW_EDGE_RIZE == record)
    {
        result = RSSK_SW_ON;
    }

    /* change to 1->0 */
    if (SW_EDGE_FALL == record)
    {
        result = RSSK_SW_OFF;
    }

    return result;
}

/***********************************************************************************************************************
* Function Name: r_rssk_led2_on
* Description  : Turn on LED2
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_rssk_led2_on (void)
{
    R_IOPORT_PinWrite(&g_ioport_ctrl, RSSK_LED2_PORT, RSSK_LED_ON);
}

/***********************************************************************************************************************
* Function Name: r_rssk_led2_off
* Description  : Turn off LED2
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_rssk_led2_off (void)
{
    R_IOPORT_PinWrite(&g_ioport_ctrl, RSSK_LED2_PORT, RSSK_LED_OFF);
}

/***********************************************************************************************************************
* Function Name: r_rssk_led3_on
* Description  : Turn on LED3
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_rssk_led3_on (void)
{
    R_IOPORT_PinWrite(&g_ioport_ctrl, RSSK_LED3_PORT, RSSK_LED_ON);
}

/***********************************************************************************************************************
* Function Name: r_rssk_led3_off
* Description  : Turn off LED3
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_rssk_led3_off (void)
{
    R_IOPORT_PinWrite(&g_ioport_ctrl, RSSK_LED3_PORT, RSSK_LED_OFF);
}

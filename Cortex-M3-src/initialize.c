/* ----------------------------------------------------------------------------
 * Copyright (c) 2012 Semiconductor Components Industries, LLC
 * (d/b/a ON Semiconductor). All rights reserved.
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor. The
 * terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * initialize.c
 *  - Initialization source file
 * ----------------------------------------------------------------------------
 * $Revision: 1.14 $
 * $Date: 2012/10/01 17:23:54 $
 * ------------------------------------------------------------------------- */

#include "main.h"

/* ----------------------------------------------------------------------------
 * Function      : void Initialize(void)
 * ----------------------------------------------------------------------------
 * Description   : Performs all system initialization tasks.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Initialize(void)
{
    /* Disable exceptions (except NMI and the hard fault exception) and
     * interrupts before configuring interfaces and peripherals by setting a
     * 1 to the 1-bit interrupt mask register PRIMASK */
    __set_PRIMASK(PRIMASK_DISABLE_INTERRUPTS);

    /* Clear the enable for all of the external interrupts. */
    Sys_NVIC_DisableAllInt();

    /* Clear the pending status for all of the external interrupts. */
    Sys_NVIC_ClearAllPendingInt();

    /* Disable all DMAs */
    Sys_DMA_Channel0_Disable();
    Sys_DMA_Channel1_Disable();
    Sys_DMA_Channel2_Disable();
    Sys_DMA_Channel3_Disable();

    /* Clear the DMA status */
    Sys_DMA_Clear_Channel0_Status();
    Sys_DMA_Clear_Channel1_Status();
    Sys_DMA_Clear_Channel2_Status();
    Sys_DMA_Clear_Channel3_Status();

    /* Disable the ACB */
    ACB->CTRL = ACB_CTRL_DISABLE_VAL;

    /* Clear any ACB state */
    ACB->CMD = ACB_CMD_CLEAR_VAL;

    /* Configure the ACB to enable the G.722 encoder/decoder */
    ACB->CTRL = ACB_CTRL_VAL;

    /* Configure the DMAs */
    Sys_DMA_Channel0_Config(DMA_CH0_CFG_VAL,            \
                            DMA_CH0_TRANSFER_LENGTH,    \
                            DMA_CH0_COUNTER_INT_VAL,    \
                            DMA_CH0_SRC_ADDRESS,        \
                            DMA_CH0_DEST_ADDRESS);

    Sys_DMA_Channel1_Config(DMA_CH1_CFG_VAL,            \
                            DMA_CH1_TRANSFER_LENGTH,    \
                            DMA_CH1_COUNTER_INT_VAL,    \
                            DMA_CH1_SRC_ADDRESS,        \
                            DMA_CH1_DEST_ADDRESS);

    Sys_DMA_Channel2_Config(DMA_CH2_CFG_VAL,            \
                            DMA_CH2_TRANSFER_LENGTH,    \
                            DMA_CH2_COUNTER_INT_VAL,    \
                            DMA_CH2_SRC_ADDRESS,        \
                            DMA_CH2_DEST_ADDRESS);

    /* Enable individual external interrupts */
    NVIC_EnableIRQ(DMA2_IRQn);
    NVIC_EnableIRQ(FIFO_0_IRQn);

    /* Un-mask exceptions and interrupts by setting a 0 to the 1-bit interrupt
     * mask register PRIMASK */
    __set_PRIMASK(PRIMASK_ENABLE_INTERRUPTS);
}

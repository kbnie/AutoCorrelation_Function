/* ----------------------------------------------------------------------------
 * Copyright (c) 2013 Semiconductor Components Industries, LLC
 * (d/b/a ON Semiconductor). All rights reserved.
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor. The
 * terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * main.c
 * - Main source file
 * ----------------------------------------------------------------------------
 * $Revision: 1.20 $
 * $Date: 2013/02/15 20:32:43 $
 * ------------------------------------------------------------------------- */

#include "main.h"

/* ----------------------------------------------------------------------------
 * Function      : void DMA2_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : Handler for the DMA 2 interrupt. When this interrupt occurs,
 *                 the G.722 encoder/decoder and all three DMA channels have
 *                 completed for the current block of FIFO data.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : DMA channels 0, 1 and 2 have been configured. The G.722
 *                 encoder and decoder have been configured and enabled.
 * ------------------------------------------------------------------------- */
void DMA2_IRQHandler(void)
{

    /* Clear the status for all three DMA channels, so that they are ready for
     * the next block of FIFO data */
    Sys_DMA_Clear_Channel0_Status();
    Sys_DMA_Clear_Channel1_Status();
    Sys_DMA_Clear_Channel2_Status();

    /* Send command 0 to CFX */
    Sys_CFX_Command(CFX_CMD_0);
}

/* ----------------------------------------------------------------------------
 * Function      : void FIFO_0_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : Handler for FIFO 0 interrupt. When FIFO 0 interrupt occurs,
 *                 re-arrange and copy FIFO A0_1 data to a buffer, and enable
 *                 DMA transfers.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : DMA channels 0, 1 and 2 have been configured. The G.722
 *                 encoder and decoder have been configured and enabled.
 * ------------------------------------------------------------------------- */
void FIFO_0_IRQHandler(void)
{

    /* Enable the DMAs. This is necessary every time a new block of FIFO data
     * has arrived, since all three DMA channels operate in linear mode. */
    Sys_DMA_Channel0_Enable();
    Sys_DMA_Channel1_Enable();
    Sys_DMA_Channel2_Enable();

}

/* ----------------------------------------------------------------------------
 * Function      : int main(void)
 * ----------------------------------------------------------------------------
 * Description   : Initializes the system, and start an infinite while loop.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int main(void)
{
    /* Call the system initialization function */
    Initialize();

    /* The FIFO 0 and DMA 2 interrupt service routines will facilitate moving
     * the audio data through the G.722 encoder and decoder, and finally to the
     * output FIFO. */
    while (1)
    {
    }

    return 0;   /* Not reached; main() loops forever */
}


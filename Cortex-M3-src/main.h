/* ----------------------------------------------------------------------------
 * Copyright (c) 2013 Semiconductor Components Industries, LLC
 * (d/b/a ON Semiconductor). All rights reserved.
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor. The
 * terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * main.h
 * - Main application-level include file
 * ----------------------------------------------------------------------------
 * $Revision: 1.20 $
 * $Date: 2013/02/15 20:32:43 $
 * ------------------------------------------------------------------------- */

#ifndef MAIN_H
#define MAIN_H

#include <sk4.h>

/* ----------------------------------------------------------------------------
 * Application configuration
 * ------------------------------------------------------------------------- */

/* Size of the stack in bytes */
#define STACK_SIZE                      1024

/* FIFO block size in words. This must match the FIFO block size defined in the
 * CFX part of the application. */
#define FIFO_BLOCK_SIZE                 8

/* FIFO A0_1 address offset (in words) relative to A0 memory base. This must
 * match the define in the CFX part of the application. */
#define FIFO_A0_1_BASE                  160

/* FIFO B0_1 address offset (in words) relative to B0 memory base. This must
 * match the define in the CFX part of the application. */
#define FIFO_B0_1_BASE                  96

/* ----------------------------------------------------------------------------
 * ACB configuration
 * ------------------------------------------------------------------------- */
/* ACB control setting used to disable the codecs during hardware
 * initialization */
#define ACB_CTRL_DISABLE_VAL            (G722_DEC_DISABLE               | \
                                         G722_ENC_DISABLE               | \
                                         CVSD_DEC_DISABLE               | \
                                         CVSD_ENC_DISABLE)

/* Audio Coding Block control: enable G.722 encoder and decoder, set data
 * format as 16-bit (m1p15), enable encoder filterbank and decoder filterbank,
 * and disable decoder concealment mode.
 * The CVSD codec is disabled and its parameters remain at default values. */
#define ACB_CTRL_VAL                    (G722_DEC_ENABLE                | \
                                         G722_DEC_OUT_16BIT             | \
                                         G722_DEC_FB_ENABLE             | \
                                         G722_CONCL_DISABLE             | \
                                         G722_ENC_ENABLE                | \
                                         G722_ENC_IN_16BIT              | \
                                         G722_ENC_FB_ENABLE             | \
                                         CVSD_DEC_H_5                   | \
                                         CVSD_DEC_BETA_10               | \
                                         CVSD_DEC_J_4                   | \
                                         CVSD_DEC_DISABLE               | \
                                         CVSD_ENC_H_5                   | \
                                         CVSD_ENC_BETA_10               | \
                                         CVSD_ENC_J_4                   | \
                                         CVSD_ENC_DISABLE)

/* ACB command setting used to clear all codec states */
#define ACB_CMD_CLEAR_VAL               (G722_DEC_SBBND_CLEAR           | \
                                         G722_DEC_FB_CLEAR              | \
                                         G722_DEC_LSB_CNCL_CLEAR        | \
                                         G722_ENC_SBBND_CLEAR           | \
                                         G722_ENC_FB_CLEAR              | \
                                         CVSD_DEC_CORE_CLEAR            | \
                                         CVSD_DEC_POSTFILT_CLEAR        | \
                                         CVSD_ENC_CORE_CLEAR            | \
                                         CVSD_ENC_PREFILT_CLEAR)

/* ----------------------------------------------------------------------------
 * DMA configuration
 * ------------------------------------------------------------------------- */

/* DMA 0 configuration M-P (Memory to Peripheral), where the memory is the most
 * recent block in FIFO A0_1, and the peripheral is the hardware encoder input
 * of the ACB.
 * Byte ordering is little endian.
 * Source word size is 16 bits and destination word sizes is 32 bits. Two
 * 16-bit audio samples are pack into one 32-bit word at the source of DMA 0
 * transfer, and four 8-bit code words are packed into one 32-bit word at the
 * destination of DMA 0 transfer. The transfer length is half of the FIFO block
 * size.
 * The source address increments by a step size of -1 and the destination
 * address is static. Addressing mode of the transfer is set to be linear. In
 * this mode, each time this DMA channel is enabled, it transfers one buffer of
 * data of size DMA_CH0_TRANSFER_LENGTH.
 * All DMA 0 interrupt conditions are disabled. */
#define DMA_CH0_CFG_VAL                 (DMA_CH0_DEST_ADDR_STEP_SIZE_1  | \
                                         DMA_CH0_SRC_ADDR_STEP_SIZE_1   | \
                                         DMA_CH0_DEST_ADDR_POS          | \
                                         DMA_CH0_SRC_ADDR_NEG           | \
                                         DMA_CH0_LITTLE_ENDIAN          | \
                                         DMA_CH0_DISABLE_INT_DISABLE    | \
                                         DMA_CH0_ERROR_INT_DISABLE      | \
                                         DMA_CH0_COMPLETE_INT_DISABLE   | \
                                         DMA_CH0_COUNTER_INT_DISABLE    | \
                                         DMA_CH0_START_INT_DISABLE      | \
                                         DMA_CH0_DEST_WORD_SIZE_32      | \
                                         DMA_CH0_SRC_WORD_SIZE_16       | \
                                         DMA_CH0_TRANSFER_M_TO_P        | \
                                         DMA_CH0_DEST_ADDR_STATIC       | \
                                         DMA_CH0_DEST_G722_ENC          | \
                                         DMA_CH0_SRC_ADDR_INC           | \
                                         DMA_CH0_PRIORITY_0             | \
                                         DMA_CH0_ADDR_LIN               | \
                                         DMA_CH0_DISABLE)

#define DMA_CH0_SRC_ADDRESS             (HEAR_A0MEM_LSB_16MSB_BASE      + \
                                         (FIFO_A0_1_BASE + FIFO_BLOCK_SIZE - 1)*4)

#define DMA_CH0_TRANSFER_LENGTH         (FIFO_BLOCK_SIZE/2)
#define DMA_CH0_DEST_ADDRESS            ((uint32_t) &ACB->G722_ENC_IN)


/* Since DMA channel 0 counter interrupt is disabled, the counter interrupt
 * value does not matter and is left at the default value 0 */
#define DMA_CH0_COUNTER_INT_VAL          0

/* DMA 1 configuration P-P (Peripheral 1 to Peripheral 2), where peripheral 1
 * is encoder output and peripheral 2 is decoder input.
 * Byte ordering is little endian.
 * Source and destination word sizes are both 8 bits and the transfer length
 * is half of the FIFO block size.
 * The source and destination addresses are both static.
 * Addressing mode of the transfer is set to be linear. In this mode, each time
 * this DMA channel is enabled, it transfers one buffer of data of size
 * DMA_CH1_TRANSFER_LENGTH.
 * All DMA 1 interrupt conditions are disabled. */
#define DMA_CH1_CFG_VAL                 (DMA_CH1_LITTLE_ENDIAN          | \
                                         DMA_CH1_DISABLE_INT_DISABLE    | \
                                         DMA_CH1_ERROR_INT_DISABLE      | \
                                         DMA_CH1_COMPLETE_INT_DISABLE   | \
                                         DMA_CH1_COUNTER_INT_DISABLE    | \
                                         DMA_CH1_START_INT_DISABLE      | \
                                         DMA_CH1_DEST_WORD_SIZE_8       | \
                                         DMA_CH1_SRC_WORD_SIZE_8        | \
                                         DMA_CH1_PRIORITY_0             | \
                                         DMA_CH1_SRC_ADDR_STATIC        | \
                                         DMA_CH1_DEST_ADDR_STATIC       | \
                                         DMA_CH1_SRC_G722_ENC           | \
                                         DMA_CH1_DEST_G722_DEC          | \
                                         DMA_CH1_ADDR_LIN               | \
                                         DMA_CH1_TRANSFER_P_TO_P        | \
                                         DMA_CH1_DISABLE)

#define DMA_CH1_SRC_ADDRESS             ((uint32_t) &ACB->G722_ENC_OUT)
#define DMA_CH1_DEST_ADDRESS            ((uint32_t) &ACB->G722_DEC_IN)
#define DMA_CH1_TRANSFER_LENGTH         (FIFO_BLOCK_SIZE/2)

/* Since DMA channel 1 counter interrupt is disabled, the counter interrupt
 * value does not matter and is left at the default value 0 */
#define DMA_CH1_COUNTER_INT_VAL          0

/* DMA 2 configuration P-M (Peripheral to Memory), where the peripheral is
 * the hardware decoder output of the ACB, and the memory is the most recent
 * block in FIFO B0_1.
 * Byte ordering is little endian.
 * Source word size is 32 bits and destination word sizes is 16 bits. For every
 * two 16-bit samples packed into one 32-bit word at the source of DMA 2
 * transfer, two 16-bit samples are written to FIFO B0_1 at the destination of
 * DMA 2 transfer via a memory area where the left-aligned FIFO is mapped to
 * the 16 LSBs. The transfer length is half of the FIFO block size.
 * The source address is static and the destination address increments by a
 * step size of -1. Addressing mode of the transfer is set to be linear. In
 * this mode, each time this DMA channel is enabled, it transfers one buffer of
 * data of size DMA_CH2_TRANSFER_LENGTH.
 * When DMA 2 transfer completes, an interrupt will be raised. Other interrupt
 * conditions are disabled.*/
#define DMA_CH2_CFG_VAL                 (DMA_CH2_DEST_ADDR_STEP_SIZE_1  | \
                                         DMA_CH2_SRC_ADDR_STEP_SIZE_1   | \
                                         DMA_CH2_DEST_ADDR_NEG          | \
                                         DMA_CH2_SRC_ADDR_POS           | \
                                         DMA_CH2_LITTLE_ENDIAN          | \
                                         DMA_CH2_DISABLE_INT_DISABLE    | \
                                         DMA_CH2_ERROR_INT_DISABLE      | \
                                         DMA_CH2_COMPLETE_INT_ENABLE    | \
                                         DMA_CH2_COUNTER_INT_DISABLE    | \
                                         DMA_CH2_START_INT_DISABLE      | \
                                         DMA_CH2_DEST_WORD_SIZE_16      | \
                                         DMA_CH2_SRC_WORD_SIZE_32       | \
                                         DMA_CH2_TRANSFER_P_TO_M        | \
                                         DMA_CH2_DEST_ADDR_INC          | \
                                         DMA_CH2_SRC_G722_DEC           | \
                                         DMA_CH2_SRC_ADDR_STATIC        | \
                                         DMA_CH2_PRIORITY_0             | \
                                         DMA_CH2_ADDR_LIN               | \
                                         DMA_CH2_DISABLE)

#define DMA_CH2_DEST_ADDRESS            (HEAR_B0MEM_LSB_16MSB_BASE      + \
                                         (FIFO_B0_1_BASE + FIFO_BLOCK_SIZE - 1)*4)

#define DMA_CH2_SRC_ADDRESS             ((uint32_t) &ACB->G722_DEC_OUT)

#define DMA_CH2_TRANSFER_LENGTH         (FIFO_BLOCK_SIZE/2)

/* Since DMA channel 2 counter interrupt is disabled, the counter interrupt
 * value does not matter and is left at the default value 0 */
#define DMA_CH2_COUNTER_INT_VAL          0

/* ----------------------------------------------------------------------------
 * Prototypes for global functions
 * ------------------------------------------------------------------------- */
extern void Initialize(void);

#endif /* MAIN_H */

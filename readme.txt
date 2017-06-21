===============================================================================
 All-Core Passthrough
===============================================================================

This sample application implements a dual-channel audio passthrough that
engages all four cores (CFX, HEAR, Filter Engine, and ARM Cortex-M3
processor) of Ezairo 7100.

The signal chains are configured as follows:

Channel 0: The ADC0 output is captured by Filter Engine input 0. A 1-stage
biquad filter (an identity filter in this example) is applied to this signal,
with the filtered output routed to Filter Engine output 0. The output 0 data
from the Filter Engine is buffered in FIFO A0_0 via the IOC (see the
configuration for audio multiplexing) and processed by the HEAR in a mono
filterbank passthrough. The synthesized signal is stored in FIFO B0_0, and
output to DAC0.

Channel 1: The ADC2 output is buffered in FIFO A0_1 via the IOC and goes
through the G.722 encoder and decoder on the ARM Cortex-M3 processor subsystem.
The decoded data is stored in FIFO B0_1, which is routed to the Filter Engine
input 5. A 1-stage biquad filter (a 1st-order low-pass filter to attenuate
out-of-band components in the G.722 decoder output) is applied to this signal,
with the filtered result routed to Filter Engine output 5 and interpolation
filter 1 (see the configuration for audio multiplexing).

-------------------------------------------------------------------------------
Connecting to the hardware:
-------------------------------------------------------------------------------
In this application two input audio sources are expected at AI0 (channel 0) and
AI2 (channel 1) pins on the Ezairo 7100 EDK board. To observe the output,
connect your listening or recording device to OD0 (channel 0) and OD1
(channel 1).

-------------------------------------------------------------------------------
The following steps are performed in every new frame on the CFX and HEAR:
-------------------------------------------------------------------------------

1) An interrupt (FIFO 0) is generated on the CFX when a block of data is
   transferred from mono FIFO B0_0 to DAC0. Note that in this application,
   FIFO B0_0 is configured as the source of the CFX FIFO 0 interrupt. This
   interrupt is used to synchronize processing on the CFX and HEAR, since it is
   associated with the last scheduled data transfer by the IOC. When the CFX
   processes this interrupt it branches to the FIFO_0_done label. The new data
   in the input FIFO is available at HCT_FIFO_A0_0_START in A0MEM.

2) The FIFO_0_done branch launches function_chain0 on the HEAR. This starts
   the analysis function, which reads data from the FIFO A0_0 and generates
   complex results at location (D_HEAR_H01MEM_BASE + WOLA_RESULTS_OFFSET).

3) The HEAR generates an interrupt (HEAR_0) when the analysis function
   completes. When the CFX processes this interrupt it branches to the
   HEAR_function_chain0_done label.

4) The HEAR_function_chain0_done branch launches function_chain1 on the HEAR.
   This begins the gain application, which multiplies gain values from
   (D_HEAR_H45MEM_BASE + GAIN_TABLE_OFFSET) by the spectrum located at
   (D_HEAR_H01MEM_BASE + WOLA_RESULTS_OFFSET) and stores the results back to
   (D_HEAR_H01MEM_BASE + WOLA_RESULTS_OFFSET). The HEAR then generates an
   interrupt (HEAR_1) when the gain application function completes.
   When the CFX processes this interrupt it branches to the
   HEAR_function_chain1_done label.

5) On the CFX, the HEAR_function_chain1_done branch launches function_chain2
   on the HEAR. This starts the synthesis function, which reads the spectrum
   from (D_HEAR_H01MEM_BASE + WOLA_RESULTS_OFFSET) and makes the results
   available in the mono output FIFO B0_0 at HCT_FIFO_B0_0_START in B0MEM.

6) The HEAR generates an interrupt (HEAR_2) when the synthesis function
   completes. When the CFX processes this interrupt it branches to the
   HEAR_function_chain2_done label.

The WOLA and FIFO configuration parameter definitions can be found in
microcode.hct.

Note:  In this example, no new gains are calculated; the code initializes the
       gain table with unity gain values and does not update them at any time.
       The gain application function is included to demonstrate how it would
       fit into the signal path of such an application, if desired.

-------------------------------------------------------------------------------
The following steps are performed in every new frame on the ARM Cortex-M3
processor:
-------------------------------------------------------------------------------

1) An interrupt (FIFO 0) is generated on the ARM Cortex-M3 processor when a
   block of data is transferred from mono FIFO B0_0 to DAC0. Note that in this
   application, FIFO B0_0 is configured as the source of the FIFO 0 interrupt
   on the ARM Cortex-M3 processor. This interrupt is used to synchronize
   processing on the ARM Cortex-M3 processor, since it is associated with the
   last scheduled data transfer by the IOC. When the core processes this
   interrupt it enables DMA transfers.

2) DMA0 transfers data from memory (a new block of data in FIFO A0_1) to
   peripheral (G.722 encoder input). Note that in the DMA configuration for
   this transfer, the addressing for FIFO A0_1 has a negative step size because
   in the FIFO the most recent data is located at the lowest address.

3) G.722 encoder encodes the most recent samples at its input.

4) DMA1 transfers data from peripheral (G.722 encoder output) to peripheral
   (G.722 decoder input).

5) G.722 decoder decodes the most recent data at its input.

6) DMA2 transfers data from peripheral (G.722 decoder output) to memory (FIFO
   B0_1). Note that in the DMA configuration for this transfer, the addressing
   for FIFO B0_1 has a negative step size because in the FIFO the most recent
   data is located at the lowest address. 

7) A DMA2 interrupt is generated on the ARM Cortex-M3 processor when DMA
   channel 2 has transferred the output of the G.722 decoder to FIFO B0_1.
   The status of all three DMA channels is cleared to prepare for the next
   block of data. This is necessary because these DMA channels are configured
   in linear mode, and must be restarted from a good state before transferring
   a new block of data. The ARM Cortex-M3 processor then sends an interrupt to
   the CFX, indicating that the codec has completed processing for this block.

-------------------------------------------------------------------------------
The following steps are performed on every new sample on the Filter Engine:
-------------------------------------------------------------------------------

The Filter Engine processing is initiated at every new sample. In this
application, inputs f_in0 (ADC0) and f_in5 (FIFO B0_1) are enabled on the
Filter Engine. Filter Engine processing is triggered on the arrival of each
f_in5 sample. A 1-stage biquad filter is applied to the f_in0 data, with the
filtered output routed to f_out0 (IOC source 0, connected to FIFO A0_0).
Another 1-stage biquad filter is applied to the f_in5 data, with the filtered
output routed to f_out5 (interpolation filter 1).

The Filter Engine source code can be found in feng.fs. The Filter Engine is
configured by the CFX during application initialization (see app_init.cfs).
Filter Engine filter coefficients are defined in app_memory.cfs. The biquad
filter with coefficient set C0 is an identity filter, included to demonstrate
how the Filter Engine audio pre-processing fits into the signal path. The
biquad filter with coefficient set C1 is a 1st-order low-pass filter to
attenuate out-of-band components in the G.722 decoder output.


For more information, see the comments in the code.
The code files are organized as follows:

-  app.cfs          : main file, containing the main loop
-  app.cfi          : definitions
-  app_memory.cfs   : variable declarations, CFX stack initialization
-  app_init.cfs     : application initialization routine
-  app_isr.cfs      : interrupt vector table and CFX interrupt service routines
-  microcode.hct    : HCT configuration file, which contains the WOLA and FIFO
                      configuration parameters
-  feng.fs          : Filter Engine source code
-  main.c           : ARM Cortex-M3 processor main source code
-  main.h           : ARM Cortex-M3 processor application header file
-  memory.c         : ARM Cortex-M3 processor memory allocation
-  initialize.c     : ARM Cortex-M3 processor initialization routine

===============================================================================
Copyright (c) 2013 Semiconductor Components Industries, LLC
(d/b/a ON Semiconductor).

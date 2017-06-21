// ----------------------------------------------------------------------------
// Copyright (c) 2013 Semiconductor Components Industries, LLC
// (d/b/a ON Semiconductor). All Rights Reserved.
//
// This code is the property of ON Semiconductor and may not be redistributed
// in any form without prior written permission from ON Semiconductor. The
// terms of use and warranty for this code are covered by contractual
// agreements between ON Semiconductor and the licensee.
//-----------------------------------------------------------------------------
// feng.fs
// - Filter Engine program source
//-----------------------------------------------------------------------------
// $Revision: 1.6 $
// $Date: 2013/04/12 15:33:09 $
//-----------------------------------------------------------------------------

// Apply biquad filter with coefficient set C0 on input 0
root:       biq out0, in0, c0, sv0, rnd
// Apply biquad filter with coefficient set C1 on input 5
            biq out5, in5, c1, sv1, rnd

/*
 *                  Copyright (c), NXP Semiconductors
 *
 *                     (C)NXP Semiconductors
 *        All rights are reserved. Reproduction in whole or in part is
 *       prohibited without the written consent of the copyright owner.
 *   NXP reserves the right to make changes without notice at any time.
 *  NXP makes no warranty, expressed, implied or statutory, including but
 * not limited to any implied warranty of merchantability or fitness for any
 * particular purpose, or that the use will not infringe any third party patent,
 *  copyright or trademark. NXP must not be liable for any loss or damage
 *                          arising from its use.
 */

/**
 * \file tm_Status.h
 *
 * $Author: KARTHIK RAMACHANDRA (nxp84320) $
 * $Revision: 18627 $ (v04.13.00)
 * $Date: 2016-09-01 15:35:17 +0530 (Thu, 01 Sep 2016) $
 *
 */

#ifndef TM_STATUS_H_
#define TM_STATUS_H_

#ifndef PH_DATATYPES_H
#   include <ph_Datatypes.h>
#endif

/** Status of a given test case */
typedef enum tmStatus {
    /** The test case has paased */
    TM_STATUS_SUCCESS = 'p',
    /** The test case has failed
     *
     * It is suggested that return value of a test case
     * is intialized with this value first.
     */
    TM_STATUS_FAILED = 'f',
    /** This test case is not supposed to be run in this environment.
     *
     * Ignore the result and mark as a Ignored test case.
     *
     * Map to ASSERT:IGNORE of NUnit.
     */
    TM_STATUS_IGNORED = 'i',
    /** This test case is not yet implemented.
     *
     * It is suggested that return value of an empty test case
     * is intialized with this value first.
     */
    TM_STATUS_NOTIMPLEMENTED = 'n',
    /** Status for framework
     *
     * This return status generally suggests that the requested
     * test case ID is not part of this downloaded binary
     * and hence a new Firmware download would be required.
     */
    TM_STATUS_NOSUCHTC = '0',
    /** Necessary pre conditions to run this test case have
     * not been met.
     *
     * This status triggers a case where the test environment
     * itself is not up to date, or environment did not get
     * ready in required time.
     */
    TM_STATUS_PRE_CONDITION_FAILED = 'd',
    /** Necessary post conditions to run this test case have
     * not been met.
     *
     * This status triggers a case where the test environment
     * itself is not up to date, or environment did not get
     * get cleaned up in required time.
     */
    TM_STATUS_POST_CONDITION_FAILED = 't',
    /** This test was never run, hence not possible to get status.
     *
     * this can be a case where we are asked for get test status without
     * running the test case, or the IC has reset itself between running
     * the test case and getting the status.
     */
    TM_STATUS_NEVER_RUN = 'v',

    /* Promoting tmStatus_t to 16 bits. This is necessary for
     * alignmnet with UTF whose test cases, unfortunately, use phStatus_t
     */
    TM_U16 = 0x7FFF
} tmStatus_t;

typedef struct
{
    uint8_t *aCmdParameter;
    uint16_t wCmdParameterLength;
    uint16_t wRspLength;
    uint8_t *aRsp;
} tmTestCtx_t;


#define PEEK_U32(ctx,offset) \
	((ctx->wCmdParameterLength) >= (4 + offset)) ? (*(uint32_t*)&ctx->aCmdParameter[offset]) : 0

#define PEEK_U16(ctx,offset) \
	((ctx->wCmdParameterLength) >= (2 + offset)) ? (*(uint16_t*)&ctx->aCmdParameter[offset]) : 0

#define PEEK_U8(ctx,offset) \
	((ctx->wCmdParameterLength) >= (1 + offset)) ? (*(uint8_t*)&ctx->aCmdParameter[offset]) : 0

#define PUSH_U32(ctx,u32) do {\
        (*(uint32_t*)&ctx->aRsp[ctx->wRspLength]) = u32;\
        ctx->wRspLength+=4; \
    } while (0)

#define PUSH_U16(ctx,u32) do {\
        (*(uint16_t*)&ctx->aRsp[ctx->wRspLength]) = u32;\
        ctx->wRspLength+=4; \
    } while (0)

#define PUSH_AU8(ctx,au8,lenAu8) do {\
    phUser_MemCpy(&ctx->aRsp[ctx->wRspLength],au8,lenAu8);\
        ctx->wRspLength+=lenAu8; \
    } while (0)

typedef tmStatus_t (*tmTestCase_t) (tmTestCtx_t * inout_TcContext);

typedef tmStatus_t (*tmSetupTearDown_t)(void);

typedef tmTestCase_t (*tmTestCasegroup_t) (
        uint32_t in_TestId,
        tmSetupTearDown_t * out_Setup,
        tmSetupTearDown_t * out_TearDown);

#endif /* TM_STATUS_H_ */

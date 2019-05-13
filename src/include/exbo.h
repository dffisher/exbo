/******************************************************************************
 ******************************************************************************
 ***                                                                        ***
 ***  MIT License                                                           ***
 ***                                                                        ***
 ***  Copyright 2016,2018 Daniel F. Fisher                                  ***
 ***                                                                        ***
 ***  Permission is hereby granted, free of charge, to any person           ***
 ***  obtaining a copy of this software and associated documentation files  ***
 ***  (the "Software"), to deal in the Software without restriction,        ***
 ***  including without limitation the rights to use, copy, modify, merge,  ***
 ***  publish, distribute, sublicense, and/or sell copies of the Software,  ***
 ***  and to permit persons to whom the Software is furnished to do so,     ***
 ***  subject to the following conditions:                                  ***
 ***                                                                        ***
 ***  The above copyright notice and this permission notice shall be        ***
 ***  included in all copies or substantial portions of the Software.       ***
 ***                                                                        ***
 ***  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       ***
 ***  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    ***
 ***  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                 ***
 ***  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS   ***
 ***  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN    ***
 ***  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN     ***
 ***  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE      ***
 ***  SOFTWARE.                                                             ***
 ***                                                                        ***
 ******************************************************************************
 ******************************************************************************/

#pragma once
#ifndef included_exbo_exbo_h
#define included_exbo_exbo_h

/*********************************
 * header file inclusions
 *********************************/
#include <stddef.h>
#include <inttypes.h>

/*********************************
 * Open C++ support
 *********************************/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*********************************
 * external macro declarations
 *********************************/
/* Error Numbers */
#define ExboErr_NoInstance               (1) // "No instance was provided"
#define ExboErr_NoConfig                 (2) // "BUG: there is no configuration structure"
#define ExboErr_RecordingAPriorAttempt   (3) // "A later attempt was already recorded"
#define ExboErr_NextTimeOverflow         (4) // "The next time overflowed"
#define ExboErr_StateWithNegativeI       (5) // "BUG: state has negative I"
#define ExboErr_PayBackTimeOverflow      (6) // "The payback time overflowed"
#define ExboErr_StateWithNegativeD       (7) // "BUG: state has negative D"
#define ExboErr_InvalidConfig_X1         (8) // "The given X is not a finite real number"
#define ExboErr_InvalidConfig_X2         (9) // "The given X is not greater than 1"
#define ExboErr_InvalidConfig_A1        (10) // "The given A is not positive"
#define ExboErr_InvalidConfig_L1        (11) // "The given L is not positive"
#define ExboErr_InvalidConfig_L2        (12) // "The given L is less than the given A"
#define ExboErr_ConfigValueNotSet       (13) // "The requested configuration value is not set"
#define ExboErr_InternalError_1         (14) // "BUG: the finished config is not marked as finished"
#define ExboErr_InternalError_2         (15) // "BUG: the finished config has missing parts"
#define ExboErr_InternalError_3         (16) // "BUG: the finished config is invalid"
#define ExboErr_MAXIMUM                 (64)

/* Minimum Time Value */
#define Exbo_MinimumTime (INT64_MIN + ExboErr_MAXIMUM)

/* Warning Return Codes */
#define ExboWarn_AttemptIsEarlierThanRecommended         (-1) // "This attempt is earlier than was recommended"
#define ExboWarn_ExcessCostLimitBreach                   (-2) // "Excess cost limit breach"
#define ExboWarn_ExcessCostLimitBreachWithDebtOverflow   (-3) // "Excess cost limit breach with debt accumulator overflow"
#define ExboWarn_COUNT                                    (4)

/*********************************
 * external struct, union,
 * typedef and enum declarations
 *********************************/
typedef void *exbo;

/*********************************
 * external data declarations
 *********************************/

/*********************************
 * external function declarations
 *********************************/
extern exbo exboCreate(void);

extern exbo exboCreateConfigured(double X, int64_t A, int64_t L);

extern void exboDestroy(exbo xp);

extern int exboClearConfig(exbo xp);

extern int exboConfigure_X(exbo xp, double X);

extern int exboConfigure_A(exbo xp, int64_t A);

extern int exboConfigure_L(exbo xp, int64_t L);

extern int exboValidateConfig(exbo xp);

extern int exboFinishConfig(exbo xp);

extern int exboRecordAttempt(exbo xp, int64_t time);

/* To signal an error, this function returns a value that is less
 * than Exbo_MinimumTime, which equals INT64_MIN + ExboErr_MAXIMUM.
 */
extern int64_t exboGetPreviousAttemptTime(exbo xp);

/* To signal an error, this function returns a value that is less
 * than Exbo_MinimumTime, which equals INT64_MIN + ExboErr_MAXIMUM.
 */
extern int64_t exboGetNextAttemptTime(exbo xp);

/* To signal an error, this function returns a value that is less
 * than Exbo_MinimumTime, which equals INT64_MIN + ExboErr_MAXIMUM.
 */
extern int64_t exboGetPayBackTime(exbo xp);

extern int exboIsConfigFinished(exbo xp); 

extern int exboIsConfigValidated(exbo xp); 

extern int exboDoesConfigHave_X(exbo xp); 

extern int exboDoesConfigHave_A(exbo xp); 

extern int exboDoesConfigHave_L(exbo xp); 

extern double exboGetConfig_X(exbo xp); 

extern int64_t exboGetConfig_A(exbo xp); 

extern int64_t exboGetConfig_L(exbo xp); 

extern const char *exboGetNanErrorMessage(double nanErrorNumber);

extern const char *exboGetTimeErrorMessage(int64_t timeErrorNumber);

extern const char *exboGetErrorMessage(int errorNumber);

/********************************r
 * Close C++ support
 *********************************/
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* included_exbo_exbo_h */
/*********************************
 * The End
 *********************************/

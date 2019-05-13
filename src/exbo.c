/******************************************************************************
 ******************************************************************************
 ***                                                                        ***
 ***  MIT License                                                           ***
 ***                                                                        ***
 ***  Copyright (c) 2016,2018 Daniel F. Fisher                              ***
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

/*********************************
 * header file inclusions
 *********************************/
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <exbo.h>

/*********************************
 * internal macro declarations
 *********************************/

/* Configuration Defaults */
#define ONE_SECOND ((int64_t)1000)
#define DEFAULT_X ((double)2.0)
#define DEFAULT_A ((int64_t)(60 * ONE_SECOND))
#define DEFAULT_L_OVER_A ((int64_t)6)

/* Internal Error Code Constants */
#define NanTag_ExboErr_NoInstance        "1"
#define NanTag_ExboErr_NoConfig          "2"
#define NanTag_ExboErr_ConfigValueNotSet "13"


/*********************************
 * internal struct, union,
 * typedef and enum declarations
 *********************************/
struct config {
    int isFinished;
    int isValid;
    int has_X;
    int has_A;
    int has_L;
    double X;
    int64_t A;
    int64_t L;
};

struct instance {
    int64_t T;
    int64_t D;
    int64_t I;
    struct config *config;
};

/*********************************
 * internal data declarations
 *********************************/

/*********************************
 * internal function declarations
 *********************************/
static struct instance *zInstanceCreate(void);
static void zInstanceDestroy(struct instance *p);
static struct config *zConfigCreate(void);
static void zConfigDestroy(struct config *p);
static void zConfigInit(struct config *p);
static int zConfigValidate(struct config *p);
static int zConfigRevalidate(struct config *p);
static int zConfigValidate_X(struct config *p);
static int zConfigValidate_A(struct config *p);
static int zConfigValidate_L(struct config *p);
static int zConfigFinish(struct config *p);
static int zConfigSetDefaults(struct config *p);
static int zSetDefault_X(struct config *p);
static int zSetDefault_A(struct config *p);
static int zSetDefault_L(struct config *p);
static int zValidateFinish(struct config *p);
static int zInterval(int64_t L, int64_t A, double X, int64_t D, int64_t *Ip);
static int z_J(double l, double X, double *jp);
static int z_m(double j, double x, double *vp);

/*********************************
 * external data definitions
 *********************************/

/*********************************
 * internal data definitions
 *********************************/
static const char *zErrMessageNotError = "Exbo_MinimumTime (== INT64_MIN + 64) and above are valid times";
static const char *zErrMessageNegative = "Negative error number is undefined";
static const char *zErrMessageMaximum = "ExboErr_MAXIMUM (==64) and above are undefined errors";
static const char *zErrMessages[ExboErr_MAXIMUM] = {
    "No Error",
    "No instance was provided",                                   // ExboErr_NoInstance               (1)
    "BUG: there is no configuration structure",                   // ExboErr_NoConfig                 (2)
    "A later attempt was already recorded",                       // ExboErr_RecordingAPriorAttempt   (3)
    "The next time overflowed",                                   // ExboErr_NextTimeOverflow         (4)
    "BUG: state has negative I",                                  // ExboErr_StateWithNegativeI       (5)
    "The payback time overflowed",                                // ExboErr_PayBackTimeOverflow      (6)
    "BUG: state has negative D",                                  // ExboErr_StateWithNegativeD       (7)
    "The given X is not a finite real number",                    // ExboErr_InvalidConfig_X1         (8)
    "The given X is not greater than 1",                          // ExboErr_InvalidConfig_X2         (9)
    "The given A is not positive",                                // ExboErr_InvalidConfig_A1        (10)
    "The given L is not positive",                                // ExboErr_InvalidConfig_L1        (11)
    "The given L is less than the given A",                       // ExboErr_InvalidConfig_L2        (12)
    "The requested configuration value is not set",               // ExboErr_ConfigValueNotSet       (13)
    "BUG: the finished config is not marked as finished",         // ExboErr_InternalError_1         (14)
    "BUG: the finished config has missing parts",                 // ExboErr_InternalError_2         (15)
    "BUG: the finished config is invalid",                        // ExboErr_InternalError_3         (16)
    "Error 17 is undefined",
    "Error 18 is undefined",
    "Error 19 is undefined",
    "Error 20 is undefined",
    "Error 21 is undefined",
    "Error 22 is undefined",
    "Error 23 is undefined",
    "Error 24 is undefined",
    "Error 25 is undefined",
    "Error 26 is undefined",
    "Error 27 is undefined",
    "Error 28 is undefined",
    "Error 29 is undefined",
    "Error 30 is undefined",
    "Error 31 is undefined",
    "Error 32 is undefined",
    "Error 33 is undefined",
    "Error 34 is undefined",
    "Error 35 is undefined",
    "Error 36 is undefined",
    "Error 37 is undefined",
    "Error 38 is undefined",
    "Error 39 is undefined",
    "Error 40 is undefined",
    "Error 41 is undefined",
    "Error 42 is undefined",
    "Error 43 is undefined",
    "Error 44 is undefined",
    "Error 45 is undefined",
    "Error 46 is undefined",
    "Error 47 is undefined",
    "Error 48 is undefined",
    "Error 49 is undefined",
    "Error 50 is undefined",
    "Error 51 is undefined",
    "Error 52 is undefined",
    "Error 53 is undefined",
    "Error 54 is undefined",
    "Error 55 is undefined",
    "Error 56 is undefined",
    "Error 57 is undefined",
    "Error 58 is undefined",
    "Error 59 is undefined",
    "Error 60 is undefined",
    "Error 61 is undefined",
    "Error 62 is undefined",
    "Error 63 is undefined"
};

/*********************************
 * external function definitions
 *********************************/
exbo exboCreate(void) {
    return (exbo)zInstanceCreate();
}

exbo exboCreateConfigured(double X, int64_t A, int64_t L) {
    exbo result;
    struct instance *p = zInstanceCreate();
    if (p != (struct instance *)0) {
        if (!exboConfigure_X((exbo)p, X)) {
            if (!exboConfigure_A((exbo)p, A)) {
                if (!exboConfigure_L((exbo)p, L)) {
                    if (!exboFinishConfig((exbo)p)) {
                        result = (exbo)p;
                    } else {
                        result = (exbo)0;
                    }
                } else {
                    result = (exbo)0;
                }
            } else {
                result = (exbo)0;
            }
        } else {
            result = (exbo)0;
        }
        if (result == (exbo)0) {
            zInstanceDestroy(p);
        }
    } else {
        result = (exbo)0;
    }
    return result;
}

void exboDestroy(exbo xp) {
    zInstanceDestroy((struct instance *)xp);
    return;
}

int exboClearConfig(exbo xp) {
    int result;
    if (xp != (exbo)0) {
        struct config *config = ((struct instance *)xp)->config;
        if (config != (struct config *)0) {
            zConfigInit(config);
            result = 0;
        } else {
            // There is no instance structure
            result = ExboErr_NoConfig;
        }
    } else {
        // There is no instance structure
        result = ExboErr_NoInstance;
    }
    return result;
}

int exboConfigure_X(exbo xp, double X) {
    int result;
    if (xp != (exbo)0) {
        struct config *config = ((struct instance *)xp)->config;
        if (config != (struct config *)0) {
            config->isFinished = 0;
            config->isValid = 0;
            config->has_X = 1;
            config->X = X;
            result = 0;
        } else {
            // There is no instance structure
            result = ExboErr_NoConfig;
        }
    } else {
        // There is no instance structure
        result = ExboErr_NoInstance;
    }
    return result;
}

int exboConfigure_A(exbo xp, int64_t A) {
    int result;
    if (xp != (exbo)0) {
        struct config *config = ((struct instance *)xp)->config;
        if (config != (struct config *)0) {
            config->isFinished = 0;
            config->isValid = 0;
            config->has_A = 1;
            config->A = A;
            result = 0;
        } else {
            // There is no instance structure
            result = ExboErr_NoConfig;
        }
    } else {
        // There is no instance structure
        result = ExboErr_NoInstance;
    }
    return result;
}

int exboConfigure_L(exbo xp, int64_t L) {
    int result;
    if (xp != (exbo)0) {
        struct config *config = ((struct instance *)xp)->config;
        if (config != (struct config *)0) {
            config->isFinished = 0;
            config->isValid = 0;
            config->has_L = 1;
            config->L = L;
            result = 0;
        } else {
            // There is no instance structure
            result = ExboErr_NoConfig;
        }
    } else {
        // There is no instance structure
        result = ExboErr_NoInstance;
    }
    return result;
}

int exboValidateConfig(exbo xp) {
    int result;
    if (xp != (exbo)0) {
        struct config *config = ((struct instance *)xp)->config;
        if (config != (struct config *)0) {
            if (config->isFinished == 0) {
                result = zConfigValidate(config);
            } else {
                result = 0; // finished implies validated
            }
        } else {
            // There is no instance structure
            result = ExboErr_NoConfig;
        }
    } else {
        // There is no instance structure
        result = ExboErr_NoInstance;
    }
    return result;
}

int exboFinishConfig(exbo xp) {
    int result;
    if (xp != (exbo)0) {
        struct config *config = ((struct instance *)xp)->config;
        if (config != (struct config *)0) {
            result = zConfigFinish(config);
        } else {
            // There is no instance structure
            result = ExboErr_NoConfig;
        }
    } else {
        // There is no instance structure
        result = ExboErr_NoInstance;
    }
    return result;
}

int exboIsConfigFinished(exbo xp) {
    int result;
    if (xp != (exbo)0) {
        struct config *config = ((struct instance *)xp)->config;
        if (config != (struct config *)0) {
            result = config->isFinished;
        } else {
            result = 0;
        }
    } else {
        result = 0;
    }
    return result;
}

int exboIsConfigValidated(exbo xp) {
    int result;
    if (xp != (exbo)0) {
        struct config *config = ((struct instance *)xp)->config;
        if (config != (struct config *)0) {
            result = config->isValid;
        } else {
            result = 0;
        }
    } else {
        result = 0;
    }
    return result;
}

int exboDoesConfigHave_X(exbo xp) {
    int result;
    if (xp != (exbo)0) {
        struct config *config = ((struct instance *)xp)->config;
        if (config != (struct config *)0) {
            result = config->has_X;
        } else {
            result = 0;
        }
    } else {
        result = 0;
    }
    return result;
}

int exboDoesConfigHave_A(exbo xp) {
    int result;
    if (xp != (exbo)0) {
        struct config *config = ((struct instance *)xp)->config;
        if (config != (struct config *)0) {
            result = config->has_A;
        } else {
            result = 0;
        }
    } else {
        result = 0;
    }
    return result;
}

int exboDoesConfigHave_L(exbo xp) {
    int result;
    if (xp != (exbo)0) {
        struct config *config = ((struct instance *)xp)->config;
        if (config != (struct config *)0) {
            result = config->has_L;
        } else {
            result = 0;
        }
    } else {
        result = 0;
    }
    return result;
}

double exboGetConfig_X(exbo xp) {
    double result;
    if (xp != (exbo)0) {
        struct config *config = ((struct instance *)xp)->config;
        if (config != (struct config *)0) {
            if (config->has_X) {
                result = config->X;
            } else {
                result = nan(NanTag_ExboErr_ConfigValueNotSet);
            }
        } else {
            result = nan(NanTag_ExboErr_NoConfig);
        }
    } else {
        result = nan(NanTag_ExboErr_NoInstance);
    }
    return result;
}

int64_t exboGetConfig_A(exbo xp) {
    int64_t result;
    if (xp != (exbo)0) {
        struct config *config = ((struct instance *)xp)->config;
        if (config != (struct config *)0) {
            if (config->has_A) {
                result = config->A;
            } else {
                result = INT64_MIN + ExboErr_ConfigValueNotSet;
            }
        } else {
            result = INT64_MIN + ExboErr_NoConfig;
        }
    } else {
        result = INT64_MIN + ExboErr_NoInstance;
    }
    return result;
}

int64_t exboGetConfig_L(exbo xp) {
    int64_t result;
    if (xp != (exbo)0) {
        struct config *config = ((struct instance *)xp)->config;
        if (config != (struct config *)0) {
            if (config->has_L) {
                result = config->L;
            } else {
                result = INT64_MIN + ExboErr_ConfigValueNotSet;
            }
        } else {
            result = INT64_MIN + ExboErr_NoConfig;
        }
    } else {
        result = INT64_MIN + ExboErr_NoInstance;
    }
    return result;
}

int exboRecordAttempt(exbo xp, int64_t time) {
    int result;
    if (xp != (exbo)0) {
        struct instance *p = (struct instance *)xp;
        struct config *config = p->config;
        if (config != (struct config *)0) {
            int r;
            if ((r = zConfigFinish(config)) <= 0) {
                int warning = 0;
                if (r < 0) {
                    // Accumulate the warning
                    warning = r;
                }
                int64_t T_in = p->T;
                int64_t T_out = time;
                if (T_out >= T_in) {
                    int64_t D_in = p->D;
                    int64_t I_in = p->I;
                    int64_t T_diff = T_out - T_in;
                    int64_t D_prime;
                    if (T_diff >= (int64_t)0) {
                        // T_diff did not overflow
                        if (T_diff < I_in) {
                            // The user is being too aggressive.
                            // Accumulate the warning
                            // This warning overrides any previous warning.
                            warning = ExboWarn_AttemptIsEarlierThanRecommended;
                        }
                        if (T_diff < D_in) {
                            D_prime = D_in - T_diff;
                        } else {
                            D_prime = (int64_t)0;
                        }
                    } else {
                        // T_diff overflowed - no warning is needed
                        D_prime = (int64_t)0;
                    }
                    int64_t L = config->L;
                    int64_t A = config->A;
                    double X = config->X;
                    int64_t D_out = D_prime + A;
                    int64_t I_out;
                    if (D_out >= A) {
                        // D_out did not overflow
                        if ((r = zInterval(L, A, X, D_out, &I_out)) <= 0) {
                            if (r < 0) {
                                // Accumulate the warning
                                // This warning overrides any previous warning.
                                warning = r;
                            }
                            result = 0;
                        } else {
                            // Report the error from zInterval().
                            result = r;
                        }
                    } else {
                        // D_out overflowed
                        D_out = INT64_MAX;
                        I_out = D_out - (L - A);
                        // Accumulate the warning
                        warning = ExboWarn_ExcessCostLimitBreachWithDebtOverflow;
                        result = 0;
                    }
                    if (result == 0) {
                        // There is no error so update the state
                        p->T = T_out;
                        p->I = I_out;
                        p->D = D_out;
                        if (warning == 0) {
                            // record any warning that accumulated
                            result = warning;
                        }
                    }
                } else {
                    // The attempts are being recorded out of order
                    result = ExboErr_RecordingAPriorAttempt;
                }
            } else {
                // Report the error from zConfigFinish()
                result = r;
            }
        } else {
            // There is no config structure
            result = ExboErr_NoConfig;
        }
    } else {
        // There is no instance structure
        result = ExboErr_NoInstance;
    }
    return result;
}

/* To signal an error, this function returns a value that is less
 * than Exbo_MinimumTime, which equals INT64_MIN + ExboErr_MAXIMUM.
 */
int64_t exboGetPreviousAttemptTime(exbo xp) {
    int64_t result;
    if (xp != (exbo)0) {
        struct instance *p = (struct instance *)xp;
        int64_t T = p->T;
        if (T >= Exbo_MinimumTime) {
            result = T;
        } else {
            // silently mask the T underflow
            result = Exbo_MinimumTime;
        }
    } else {
        // There is no instance structure
        result = INT64_MIN + ExboErr_NoInstance;
    }
    return result;
}

/* To signal an error, this function returns a value that is less
 * than Exbo_MinimumTime, which equals INT64_MIN + ExboErr_MAXIMUM.
 */
int64_t exboGetNextAttemptTime(exbo xp) {
    int64_t result;
    if (xp != (exbo)0) {
        struct instance *p = (struct instance *)xp;
        int64_t I = p->I;
        if (I >= (int64_t)0) {
            int64_t T = p->T;
            int64_t T_plus_I = T + I;
            if (T_plus_I >= T) {
                if (T_plus_I >= Exbo_MinimumTime) {
                    result = T_plus_I;
                } else {
                    // silently mask the T + I underflow
                    result = Exbo_MinimumTime;
                }
            } else {
                // T + I overflowed
                result = INT64_MIN + ExboErr_NextTimeOverflow;
            }
        } else {
            // I should not be negative
            result = INT64_MIN + ExboErr_StateWithNegativeI;
        }
    } else {
        // There is no instance structure
        result = INT64_MIN + ExboErr_NoInstance;
    }
    return result;
}

/* To signal an error, this function returns a value that is less
 * than Exbo_MinimumTime, which equals INT64_MIN + ExboErr_MAXIMUM.
 */
int64_t exboGetPayBackTime(exbo xp) {
    int64_t result;
    if (xp != (exbo)0) {
        struct instance *p = (struct instance *)xp;
        int64_t D = p->D;
        if (D >= (int64_t)0) {
            int64_t T = p->T;
            int64_t T_plus_D = T + D;
            if (T_plus_D >= T) {
                if (T_plus_D >= Exbo_MinimumTime) {
                    result = T_plus_D;
                } else {
                    // silently mask the T + D underflow
                    result = Exbo_MinimumTime;
                }
            } else {
                // T + D overflowed
                result = INT64_MIN + ExboErr_PayBackTimeOverflow;
            }
        } else {
            // D should not be negative
            result = INT64_MIN + ExboErr_StateWithNegativeD;
        }
    } else {
        // There is no instance structure
        result = INT64_MIN + ExboErr_NoInstance;
    }
    return result;
}

const char *exboGetNanErrorMessage(double nanErrorNumber) {
    const char *result;
    if (isnan(nanErrorNumber)) {
        // extract the error number from the NaN
        uint64_t nanErrorNumberHex = *((uint64_t *)(&nanErrorNumber));
        int errorNumber = (int)(0x7ffffff & nanErrorNumberHex);
        result = zErrMessages[errorNumber];
    } else {
        result = zErrMessageNotError;
    }
    return result;
}

const char *exboGetTimeErrorMessage(int64_t timeErrorNumber) {
    const char *result;
    if (timeErrorNumber < Exbo_MinimumTime) {
        int errorNumber = (int)(timeErrorNumber - INT64_MIN);
        result = zErrMessages[errorNumber];
    } else {
        result = zErrMessageNotError;
    }
    return result;
}

const char *exboGetErrorMessage(int errorNumber) {
    const char *result;
    if (errorNumber >= 0) {
        if (errorNumber < ExboErr_MAXIMUM) {
            result = zErrMessages[errorNumber];
        } else {
            result = zErrMessageMaximum;
        }
    } else {
        result = zErrMessageNegative;
    }
    return result;
}

/*********************************
 * internal function definitions
 *********************************/

/***********************
* Managing an instance *
***********************/
static struct instance *zInstanceCreate(void) {
    struct instance *result;
    struct instance *p = (struct instance *)malloc(sizeof(*p));
    if (p != (struct instance *)0) {
        p->T = INT64_MIN;
        p->D = (int64_t)0;
        p->I = (int64_t)0;
        p->config = zConfigCreate();
        if (p->config != (struct config *)0) {
            result = p;
        } else {
            zInstanceDestroy(p);
            result = (struct instance *)0;
        }
    } else {
        result = (struct instance *)0;
    }
    return result;
}

static void zInstanceDestroy(struct instance *p) {
    if (p != (struct instance *)0) {
        zConfigDestroy(p->config);
        p->config = (struct config *)0;
        p->D = (int64_t)0;
        p->I = (int64_t)0;
        p->T = (int64_t)0;
        free((void *)p);
    }
    return;
}

/***************************
* Managing a configuration *
***************************/
static struct config *zConfigCreate() {
    struct config *p = (struct config *)malloc(sizeof(*p));
    if (p != (struct config *)0) {
        zConfigInit(p);
    }
    return p;
}

static void zConfigDestroy(struct config *p) {
    if (p != (struct config *)0) {
        zConfigInit(p);
        free((void *)p);
    }
    return;
}

static void zConfigInit(struct config *p) {
    // Assert: p != (struct config *)0
    p->isFinished = 0;
    p->isValid = 0;
    p->has_X = 0;
    p->has_A = 0;
    p->has_L = 0;
    p->X = (double)0.0;
    p->A = (int64_t)0;
    p->L = (int64_t)0;
    return;
}

/*****************************
* Validating a configuration *
*****************************/
static int zConfigValidate(struct config *p) {
    // Assert: p != (struct config *)0
    // Assert: p->isFinished == 0
    int result;
    if (p->isValid == 0) {
        int r;
        if ((r = zConfigRevalidate(p)) == 0) {
            p->isValid = 1;
            result = 0;
        } else {
            result = r;
        }
    } else {
        result = 0;     // already validated
    }
    return result;
}

static int zConfigRevalidate(struct config *p) {
    // Assert: p != (struct config *)0
    int result;
    int r;
    if ((r = zConfigValidate_X(p)) == 0) {
        if ((r = zConfigValidate_A(p)) == 0) {
            if ((r = zConfigValidate_L(p)) == 0) {
                result = 0;
            } else {
                result = r;
            }
        } else {
            result = r;
        }
    } else {
        result = r;
    }
    return result;
}

static int zConfigValidate_X(struct config *p) {
    // Assert: p != (struct config *)0
    int result;
    if (p->has_X) {
        double X = p->X;
        if (isfinite(X)) {
            if (X >= 1.0) {
                result = 0;
            } else {
                result = ExboErr_InvalidConfig_X2;
            }
        } else {
            result = ExboErr_InvalidConfig_X1;
        }
    } else {
        result = 0;
    }
    return result;
}

static int zConfigValidate_A(struct config *p) {
    // Assert: p != (struct config *)0
    int result;
    if (p->has_A) {
        if (p->A > (int64_t)0) {
            result = 0;
        } else {
            // "The given A is not positive"
            result = ExboErr_InvalidConfig_A1;
        }
    } else {
        result = 0;
    }
    return result;
}

static int zConfigValidate_L(struct config *p) {
    // Assert: p != (struct config *)0
    int result;
    if (p->has_L) {
        if (p->L > (int64_t)0) {
            if (p->has_A) {
                if (p->L >= p->A) {
                    result = 0;
                } else {
                    // "The given L is less than the given A"
                    result = ExboErr_InvalidConfig_L2;
                }
            } else {
                result = 0;
            }
        } else {
            // "The given L is not positive"
            result = ExboErr_InvalidConfig_L1;
        }
    } else {
        result = 0;
    }
    return result;
}

/****************************
* Finishing a configuration *
****************************/
static int zConfigFinish(struct config *p) {
    // Assert: p != (struct config *)0
    int result;
    if (p->isFinished == 0) {
        int r;
        if ((r = zConfigValidate(p)) == 0) {
            if ((r = zConfigSetDefaults(p)) == 0) {
                // Assert: p->has_X + p->has_A + p->has_L == 3
                p->isFinished = 1;
                if ((r = zValidateFinish(p)) == 0) {
                    result = 0;
                    // all is well
                } else {
                    p->isFinished = 0;
                    p->isValid = 0;
                    result = r;
                }
            } else {
                result = r; // defaulting failed
            }
        } else {
            result = r; // validation failed
        }
    } else {
        result = 0; // already finished
    }
    return result;
}

static int zConfigSetDefaults(struct config *p) {
    int result = 0;
    while (result == 0) {
        if (p->has_X) {
            if (p->has_A) {
                if (p->has_L) {
                    break;
                } else {
                    result = zSetDefault_L(p);
                }
            } else {
                result = zSetDefault_A(p);
            }
        } else {
            result = zSetDefault_X(p);
        }
    }
    return result;
}

static int zSetDefault_X(struct config *p) {
    // Assert: p != (struct config *)0
    // Assert: p->has_X == 0
    p->X = DEFAULT_X;
    p->has_X = 1;
    return 0;
}

static int zSetDefault_A(struct config *p) {
    // Assert: p != (struct config *)0
    // Assert: p->has_X == 1
    // Assert: p->has_A == 0
    if (p->has_L) {
        // L is set.
        // Choose A so that A * DEFAULT_L_OVER_A near enough equals L
        p->A = (int64_t)ceil((double)(p->L) / DEFAULT_L_OVER_A);
    } else {
        // L is not set.
        p->A = DEFAULT_A;
    }
    p->has_A = 1;
    return 0;
}

static int zSetDefault_L(struct config *p) {
    // Assert: p != (struct config *)0
    // Assert: p->has_X == 1
    // Assert: p->has_A == 1
    // Assert: p->has_L == 0
    // X and A are set.

    // Choose as A * DEFAULT_L_OVER_A, unless this would overflow.
    int64_t A = p->A;
    int64_t AMax = INT64_MAX / DEFAULT_L_OVER_A;
    if (A <= AMax) {
        p->L = A * DEFAULT_L_OVER_A;
    } else {
        p->L = INT64_MAX;
    }
    p->has_L = 1;
    return 0;
}

static int zValidateFinish(struct config *p) {
    // Assert: p != (struct config *)0
    int result;
    if (p->isFinished && p->isValid) {
        if (p->has_X && p->has_A && p->has_L) {
            int r;
            if ((r = zConfigRevalidate(p)) == 0) {
                result = 0;
            } else {
                result = ExboErr_InternalError_3;
            }
        } else {
            result = ExboErr_InternalError_2;
        }
    } else {
        result = ExboErr_InternalError_1;
    }
    return result;
}

/*********************************
* interval computation functions *
*********************************/
static int zInterval(int64_t L, int64_t A, double X, int64_t D, int64_t *Ip) {
    // Assert D >= A
    // Assert A > 0
    // Assert L >= A
    // Assert X >= 1.0
    // Assert Ip != (int64_t *)0
    int result;
    if (D < L) {
        // The excess cost limit is under-saturated.
        if (X > 1.0) {
            // The relaxation factor exceed 1.0
            double l = (double)(L - D) / (double)A;
            double j;
            int r = z_J(l, X, &j);
            if (r <= 0) {
                // Assert j >= 0.0
                *Ip = (int64_t)ceil((double)A * pow(X, -j));
            }
            result = r;
        } else {
            // The relaxation factor is 1.0
            *Ip = A;
            result = 0;
        }
    } else {
        if (D == L) {
            // The excess cost limit is saturated.
            *Ip = A;
            result = 0;
        } else {
            // The excess cost limit is over-saturated.
            // Wait until the excess cost is L - A.
            *Ip = D - (L - A);
            // The above does not overflow because
            // D >= L and L - A >= 0 so that D >= *Ip >= 0
            result = ExboWarn_ExcessCostLimitBreach;
        }
    }
    return result;
}

static int z_J(double l, double X, double *jp) {
    // Assert l >= 0.0;
    // Assert X > 1.0;
    // Asset jp != (double *)0;
    int result;
    int64_t J_low = (int64_t)ceil(l) - (int64_t)1;
    if (J_low < (int64_t)0) {
        J_low = (int64_t)0;
    }
    int64_t J_high = (int64_t)ceil(l + 1.0/(X - 1.0));
    int loop_r = 0;
    while (J_low < J_high) {
        int64_t J_try = J_low + (J_high - J_low)/((int64_t)2);
        double m_J;
        if ((loop_r = z_m((double)J_try, X, &m_J)) != 0) {
            break;
        }
        if (m_J <= l) {
            J_low = J_try + (int64_t)1;
        }
        if (m_J >= l) {
            J_high = J_try;
        }
    }
    if (loop_r == 0) {
        double J = (double)J_high;
        double mu = (pow(X, J) - 1.0)/((X - 1.0)*(J - l));
        *jp = log(mu)/log(X);
        result = 0;
    } else {
        result = loop_r;
    }
    return result;
}

static int z_m(double j, double X, double *vp) {
    // Assert: vp != (double *)0
    *vp = j - (1.0 - pow(X, -j))/(X - 1.0);
    return 0;
}

/*********************************
 * The End
 *********************************/

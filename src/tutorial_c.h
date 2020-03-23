/********************************************************************************
*
*  Author:
*     name:  Colby Ham
*     phone: (509)375-2419
*     email: colby.ham@pnnl.gov
*
********************************************************************************
*
*  REPOSITORY INFORMATION:
*    $Revision: $
*    $Author: $
*    $Date: $
*    $State: $
*
********************************************************************************
*
*  NOTE: DOXYGEN is used to generate documentation for this file.
*
*******************************************************************************/

/** @file tutorial_c.h
 *  tutorial_c VAP Header File.
 */

#ifndef _TUTORIAL_C_VAP_H_
#define _TUTORIAL_C_VAP_H_

#include "dsproc3.h"

/**
 *  @defgroup tutorial_c_VAP tutorial_c VAP
 */
/*@{*/

/** tutorial_c output datastream names and levels */
#define TUTORIAL30S_C1_DS_NAME "tutorial30s"
#define TUTORIAL30S_C1_DS_LEVEL "c1"
#define TUTORIAL_C1_DS_NAME "tutorial"
#define TUTORIAL_C1_DS_LEVEL "c1"

/** Standard value used for missing data. */
#define MISSING_VALUE -9999

/**
 *  User Data.
 */
typedef struct
{
    const char *proc_name;

} UserData;

/*******************************************************************************
*  Prototypes
*/

/* Main VAP Functions */

void *init_process(void);
void  finish_process(void *user_data);

int pre_transform(
    void     *user_data,
    time_t    begin_time,
    time_t    end_time,
    CDSGroup *ret_data);

int post_transform(
    void     *user_data,
    time_t    begin_time,
    time_t    end_time,
    CDSGroup *trans_data);

int process_data(
    void     *user_data,
    time_t    begin_date,
    time_t    end_date,
    CDSGroup *trans_data);

int create_quicklook(
    void     *user_data,
    time_t    begin_date,
    time_t    end_date);

/*@}*/

#endif /* _TUTORIAL_C_VAP_H_ */

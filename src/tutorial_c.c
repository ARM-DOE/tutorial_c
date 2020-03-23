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

/** @file tutorial_c.c
 *  tutorial_c VAP Main Functions.
 */

#include "tutorial_c.h"
#include "tutorial_c_trans_fields.h"
#include "tutorial_c_output_fields.h"

#include "../config.h"
static const char *_Version = PACKAGE_NAME"-"PACKAGE_VERSION;

/**
 *  Initialize the process.
 *
 *  This function is used to do any up front process initialization that
 *  is specific to this process, and to create the UserData structure that
 *  will be passed to all hook functions.
 *
 *  If an error occurs in this function it will be appended to the log and
 *  error mail messages, and the process status will be set appropriately.
 *
 *  @return
 *    - void pointer to the UserData structure
 *    - NULL if an error occurred
 */
void *init_process(void)
{
    UserData *mydata;

    DSPROC_DEBUG_LV1(
        "Creating user defined data structure\n");

    mydata = (UserData *)calloc(1, sizeof(UserData));

    if (!mydata) {

        DSPROC_ERROR(
            DSPROC_ENOMEM,
            "Memory allocation error creating user data structure\n");

        return((void *)NULL);
    }

    mydata->proc_name = dsproc_get_name();

    /* roll up the qc bits of all vars 
    /* in the output DOD with transformed qc bits */
    dsproc_set_trans_qc_rollup_flag(1);


    return((void *)mydata);
}

/**
 *  Finish the process.
 *
 *  This function frees all memory used by the UserData structure.
 *
 *  @param user_data  - void pointer to the UserData structure
 *                      returned by the init_process() function
 */
void finish_process(void *user_data)
{
    UserData *mydata = (UserData *)user_data;

    DSPROC_DEBUG_LV1(
        "Cleaning up user defined data structure for process\n");

    if (mydata) {
        free(mydata);
    }
}

/*
 * Function called from pre_transform_hook.
 *
 * This function converts from F to K if the F unit
 * description is in a format unrecognizable by 
 * udUnits as the tps.c1 is known to be in.
 *
 * @param var_name   - the name of the variable whose units are
 *                     to be examined.
 *
 * @return
 *   -  1 if processing should continue normally
 *   -  0 if fatel error occurred.
 *
 */

int check_units(char *var_name)
{
    CDSVar *tempvar;
    CDSVar *time_var;
    float  *tempdata;
    int    i, nsamples, status;
    
    /**************************************/
    /* Get var from retrieved data structure */
    /**************************************/
    
    if (!(tempvar = dsproc_get_retrieved_var(var_name, 0)) ) return(0);
    /* Get the var's data */
    if (!(tempdata = (float *)dsproc_get_var_data_index(tempvar))) return(0);
    if( !(time_var = dsproc_get_time_var(tempvar)) ) return(0);
    nsamples = time_var->sample_count;

    /*******************************/
    /* Check temperature units for format that failes */
    /* udunits test. */
    /*******************************/
    char *tempunit;
    tempunit = dsproc_get_att_text(tempvar, "units",
            NULL, NULL);
    if(strstr(tempunit, "degree F"))
    {
        /* Convert degree F to Kelvin */
        /* and update the units attribute */
        status = dsproc_set_att_text(tempvar, "units","K");
        if(status == 0) return(0);
        for (i = 0; i < nsamples; i++) {
            if(tempdata[i] != -9999.) tempdata[i] = 5./9.*(tempdata[i]+ 459.67);
        }
    }

    return(1);

}

/**
 *  Hook function called just prior to data transformation.
 *
 *  This function will be called once per processing interval just prior to
 *  data transformation, and after the retrieved observations are merged.
 *
 *  @param user_data  - void pointer to the UserData structure
 *                      returned by the init_process() function
 *  @param begin_date - the begin time of the current processing interval
 *  @param end_date   - the end time of the current processing interval
 *  @param ret_data   - pointer to the parent CDSGroup containing all the
 *                      retrieved data
 *
 *  @return
 *    -  1 if processing should continue normally
 *    -  0 if processing should skip the current processing interval
 *         and continue on to the next one.
 *    - -1 if a fatal error occurred and the process should exit.
 */
int pre_transform_hook(
    void     *user_data,
    time_t    begin_date,
    time_t    end_date,
    CDSGroup *ret_data)
{
    UserData *mydata = (UserData *)user_data;
    int       status    = 1;

    /* Dump retrieved data */
/*
    if (dsproc_get_debug_level() > 1) {

        dsproc_dump_retrieved_datasets(
            "./debug_dumps", "before_units_check.debug", 0);
    } 
*/

    /* Check units of all retrieved temperature values */
    status = check_units("temperature_vap");
    if(status == 0) return(-1);
    status = check_units("temperature_ingest");
    if(status == 0) return(-1);
    status = check_units("temperature");
    if(status == 0) return(-1);

    /* Dump retrieved data */
/*
    if (dsproc_get_debug_level() > 1) {

        dsproc_dump_retrieved_datasets(
            "./debug_dumps", "after_units_check.debug", 0);
    } 
*/

    return(status);
}

/**
 *  Hook function called just after data transformation.
 *
 *  This function will be called once per processing interval just after data
 *  transformation, but before the process_data function is called.
 *
 *  @param  user_data - void pointer to the UserData structure
 *                      returned by the init_process() function
 *  @param begin_date - the begin time of the current processing interval
 *  @param end_date   - the end time of the current processing interval
 *  @param trans_data - pointer to the parent CDSGroup containing all the
 *                      transformed data
 *
 *  @return
 *    -  1 if processing should continue normally
 *    -  0 if processing should skip the current processing interval
 *         and continue on to the next one.
 *    - -1 if a fatal error occurred and the process should exit.
 */
int post_transform_hook(
    void     *user_data,
    time_t    begin_date,
    time_t    end_date,
    CDSGroup *trans_data)
{
    UserData *mydata = (UserData *)user_data;
    int       status = 1;

/*  Sample call to your own process */
/*    status = tutorial_c_post_transform_hook( 
            user_data, begin_date, end_date, trans_data);  */

/* Dump transformed data */
/*    if (dsproc_get_debug_level() > 1) {

        dsproc_dump_transformed_datasets(
            "./debug_dumps", "post_transform.debug", 0);
    } */


    return(status);
}

/**
 *  Main data processing function.
 *
 *  This function will be called once per processing interval just after the
 *  output datasets are created, but before they are stored to disk.
 *
 *  @param  user_data - void pointer to the UserData structure
 *                      returned by the init_process() function
 *  @param  begin_date - begin time of the processing interval
 *  @param  end_date   - end time of the processing interval
 *  @param  trans_data - retriever data transformed to user defined 
                         coordinate systems
 *
 *  @return
 *    -  1 if processing should continue normally
 *    -  0 if processing should skip the current processing interval
 *         and continue on to the next one.
 *    - -1 if a fatal error occurred and the process should exit.

 */
int process_data(
    void     *user_data,
    time_t    begin_date,
    time_t    end_date,
    CDSGroup *trans_data)
{
    UserData *mydata = (UserData *)user_data;

    /* ------------------------------------------------------------- */
    /* Start algorithm */
    /* ------------------------------------------------------------- */

    int dsid_out, status;
    CDSVar *tempvap_var;
    CDSVar *tempingest_var;
    CDSVar *temp_var;
    CDSVar *time_var;
    CDSVar *temp_source_var;
    float  *tempvap_data;
    float  *tempingest_data;
    float  *temp_data;
    int    *temp_source_data;
    int    i, j;
    int    nsamples;


    /**************************************/
    /* Get retrieved and output variables */
    /* and data needed to do analysis     */ 
    /**************************************/
    /*Get dsid for tutorial.c1 */
    dsid_out = dsproc_get_output_datastream_id("tutorial","c1");
    if(dsid_out < 0) return(-1);

    /* Get the temp vap and ingest variables and data*/
    if (!(tempvap_var = dsproc_get_output_var(dsid_out, 
        "temperature_vap", 0)) ) return(-1);
    if (!(tempingest_var = dsproc_get_output_var(dsid_out,
        "temperature_ingest", 0)) ) return(-1);

    /* Get the data for the retrieved vap and ingest temps */
    if (!(tempvap_data = (float *)dsproc_get_var_data_index(tempvap_var))) return(-1);
    if (!(tempingest_data = (float *)dsproc_get_var_data_index(tempingest_var))) return(-1);
    
    /* get output variable for temperature */
    if (!(temp_var = dsproc_get_output_var(dsid_out,
        "temperature",0)) ) return(-1);

    /* The length of the new temp will be  */
    /* the same as those passed through  */
    if( !(time_var = dsproc_get_time_var(tempvap_var)) ) return(-1);
    nsamples = time_var->sample_count;

    /*************************************/
    /* Get source variable information   */
    /* for each retrieved temp variable    */
    /*************************************/
    /* get source variable that will document whether */
    /* data was used from the vap or ingeset source */
    /* from the output datastream */
    if (!(temp_source_var = dsproc_get_output_var(dsid_out,
        "source_temperature",0)) ) return(-1);

    const char *tempvap_ds_source;
    const char *tempvap_ds_var_name;
    char tempvap_full_source[128];
    const char *tempingest_ds_source;
    const char *tempingest_ds_var_name;
    char tempingest_full_source[128];
    char full_source[128];

    /* get the source datastream name and variable name */
    /* for both the tempvap and tempingest variables */
    if(!(tempvap_ds_source = dsproc_get_source_ds_name(tempvap_var))) return(-1);
    if(!(tempvap_ds_var_name = dsproc_get_source_var_name(tempvap_var))) return(-1);
    sprintf(tempvap_full_source, "%s:%s", tempvap_ds_source, tempvap_ds_var_name);
    if(!(tempingest_ds_source = dsproc_get_source_ds_name(tempingest_var))) return(-1);
    if(!(tempingest_ds_var_name = dsproc_get_source_var_name(tempingest_var))) return(-1);
    sprintf(tempingest_full_source, "%s:%s", tempingest_ds_source, tempingest_ds_var_name);

    /* Determine the number of flag attrbute values associted with var */
    int source_temp_natts;
    int flag_cnt = 0, k;
    char **flag_att_names, **flag_att_values; 

    source_temp_natts = temp_source_var->natts;
    for (j = 0; j < source_temp_natts; j++) {
        
        if(strstr(temp_source_var->atts[j]->name, "flag_") && 
           strstr(temp_source_var->atts[j]->name, "_description"))
        {
            flag_cnt = flag_cnt+ 1;
        } 
    }

    /* allocate memory for flag names array and for each possible */
    /* flag gets its value from DOD */
    if(!(flag_att_names = (char **)calloc(flag_cnt, sizeof(char*))) )return(-1);
    if(!(flag_att_values = (char **)calloc(flag_cnt, sizeof(char*))) )return(-1);
    for (j = 0; j < flag_cnt; j++) {
        if(!(flag_att_names[j] = (char *)calloc(512, sizeof(char))) )return(-1);
        if(!(flag_att_values[j] = (char *)calloc(512, sizeof(char))) )return(-1);

        sprintf(flag_att_names[j], "flag_%d_description", j);
        flag_att_values[j] = dsproc_get_att_text(temp_source_var, flag_att_names[j],
                NULL, NULL);
        if (!flag_att_values[j]) return(-1);

    }

    /* initalize source to zero then set the value */
    /* of source appropriately */
    /* For temp_data we do this using a provided function */
    if(!(temp_data = (float *)dsproc_alloc_var_data_index(temp_var, 0, nsamples)) )return(-1);
    /* allocate memory for data and set values */
    /* for temp_source_data we use an alternative method of allocating an */
    /* an array and then later using this array with the */
    /* dsproc_set_var_data function */ 
    if(!(temp_source_data = (int *)calloc(nsamples, sizeof(int))) )return(-1);

    for (k = 0; k < nsamples; k++) {
        /* Set the temp value */
        temp_source_data[k] = 0;
        /* If vap value is greater than ingest then set to vap */
        if (tempvap_data[k] > tempingest_data[k]) {
            temp_data[k] = tempvap_data[k];
            sprintf(full_source, "%s", tempvap_full_source);
        }
        else {
            /* otherwise set to to ingest value */
            temp_data[k] = tempingest_data[k];
            sprintf(full_source, "%s", tempingest_full_source);
        }
        /* initialize then set the source of temp */
        for (j = 0; j < flag_cnt; j++) {
            if(strcmp(flag_att_values[j], full_source) == 0) {
                temp_source_data[k] = j;
            }
        }
    }

    /* Set values of source_temp variable */
    if(!(dsproc_set_var_data(temp_source_var, CDS_INT, 0, (size_t)nsamples, NULL,
        (void *)temp_source_data))) return(-1);

    for (j = 0; j < flag_cnt; j++) {
        free(flag_att_values[j]);
        free(flag_att_names[j]);
    }
    free(flag_att_values);
    free(flag_att_names);
    free(temp_source_data);


    /* ------------------------------------------------------------- */
    /* End algorithm */
    /* ------------------------------------------------------------- */

    /* Print the entire output dataset if debug level is 2 */
/*
    if (dsproc_get_debug_level() > 1) {

        dsproc_dump_output_datasets(
            "./debug_dumps", "process_data.debug", 0);
    }
*/




    return(1);
}

/*
 * Quicklook hook function 
 *
 * This function will be called once per processing interval just after data
 * is stored after process_data is complete.
 *
 * @param  user_data - void pointer to the UserData structure
 *                     returned by the init_process() function
 * @param begin_date - the begin time of the current processing interval
 * @param end_date   - the end time of the current processing interval
 *
 * @return
 *   -  1 if processing should continue normally
 *   -  0 if processing should skip the current processing interval
 *        and continue on to the next one.
 *   -1 if a fatal error occurred and the process should exit.
 */ 
int create_quicklook(
    void     *user_data,
    time_t    begin_date,
    time_t    end_date)
{
    /* see adi_example4_c for an example implementation. */
 
    return(1);
}

/**
 *  Main entry function.
 *
 *  @param  argc - number of command line arguments
 *  @param  argv - command line arguments
 *
 *  @return
 *    - 0 if successful
 *    - 1 if an error occurred
 */
int main(int argc, char **argv)
{
    int exit_value;

    dsproc_use_nc_extension();

    static const char *proc_names[] = {
        "tutorial_c"
    };

    int nproc_names = sizeof(proc_names)/sizeof(const char *);

    dsproc_set_init_process_hook(init_process);
    dsproc_set_process_data_hook(process_data);
    dsproc_set_finish_process_hook(finish_process);

    dsproc_set_pre_transform_hook(pre_transform_hook);
    dsproc_set_post_transform_hook(post_transform_hook);
    dsproc_set_quicklook_hook(create_quicklook);
    
    exit_value = dsproc_main(
        argc, argv,
        PM_TRANSFORM_VAP,
        _Version,
        nproc_names,
        (const char **)proc_names);

    return(exit_value);
}


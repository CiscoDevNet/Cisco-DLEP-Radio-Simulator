/*------------------------------------------------------------------
 * dlep_source_cli.c -- source CLI commands 
 *
 * October 2011, Bo Berry
 *
 * Copyright (c) 2011 by Cisco Systems, Inc.
 * All rights reserved. 
 * 
 * Permission is hereby granted, free of charge, to any person 
 * obtaining a copy of this software and associated documentation 
 * files (the "Software"), to deal in the Software without 
 * restriction, including without limitation the rights to use, 
 * copy, modify, merge, publish, distribute, sublicense, and/or 
 * sell copies of the Software, and to permit persons to whom 
 * the Software is furnished to do so, subject to the following 
 * conditions: 
 * 
 * The above copyright notice and this permission notice shall be 
 * included in all copies or substantial portions of the Software. 
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND  
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT  
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,  
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING  
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR  
 * OTHER DEALINGS IN THE SOFTWARE. 
 *------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "cli.h"

#include "dlep_source_cli.h" 
//SENDIL
#include "sim_dlep_radio_cli.h"



/*
 * directory record
 */ 
cli_record_t cli_source_dir;


/*
 * allocate command records
 */
static cli_record_t source_command_cmd;



/**
 ** source commands 
 **/ 

static void
source_commands (uint32_t argc, char *argv[])
{
    FILE *fp;

#define MAX_INPUT_LENGTH  ( 512 )
    char input_string[MAX_INPUT_LENGTH];


    if (argv[1] && *argv[1] == '?') {
        printf("source_commands <filename> - source commands from the file\n");
        printf("\n");
        printf(" <filename> - the file that contains commands being sourced \n");
        printf("\n");
        return;
    }

    printf("sourcing file %s \n", argv[1]);

    fp = fopen(argv[1], "r");
    if (!fp) {
        printf("Error: problem opening source file: %s\n",
                argv[1]);
        return;
    }

    while (fgets(input_string, MAX_INPUT_LENGTH, fp)) {
        if (input_string[0] == '\0') {
            continue;
        } else if (input_string[0] == '#') {
            continue;
        }

        dlep_cli_engine(input_string);
    } 

    return;
}



/** 
 * NAME
 *    dlep_source_cli_init
 *
 * SYNOPSIS 
 *    void
 *    dlep_source_cli_init()
 *
 * DESCRIPTION
 *    Registers the dlep source command with the cli 
 *    facility. 
 *
 * INPUT PARAMETERS
 *    none
 *
 * OUTPUT PARAMETERS
 *    none
 *
 * RETURN VALUE
 *    none
 * 
 */
void
dlep_source_cli_init (void)
{
    RC_CLI_t rc;

   /*
    * Create directory and commands
    */
    rc = cli_mkdir("source", NULL, &cli_source_dir);


    rc = cli_mkcmd("source_commands", 
                    source_commands, 
                   &cli_source_dir, 
                   &source_command_cmd);

   return;
}


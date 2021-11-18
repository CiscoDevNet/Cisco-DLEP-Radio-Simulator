/*----------------------------------------------------------------------
 * fuzz_packet.c -- Routines to read a fuzzed packet file 
 *
 * October 2010, Bo Berry
 *
 * Copyright (c) 2010 by Cisco Systems, Inc.
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
 *---------------------------------------------------------------------
 */ 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "safe_types.h"
#include "fuzz_packet.h"


#define FUZZ_PACKET_LENGTH                 ( 2048 ) 


/** 
 * NAME  
 *    fuzz_packet_load 
 *
 * DESCRIPTION 
 *    This function reads the designated file that contains
 *    a fuzzed DLEP packet. The fuzzed packet is used for testing. 
 *
 * INPUT PARAMETERS
 *
 * OUTPUT PARAMETERS
 *
 * RETURN VALUE
 *    length - number of bytes in packet  
 * 
 */
uint32_t 
fuzz_packet_load (char *fuzzy_filename, uint8_t *p2packet) 
{
    uint32_t i; 
    uint32_t key_index;
    uint32_t line_count;

    FILE *fp;


#define MAX_INPUT_LENGTH  ( 512 )
    char directed_filename[MAX_INPUT_LENGTH];
    char input_string[MAX_INPUT_LENGTH];

#define ARGC_MAX   ( 8 )
    uint32_t argc;
    char *argv[ARGC_MAX];


    if (!fuzzy_filename || !p2packet) {
        return (0);
    } 

    strcpy(directed_filename, "fuzzy_dir/");
    strcat(directed_filename, fuzzy_filename);
    
    fp = fopen(directed_filename, "r");
    if (!fp) {
        printf("Error: problem opening fuzzed file: %s\n", 
                directed_filename);
        return (0);
    }

    key_index = 0;
    line_count = 0;
    while (fgets(input_string, MAX_INPUT_LENGTH, fp)) {
        line_count++; 

        if (input_string[0] == '\0') {
            continue;
        } else if (input_string[0] == '#') {
            continue;
        }

        argv[0] = strtok(input_string, " ,\t\n");
        /* empty line */
        if (argv[0] == NULL) {
            continue;
        }
        p2packet[key_index] =  strtoul(argv[0], NULL, 16);
        key_index++; 

        argc = 1;
        for (i=1; i<ARGC_MAX; i++) {
            argv[i] = strtok(NULL, " \t\n");

            if (argv[i] == NULL) {
                break;
            } else {
                argc++;
                p2packet[key_index] =  strtoul(argv[i], NULL, 16);
                key_index++; 
            }
        }
    } 

    //packet_dump(" ", p2packet, key_index);

    fclose(fp);
    return (key_index);
}



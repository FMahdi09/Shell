#ifndef PARSER
#define PARSER

#include "job.h"

/* FUNCTION DECLARATIONS */

char** parse_input_string (char* buffer, const char delim);
job* parse_job (char** command);

#endif
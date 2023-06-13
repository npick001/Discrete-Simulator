#pragma once

// compiler directives, will not compile if gen_output is 0
#define SIM_OUTPUT 1
#define DEBUG 0
#define WRITETOFILE 1

#if SIM_OUTPUT
#include <iomanip>
#endif
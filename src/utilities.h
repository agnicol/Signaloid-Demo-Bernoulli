
#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

#include "common.h"

typedef struct CommandLineArguments {
  CommonCommandLineArguments common;

  double airspeed;
  double airspeed_uncertainty;
  double wing_area;
} CommandLineArguments;

/**
 *	@brief	Print out command line usage.
 */
void printUsage(void);

/**
 *	@brief	Get command line arguments.
 *
 *	@param	argc		: argument count from main()
 *	@param	argv		: argument vector from main()
 *	@param	arguments	: Pointer to struct to store arguments
 *	@return			: `kCommonConstantSuccess` if successful, else
 *`kCommonConstantError`
 */
CommonConstantReturnType
getCommandLineArguments(int argc, char *argv[],
                        CommandLineArguments *arguments);

#define DEFAULT_AIRSPEED (30.0)
#define DEFAULT_UNCERTAINTY (0.05)
#define DEFAULT_WING_AREA (1.0)


#include "utilities.h"
#include "common.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void printUsage(void) {
  fprintf(stderr, "Example: Airfoil lift force calculation\n");
  fprintf(stderr, "\n");
  printCommonUsage();
  fprintf(
      stderr,
      "	[-s, --airspeed <Airspeed in meters per second (Default: %.2f)>]\n"
      " [-u, --uncertainty <Percentage uncertainty in airspeed value "
      "(Default: %.2f, range: 0-1.0)>]\n"
      "	[-a, --area <Wing area in square meters (Default: %.2f)>]\n",
      DEFAULT_AIRSPEED, DEFAULT_UNCERTAINTY, DEFAULT_WING_AREA);
  fprintf(stderr, "\n");
}

void setDefaultCommandLineArguments(CommandLineArguments *arguments) {
  assert(arguments != NULL);

/*
 *	Older GCC versions have a bug which gives a spurious warning for the C
 *universal zero initializer `{0}`. Any workaround makes the code less portable
 *or prevents the common code from adding new fields to the
 *`CommonCommandLineArguments` struct. Therefore, we surpress this warning.
 *
 *	See https://gcc.gnu.org/bugzilla/show_bug.cgi?id=53119.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-braces"

  *arguments = (CommandLineArguments){
      .common = (CommonCommandLineArguments){0},
      .airspeed = DEFAULT_AIRSPEED,
      .airspeed_uncertainty = DEFAULT_UNCERTAINTY,
      .wing_area = DEFAULT_WING_AREA,
  };
#pragma GCC diagnostic pop
}

CommonConstantReturnType
getCommandLineArguments(int argc, char *argv[],
                        CommandLineArguments *arguments) {
  const char *airspeedArg = NULL;
  const char *airspeedUncertaintyArg = NULL;
  const char *wingareaArg = NULL;

  assert(arguments != NULL);
  setDefaultCommandLineArguments(arguments);

  DemoOption options[] = {
      {.opt = "s",
       .optAlternative = "airspeed",
       .hasArg = true,
       .foundArg = &airspeedArg,
       .foundOpt = NULL},
      {.opt = "u",
       .optAlternative = "uncertainty",
       .hasArg = true,
       .foundArg = &airspeedUncertaintyArg,
       .foundOpt = NULL},
      {.opt = "a",
       .optAlternative = "area",
       .hasArg = true,
       .foundArg = &wingareaArg,
       .foundOpt = NULL},
      {0},
  };

  if (parseArgs(argc, argv, &arguments->common, options) != 0) {
    fprintf(stderr, "Parsing command line arguments failed\n");
    printUsage();
    return kCommonConstantReturnTypeError;
  }

  if (arguments->common.isHelpEnabled) {
    printUsage();
    exit(EXIT_SUCCESS);
  }

  if ((strcmp(arguments->common.outputFilePath, "") != 0) ||
      arguments->common.isWriteToFileEnabled) {
    fprintf(
        stderr,
        "Error: This application does not support saving outputs to file.\n");
    exit(EXIT_FAILURE);
  }

  if (arguments->common.outputSelect != 0) {
    fprintf(stderr, "Error: Output select option not supported.\n");
    exit(EXIT_FAILURE);
  }

  if (arguments->common.isVerbose) {
    fprintf(stderr, "Error: Verbose mode not supported.\n");
    exit(EXIT_FAILURE);
  }

  if (arguments->common.isBenchmarkingMode) {
    fprintf(stderr, "Error: Benchmarking mode not supported.\n");
    exit(EXIT_FAILURE);
  }

  if (arguments->common.isHelpEnabled) {
    printUsage();
    exit(EXIT_SUCCESS);
  }

  if (airspeedArg != NULL) {
    double airspeed;
    int ret = parseDoubleChecked(airspeedArg, &airspeed);

    if ((ret != kCommonConstantReturnTypeSuccess) || (airspeed <= 0.0)) {
      fprintf(stderr, "Error: The airspeed must be a positive real number.\n");
      printUsage();
      return kCommonConstantReturnTypeError;
    }

    arguments->airspeed = airspeed;
  }

  if (airspeedUncertaintyArg != NULL) {
    double airspeed_uncertainty;
    int ret = parseDoubleChecked(airspeedUncertaintyArg, &airspeed_uncertainty);

    if ((ret != kCommonConstantReturnTypeSuccess) ||
        (airspeed_uncertainty < 0.0) || (airspeed_uncertainty > 1.0)) {
      fprintf(stderr, "Error: The uncertainty must be a real number between "
                      "0.0 and 1.0.\n");
      printUsage();
      return kCommonConstantReturnTypeError;
    }

    arguments->airspeed_uncertainty = airspeed_uncertainty;
  }

  if (wingareaArg != NULL) {
    double wingarea;
    int ret = parseDoubleChecked(wingareaArg, &wingarea);

    if ((ret != kCommonConstantReturnTypeSuccess) || (wingarea <= 0.0)) {
      fprintf(stderr, "Error: The wing area must be a positive real number.\n");
      printUsage();
      return kCommonConstantReturnTypeError;
    }

    arguments->wing_area = wingarea;
  }

  if (strcmp(arguments->common.inputFilePath, "") != 0) {
    fprintf(stderr, "Error: This application does not support input files.\n");
    exit(EXIT_FAILURE);
  }

  return kCommonConstantReturnTypeSuccess;
}

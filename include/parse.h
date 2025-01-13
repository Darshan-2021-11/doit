#ifndef parse_h
#define parse_h

#include "types.h"

/**
 * @brief Load custom config file provided by user.
 * @param config_file Absolute path of the config file.
 * @return Nothing.
 */
void doit_load_config(char *config_file);

/**
 * @brief Load doit tasks from category name.
 * @param config_file Absolute path of the category data file.
 * @return Nothing.
 */
void doit_load_data(char *data_file);

/**
 * @brief Dump config file data if modifications done to config.
 * @param config_file Absolute path of the config file.
 * @return Nothing.
 */
void doit_dump_config(char *config_file);

/**
 * @brief Dump category data file if modifications done to category.
 * @param config_file Absolute path of the category data file.
 * @return Nothing.
 */
void doit_dump_data(char *data_file);

#endif //parse_h

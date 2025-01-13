#ifndef logging_h
#define logging_h

/**
 * @brief Exit abruptly printing a list of variable list argument to stderror.
 * @param errstr Format of the variable list argument to be printed.
 * @return Nothing.
 */
void doit_die(const char *errstr, ...);
/**
 * @brief Printing a list of variable list argument to stderror, stating warning.
 * @param errstr Format of the variable list argument to be printed.
 * @return Nothing.
 */
void doit_warn(const char *errstr, ...);

#endif // logging_h

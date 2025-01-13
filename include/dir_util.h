#ifndef dir_util_h
#define dir_util_h

/**
 * @brief Makes directories if necessary for creation of file.
 * @param file Absolute path of the file to be created.
 * @return Nothing.
 *
 * Always checks from the root of file system partition the
 * directories required until it reaches no other directory separator
 * such as '/' or '\' and creates if the directory is not present.
 */
int doit_mkdir_p(char *file);
#endif //dir_util_h

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "types.h"
#include "dir_util.h"
#include "log_util.h"

//TODO
void
doit_load_config(char *config_file) {
	(void)config_file;
}

//TODO
void
doit_load_data(char *data_file) {

	// does error handling
	doit_mkdir_p(data_file);
	FILE *data = fopen(data_file, "r");
	if (data == NULL) {
		doit_warn("Unable to open file to load data: %s, %s\n", data_file, strerror(errno));
		return;
	}
	fclose(data);
}

//TODO
void
doit_dump_config(char *config_file) {
	(void)config_file;
}

//TODO
void
doit_dump_data(char *data_file) {
	FILE *data = fopen(data_file, "w");
	if (data == NULL)
		doit_die("Unable to open file to dump data: %s, %s\n", data, strerror(errno));

	doit_defrag_tasks_t();

	int flag;
	flag = fwrite(
			&doit_tasks[doit_task_priority],
			sizeof (doit_task_t),
			doit_tasks[doit_task_priority].size,
			data
			);
	if (!flag) goto err;
err:
	fprintf(stderr, "Unable to dump data: %s", data_file);
	fclose(data);
}

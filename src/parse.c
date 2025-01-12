#include <stdio.h>

#include "types.h"
#include "dir_util.h"
#include "log_util.h"

void
load_config(const char *config_file) {
	(void)config_file;
}

void
load_data(const char *data_file) {
	FILE *data = fopen(data_file, "r");
	if (data == NULL)
		die("Unable to open data file to load data: %s\n", data);
	fclose(data);
}

void
dump_config(const char *config_file) {
	(void)config_file;
}
void
dump_data(const char *data_file) {
	FILE *data = fopen(data_file, "w");
	if (data == NULL)
		die("Unable to open data file to dump data: %s\n", data);

	doit_defrag_tasks_t();

	int flag;
	flag = fwrite(
			&tasks,
			sizeof (doit_task_t),
			tasks.size,
			data
			);
	if (!flag) goto err;
err:
	fprintf(stderr, "Unable to dump data: %s", data_file);
	fclose(data);
}

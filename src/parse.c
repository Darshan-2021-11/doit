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

void
doit_load_data(char *data_file) {

	FILE *data = fopen(data_file, "r");
	if (data == NULL) {
		doit_warn("Unable to load data file: %s, %s\n", data_file, strerror(errno));
		doit_init_tasks_t();
		return;
	}

	int16_t nmemb;
	nmemb = fread(
			&doit_tasks[doit_task_priority],
			sizeof (doit_task_t),
			MAXLEN,
			data
			);
	if (nmemb < doit_tasks[doit_task_priority].size)
		fprintf(stderr, "Corrupted data file: %s, %s\n", data_file, strerror(errno));

	fclose(data);
}

//TODO
void
doit_dump_config(char *config_file) {
	(void)config_file;
}

void
doit_dump_data(char *data_file) {

	doit_mkdir_p(data_file);

	FILE *data = fopen(data_file, "w");
	if (data == NULL) {
		doit_die("Unable to write data file: %s, %s\n", data_file, strerror(errno));
		return;
	}

	int16_t nmemb;
	nmemb = fwrite(
			&doit_tasks[doit_task_priority],
			sizeof (doit_task_t),
			doit_tasks[doit_task_priority].size,
			data
			);

	if (nmemb < doit_tasks[doit_task_priority].size)
		fprintf(stderr, "Unable to dump data: %s, %s\n", data_file, strerror(errno));

	fclose(data);
}

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
		for (int priority = 0; priority < MAXPRIORITY; ++priority)
			doit_init_tasks_t(priority);
		return;
	}

	int16_t nmemb;
	nmemb = fread(
			&doit_task_priority,
			sizeof (doit_tasks_t),
			MAXPRIORITY,
			data
			);
	if (nmemb < MAXPRIORITY)
		fprintf(stderr, "Corrupted data file: %s.\n", data_file);

	for (int priority = 0; priority < MAXPRIORITY; ++priority) {
		nmemb = fread(
				&doit_task_list[priority],
				sizeof (doit_task_t),
				doit_task_priority[priority].size,
				data
				);
		if (nmemb < doit_task_priority[priority].size)
			fprintf(stderr, "Corrupted data file: %s.\n", data_file);
	}

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
			&doit_task_priority,
			sizeof (doit_tasks_t),
			MAXPRIORITY,
			data
			);

	if (nmemb < MAXPRIORITY)
		fprintf(stderr, "Corrupted data file: %s.\n", data_file);

	for (int priority = 0; priority < MAXPRIORITY; ++priority) {
		nmemb = fwrite(
				&doit_task_list[priority],
				sizeof (doit_task_t),
				doit_task_priority[priority].size,
				data
				);
		if (nmemb < doit_task_priority[priority].size)
			fprintf(stderr, "Corrupted data file: %s.\n", data_file);
	}

	fclose(data);
}

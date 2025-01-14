#ifndef types_h
#define types_h
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "defines.h"

typedef
struct _doit_task_t {

	time_t
		createdAt;

	uint16_t
		prev,
		parent,
		next,
		child,
		lastChild;

	char name[MAXLEN];

} doit_task_t;

typedef struct _doit_task_alt_t {
	int unused;
	int free_ptr;
} doit_task_alt_t;

typedef
struct _doit_tasks_t {

	uint16_t
		head,
		size,
		n,
		lastChild;

	doit_task_t
		task_list[MAXLEN];

} doit_tasks_t;

extern doit_tasks_t doit_tasks[MAXPRIORITY];
extern int16_t doit_task_priority;

void doit_init_task_t(char name[MAXLEN], uint16_t idx, uint16_t prev, uint16_t parent);
void doit_init_tasks_t(void);
bool doit_check_bounds_tasks_t(uint16_t idx);
uint16_t doit_alloc_task_t(void);
void doit_free_task_t(uint16_t idx);
uint16_t doit_add_task_t(char name[MAXLEN], uint16_t idx, bool child);
void doit_delete_task_t(uint16_t idx);
void doit_print_task_t(uint16_t idx);
void doit_print_tasks_t_helper(uint16_t idx, uint16_t level);
void doit_print_tasks_t(uint16_t idx);
void doit_defrag_tasks_t(void);
#endif //types_h

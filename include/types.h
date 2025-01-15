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
		next,
		parent,
		firstChild,
		lastChild;

	char name[MAXLEN];

} doit_task_t;

typedef struct _doit_task_alt_t {
	uint16_t
		unused,
		nextFree;
} doit_task_alt_t;

typedef
struct _doit_tasks_t {

	uint16_t
		size,
		nextFree,
		firstChild,
		lastChild;

} doit_tasks_t;

extern doit_tasks_t doit_task_priority[MAXPRIORITY];
extern doit_task_t  doit_task_list[MAXPRIORITY][MAXLEN];
extern int16_t      doit_task_current_priority;

void doit_init_task_t(char name[MAXLEN], uint16_t idx, uint16_t prev, uint16_t parent);
void doit_init_tasks_t(uint16_t priority);
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

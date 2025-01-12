#ifndef types_h
#define types_h
#include <stdint.h>
#include <time.h>

#include "defines.h"

typedef
struct _doit_task_t {

	time_t
		createdAt;

	uint16_t
		name,
		priority,
		prev,
		parent,
		next,
		child;

} doit_task_t;

typedef
struct _doit_tasks_t {

	uint16_t
		head,
		size,
		n;

	doit_task_t
		task_list[MAXLEN];

} doit_tasks_t;

extern doit_tasks_t tasks;

void doit_init_tasks_t(void);
void doit_init_task_t(uint16_t idx, uint16_t name, uint16_t priority, uint16_t prev, uint16_t parent);
uint16_t doit_alloc_task_t(void);
void doit_free_task_t(uint16_t idx);
void doit_print_tasks_t(int level);
void doit_defrag_tasks_t(void);
#endif //types_h

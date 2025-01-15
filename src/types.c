#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "log_util.h"

doit_tasks_t doit_task_priority[MAXPRIORITY];
doit_task_t  doit_task_list[MAXPRIORITY][MAXLEN];
int16_t      doit_task_current_priority;

void
doit_init_task_t(char name[MAXLEN], uint16_t idx, uint16_t prev, uint16_t parent) {
	time_t createdAt;
	time(&createdAt);
	{
		doit_task_list[doit_task_current_priority][idx].createdAt = createdAt,
		doit_task_list[doit_task_current_priority][idx].prev = prev,
		doit_task_list[doit_task_current_priority][idx].parent = parent,
		doit_task_list[doit_task_current_priority][idx].next = UINT16_MAX,
		doit_task_list[doit_task_current_priority][idx].firstChild = UINT16_MAX,
		doit_task_list[doit_task_current_priority][idx].lastChild = UINT16_MAX;
		char *dest = doit_task_list[doit_task_current_priority][idx].name,
				 *src  = name;
		while (*src && src - name < MAXLEN - 1) *dest++ = *src++;
		*dest = '\0';
	}
	if (prev != UINT16_MAX)
		doit_task_list[doit_task_current_priority][prev].next = idx;
	else if (parent != UINT16_MAX)
		doit_task_list[doit_task_current_priority][parent].firstChild = idx;
}

void
doit_init_tasks_t(uint16_t priority) {
	doit_task_priority[priority] = (doit_tasks_t) {
		.size = 0,
		.nextFree = 0,
		.firstChild = UINT16_MAX,
		.lastChild = UINT16_MAX,
	};
}

bool
doit_check_bounds_tasks_t(uint16_t idx) {
	return 
		idx >= doit_task_priority[doit_task_current_priority].size ||
		((doit_task_alt_t *)&doit_task_list[doit_task_current_priority][idx])->unused == UINT16_MAX;
}

uint16_t
doit_alloc_task_t(void) {
	if (doit_task_priority[doit_task_current_priority].size == MAXLEN) {
		doit_warn("Stop procastinating, its already too much.\n");
		doit_die( "Attempted to add more task than %d.\n", MAXLEN);
	}
	if (doit_task_priority[doit_task_current_priority].nextFree == doit_task_priority[doit_task_current_priority].size) {
		++doit_task_priority[doit_task_current_priority].size;
		return doit_task_priority[doit_task_current_priority].nextFree++;
	}
	uint16_t idx = doit_task_priority[doit_task_current_priority].nextFree;
	doit_task_priority[doit_task_current_priority].nextFree =
		((doit_task_alt_t *)&doit_task_list[doit_task_current_priority][idx])->nextFree;
	return idx;
}

void
doit_free_task_t(uint16_t idx) {
	uint16_t
		prev = doit_task_list[doit_task_current_priority][idx].prev,
		next = doit_task_list[doit_task_current_priority][idx].next,
		parent = doit_task_list[doit_task_current_priority][idx].parent,
		firstChild = doit_task_list[doit_task_current_priority][idx].firstChild;

	while (firstChild != UINT16_MAX)
		doit_free_task_t(firstChild), firstChild = doit_task_list[doit_task_current_priority][idx].firstChild;

	if (prev != UINT16_MAX) {
		doit_task_list[doit_task_current_priority][prev].next = next;
		if (next != UINT16_MAX)
			doit_task_list[doit_task_current_priority][next].prev = prev;
		else if (parent != UINT16_MAX)
			doit_task_list[doit_task_current_priority][parent].lastChild = prev;
		else
			doit_task_priority[doit_task_current_priority].lastChild = prev;
	} else if (parent != UINT16_MAX) {
		doit_task_list[doit_task_current_priority][parent].firstChild = next;
		if (next != UINT16_MAX)
			doit_task_list[doit_task_current_priority][next].prev = prev;
	} else {
		if (next != UINT16_MAX) {
			doit_task_list[doit_task_current_priority][next].prev = prev,
			doit_task_priority[doit_task_current_priority].firstChild = next;
		}
		else {
			doit_init_tasks_t(doit_task_current_priority);
			return;
		}
	}
	*(doit_task_alt_t *)&doit_task_list[doit_task_current_priority][idx] =
		(doit_task_alt_t) {
			UINT16_MAX,
			doit_task_priority[doit_task_current_priority].nextFree,
		};
	doit_task_priority[doit_task_current_priority].nextFree = idx;
}

uint16_t
doit_add_task_t(char name[MAXLEN], uint16_t idx, bool child) {
	if (
			idx != UINT16_MAX &&
			doit_check_bounds_tasks_t(idx)
		 ) {
		doit_die("Task with id: %d, does not exist.\n", idx);
		return UINT16_MAX;
	}
	uint16_t allocated = doit_alloc_task_t();
	if (!child) {
		if (idx != UINT16_MAX) idx = doit_task_list[doit_task_current_priority][idx].parent;
	}
	if (idx == UINT16_MAX) {
		doit_init_task_t(
				name,
				allocated,
				doit_task_priority[doit_task_current_priority].lastChild,
				UINT16_MAX
				);

		doit_task_priority[doit_task_current_priority].lastChild = allocated;
		if (doit_task_priority[doit_task_current_priority].firstChild == UINT16_MAX)
			doit_task_priority[doit_task_current_priority].firstChild = allocated;
	} else {
		doit_init_task_t(
				name,
				allocated,
				doit_task_list[doit_task_current_priority][idx].lastChild,
				idx
				);
		doit_task_list[doit_task_current_priority][idx].lastChild = allocated;
	}
	printf("Task added successfully.\n");
	return allocated;
}

void
doit_delete_task_t(uint16_t idx) {
	if (doit_check_bounds_tasks_t(idx)) {
		doit_die("Task with id: %d, does not exist.\n", idx);
		return;
	}
	doit_free_task_t(idx);
	printf("Task(and its subtasks) removed successfully.\n");
}

void
doit_print_task_t(uint16_t idx) {
	struct tm *temp = localtime(&doit_task_list[doit_task_current_priority][idx].createdAt);
	char buffer[32];
	strftime(
			buffer,
			sizeof(buffer),
			"%a,%d/%m/%Y %H:%M",
			temp
			);
	printf(
			"(%hu) %s\t%s\n",
			idx,
			buffer,
			doit_task_list[doit_task_current_priority][idx].name
			);
}

/**
 * Resources:
 * 1. https://en.wikipedia.org/wiki/Box-drawing_characters
 * 2. https://stackoverflow.com/questions/4494306/drawing-tables-in-terminal-using-ansi-box-characters
 */
#define BEGIN_GRAPHIC "\e(0"
#define END_GRAPHIC "\e(B"
#define BEGIN_BOLD  "\e[1m"
#define END_BOLD    "\e[0m"
#define CNTD        "\x71"
#define LEVEL_CNTD  BEGIN_GRAPHIC /* BEGIN_BOLD */ "\x78\t"      /* END_BOLD */ END_GRAPHIC
#define LEVEL_DCNTD BEGIN_GRAPHIC /* BEGIN_BOLD */ " \t"         /* END_BOLD */ END_GRAPHIC
#define NEXT        BEGIN_GRAPHIC /* BEGIN_BOLD */ "\x74"   CNTD /* END_BOLD */ END_GRAPHIC
#define LAST        BEGIN_GRAPHIC /* BEGIN_BOLD */ "\x6D"   CNTD /* END_BOLD */ END_GRAPHIC
static const char *next = NEXT;
static const char *last = LAST;
static const char *lvl_cntd = LEVEL_CNTD;
static const char *lvl_dcntd = LEVEL_DCNTD;
static bool list[MAXLEN];

void
doit_print_tasks_t_helper(uint16_t idx, uint16_t level) {
	list[level] = doit_task_list[doit_task_current_priority][idx].next != UINT16_MAX;

	for (uint16_t i = 0; i < level; ++i)
		printf("%s", list[i] ? lvl_cntd : lvl_dcntd);
	printf("%s", list[level] ? next : last);
	doit_print_task_t(idx);

	if (doit_task_list[doit_task_current_priority][idx].firstChild != UINT16_MAX)
		doit_print_tasks_t_helper(doit_task_list[doit_task_current_priority][idx].firstChild, level + 1);
	if (doit_task_list[doit_task_current_priority][idx].next != UINT16_MAX)
		doit_print_tasks_t_helper(doit_task_list[doit_task_current_priority][idx].next, level);
}
#undef END_GRAPHIC
#undef BEGIN_BOLD
#undef END_BOLD
#undef CNTD
#undef LEVEL_CNTD
#undef LEVEL_DCNTD
#undef NEXT
#undef LAST

void
doit_print_tasks_t(uint16_t idx) {
	if (doit_task_priority[doit_task_current_priority].firstChild == UINT16_MAX)
		doit_die("No task exists with priority %d.\n", doit_task_current_priority);
	else if (idx != UINT16_MAX && doit_check_bounds_tasks_t(idx))
		doit_die("Task with id: %d, does not exist with priority %d.\n", idx, doit_task_current_priority);

	if (idx == UINT16_MAX) {
		doit_print_tasks_t_helper(doit_task_priority[doit_task_current_priority].firstChild, 0);
	}
	else {
		doit_print_task_t(idx);
		if (doit_task_list[doit_task_current_priority][idx].firstChild != UINT16_MAX)
			doit_print_tasks_t_helper(doit_task_list[doit_task_current_priority][idx].firstChild, 0);
	}
}

void
doit_defrag_tasks_t(void) {
	if (!doit_task_priority[doit_task_current_priority].size) return;

	uint16_t l = 0, r = doit_task_priority[doit_task_current_priority].size - 1, t;
	while (l <= r) {
		if (((doit_task_alt_t *)&doit_task_list[doit_task_current_priority][r])->unused == UINT16_MAX)
			--r;
		else if (((doit_task_alt_t *)&doit_task_list[doit_task_current_priority][l])->unused != UINT16_MAX)
			++l;
		else {
			{
				doit_task_list[doit_task_current_priority][l].createdAt = doit_task_list[doit_task_current_priority][r].createdAt,
				doit_task_list[doit_task_current_priority][l].prev = doit_task_list[doit_task_current_priority][r].prev,
				doit_task_list[doit_task_current_priority][l].parent = doit_task_list[doit_task_current_priority][r].parent,
				doit_task_list[doit_task_current_priority][l].next = doit_task_list[doit_task_current_priority][r].next,
				doit_task_list[doit_task_current_priority][l].firstChild = doit_task_list[doit_task_current_priority][r].firstChild,
				doit_task_list[doit_task_current_priority][l].lastChild = doit_task_list[doit_task_current_priority][r].lastChild;
				char *dest = doit_task_list[doit_task_current_priority][l].name,
						 *src  = doit_task_list[doit_task_current_priority][r].name;
				while (*src && src - doit_task_list[doit_task_current_priority][r].name < MAXLEN - 1) *dest++ = *src++;
				*dest = '\0';
			}

			if ((t = doit_task_list[doit_task_current_priority][l].prev) != UINT16_MAX)
				doit_task_list[doit_task_current_priority][t].next = l;
			if ((t = doit_task_list[doit_task_current_priority][l].next) != UINT16_MAX)
				doit_task_list[doit_task_current_priority][t].prev = l;

			if ((t = doit_task_list[doit_task_current_priority][l].parent) != UINT16_MAX) {
				if (doit_task_list[doit_task_current_priority][t].firstChild == r)
					doit_task_list[doit_task_current_priority][t].firstChild = l;
				if (doit_task_list[doit_task_current_priority][t].lastChild == r)
					doit_task_list[doit_task_current_priority][t].lastChild = l;
			} else {
				if (doit_task_priority[doit_task_current_priority].firstChild == r)
					doit_task_priority[doit_task_current_priority].firstChild = l;
				if (doit_task_priority[doit_task_current_priority].lastChild == r)
					doit_task_priority[doit_task_current_priority].lastChild = l;
			}

			++l, --r;
		}
	}
	doit_task_priority[doit_task_current_priority].size = l,
	doit_task_priority[doit_task_current_priority].nextFree = l;
}

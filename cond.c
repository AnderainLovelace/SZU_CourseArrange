#include <string.h>
#include <stdlib.h>
#include "cond.h"

#define CONDITION_SIZE		128

typedef int(*cond_func)(void*,const COURSE_DATA *);

typedef struct{
	cond_func		check;
	void			*ext;
}CONDITION;


CONDITION		conditions[CONDITION_SIZE];
int				conditions_count;

int ignore_day		(void * ext,const COURSE_DATA * c_data)
{
	int			day = *(int*)ext,i;
	DAY_DATA	*d_data = c_data->day_data;

	for (i=0;i<c_data->day_count;++i) {
		if (day==d_data[i].index) return 1;
	}

	return 0;
}

int ignore_teacher	(void * ext,const COURSE_DATA * c_data)
{
	return strcmp(c_data->teacher,(const char*)ext)==0;
}

int ignore_class	(void * ext,const COURSE_DATA * c_data)
{
	int			cls = *(int*)ext,i;
	DAY_DATA	*d_data = c_data->day_data;

	for (i=0;i<c_data->day_count;++i) {
		if (cls >= d_data[i].start && cls < d_data[i].start+d_data[i].size)
			return 1;
	}

	return 0;
}

void add_ignore_day (int day)
{
	CONDITION * cond = &conditions[conditions_count++];

	cond->ext = (void*)calloc(sizeof(int),1);
	*((int*)(cond->ext)) = day;

	cond->check = ignore_day;
}

void add_ignore_teacher (const char * name)
{
	CONDITION * cond = &conditions[conditions_count++];

	cond->ext = (void*)strdup(name);

	cond->check = ignore_teacher;
}

void add_ignore_class (int cls)
{
	CONDITION * cond = &conditions[conditions_count++];

	cond->ext = (void*)calloc(sizeof(int),1);
	*((int*)(cond->ext)) = cls;

	cond->check = ignore_class;
}

int cond_check(const COURSE_DATA * c_data)
{
	int i,r;
	for (i=0;i<conditions_count;++i) {
		r = conditions[i].check(conditions[i].ext,c_data);
		if (r) return 1;
	}
	return 0;
}
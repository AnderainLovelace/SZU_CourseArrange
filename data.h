#ifndef _DATA_H_

#define _DATA_H_

#include "list.h"

typedef struct {
	int		start;
	int		size;
	int		index;
}DAY_DATA;

typedef struct {
	const char	*	id;
	const char	*	name;
	const char	*	school;
	const char	*	teacher;
	int				day_count;
	DAY_DATA	*	day_data;
}COURSE_DATA;
//
#define LOAD_CANNOTOPEN		-1
#define LOAD_SYNTAX			-2
#define LOAD_SUCCESS		0

// load csv file
int load_data (LIST * list,const char * file_name);


#endif
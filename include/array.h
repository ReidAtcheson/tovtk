#ifndef ARRAY_H_
#define ARRAY_H_

#include "types.h"

typedef struct{
	void* data;
	Real (*get_val)(void*,Index);
	Index (*get_len)(void*);
}oned_array;

typedef struct{
	void* data;
	Real (*get_val)(void*,Index,Index);
	Index (*get_rows)(void*);
	Index (*get_cols)(void*);
}twod_array;




#endif

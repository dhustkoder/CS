#ifndef CS_COMMON_H_
#define CS_COMMON_H_
#include <stdlib.h>


#define UNUSED(x) ((void)x)


typedef int(*CmpFun)(const void*, const void*);
typedef const void*(*ConstSearchFun)(const void*, int, int, const void*, CmpFun);
typedef const void*(*SearchFun)(void*, int, int, const void*, CmpFun);
typedef void(*SortFun)(void*, int, int, CmpFun);


static inline int* make_array_from_strings(const char* const* const strs, const int size)
{
	int* const a = malloc(sizeof(int) * size);

	if (a == NULL) {
		fprintf(stderr, "Couldn't allocate memory");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < size; ++i)
		a[i] = strtol(strs[i], NULL, 0);

	return a;
}


static inline int cmp_int(const void* a, const void* b)
{
	return *((int*)a) - *((int*)b);
}


static inline void print_int_array(const int* const a, const int size)
{
	for (int i = 0; i < size; ++i)
		printf("[%d] = %d\n", i, a[i]);
}

static inline void print_str_array(const char* const * a, const int size)
{
	for (int i = 0; i < size; ++i)
		printf("[%d] = %s\n", i, a[i]);
}


#ifdef CSDEBUG

#define print_array(array, size)                         \
        _Generic((array),                                \
        const int*: print_int_array,                     \
        int*: print_int_array,                           \
        const char**: print_str_array,                   \
        char**: print_str_array)(array, size)


#else

// do nothing
#define print_array(...)

#endif


static inline int sort_test(const int argc,
                            const char* const * const argv,
                            const SortFun sortfun)
{
	if (argc < 3) {
		fprintf(stderr, "Usage: %s [list]\n", argv[0]);
		return EXIT_FAILURE;
	}

	const int size = argc - 1;
	int* const data = make_array_from_strings(argv + 1, size);

#ifdef CSDEBUG
	printf("UNSORTED:\n");
	print_array(data, size);
#endif
	
	sortfun(data, size, sizeof(int), cmp_int);
	
#ifdef CSDEBUG
	printf("SORTED:\n");
	print_array(data, size);
#endif

	free(data);
	return EXIT_SUCCESS;
}


static inline int search_test_impl(const int argc,
                                   const char* const * argv,
                                   const SearchFun searchfun)
{
	if (argc < 4) {
		fprintf(stderr, "Usage: %s [list] [target]\n", argv[0]);
		return EXIT_FAILURE;
	}

	const int size = argc - 2;
	const int target = strtol(argv[argc - 1], NULL, 0);
	int* const data = make_array_from_strings(argv + 1, size);

	const void* found = searchfun(data, size, sizeof(int), &target, cmp_int);


#ifdef CSDEBUG
	printf("ARRAY:\n");
	print_array(data, size);
#endif

	if (found != NULL) {
		const int index = (int) (((char*)found) - ((char*)data)) / sizeof(int);
		printf("%d FOUND AT INDEX %d\n", target, index);
	} else {
		printf("%d NOT FOUND\n", target);
	}

	free(data);
	return EXIT_SUCCESS;
}


#define search_test(argc, argv, searchfun)                            \
	_Generic((searchfun),                                         \
	ConstSearchFun: search_test_impl,                             \
	SearchFun: search_test_impl)(argc, argv, (SearchFun)searchfun)



#endif


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>

#include "woofi/arraylist.h"
#ifdef WITH_TEST
# include <criterion/criterion.h>
#endif

arraylist *arraylist_create() {
    arraylist *new_list = NULL;

    new_list = malloc(sizeof(*new_list));
    if (new_list == NULL) {
	return NULL;
    }

    new_list->count = 0;
    new_list->length = INITIALI_ARRAYLIST_SIZE;
    new_list->element = calloc(new_list->length, sizeof(*(new_list->element)));
    return new_list;
}

/**
 * Expand the list passed in argument to half of it's size
 * @param list a non null pointer to a list
 * @return 0 if list has grow
 *        -1 on error, if it failed to allocate requested memory (see errno)
 */
static int arraylist_grow(arraylist *list) {
    if (list == NULL) {
	return -1;
    }

    list->length += list->length / 2;
    int *new_elements = realloc(list->element, list->length * sizeof(*(list->element)));
    if (new_elements == NULL) {
	return -1;
    }
    else {
        list->element = new_elements;
    }

    return 0;
}

void arraylist_delete(arraylist *list) {
    assert(list);
    free(list->element);
    free(list);
}

int arraylist_is_empty(const arraylist *list) {
    // FIXME: Assert
    return list->count == 0;
}

void arraylist_print(const arraylist *list) {

    // FIXME: Assert
    printf("[");
    for (size_t i = 0; i < list->count; ++i) {
	printf("%d, ", list->element[i]);
    }
    printf("]\n");
}

int arraylist_insert_front(arraylist *list, int value) {
    int error;

    assert(list);

    if (list->count == list->length) {
	error = arraylist_grow(list);
	if (error == -1) {
	    return -1;
	}
    }

    for (int i = list->count; i > 0; i--) {
	list->element[i] = list->element[i - 1];
    }

    list->count++;
    list->element[0] = value;

    return 0;
}

int arraylist_insert_last(arraylist *list, int value) {
    int error;

    assert(list);

    if (list->count == list->length) {
	error = arraylist_grow(list);
	if (error == -1) {
	    return -1;
	}
    }

    list->element[list->count] = value;
    list->count++;

    return 0;
}

int arraylist_insert_at(arraylist *list, size_t index, int value) {
    int error;

    assert(list);

    if (list->count == list->length) {
	error = arraylist_grow(list);
	if (error == -1) {
	    return -1;
	}
    }

    if (index > list->count) {
	index = list->count;
    }
    for (size_t i = list->count; i > index; --i) {
	list->element[i] = list->element[i - 1];
    }

    list->count++;
    list->element[index] = value;

    return 0;
}

int arraylist_remove_value(arraylist *list, int value) {
    size_t i = 0;

    assert(list);

    while (list->element[i] != value && i < list->count) {
	i++;
    }

    if (i < list->count) {
	list->count--;
	for (;i < list->count; i++) {
	    list->element[i] = list->element[i + 1];
	}
	return 1;
    }

    return 0;
}

int arraylist_remove_at(arraylist *list, size_t index) {
    assert(list);

    if (arraylist_is_empty(list)){
	return 0;
    }

    if (index >= list->count) {
	index = list->count - 1;
    }

    for (size_t i = index; i < list->count; ++i) {
	list->element[i] = list->element[i + 1];
    }

    list->count--;

    return 1;
}

int arraylist_contains(const arraylist *list, int value) {
    assert(list);

    for (size_t i = 0; i < list->count; ++i) {
	if (list->element[i] == value) {
	    return 1;
	}
    }

    return 0;
}

int arraylist_fast_get(const arraylist *list, size_t index) {
    assert(list);
    assert(index < list->count);
    return list->element[index];
}

int arraylist_get(const arraylist *list, size_t index, int *found) {
    assert(list);

    if (index >= list->count) {
	if (found) {
	    *found = 0;
	}
	return 0;
    }

    if (found) {
	*found = 1;
    }

    return list->element[index];
}

int arraylist_count(const arraylist *list) {
    assert(list);

    return list->count;
}

#ifdef WITH_TEST

Test(ArrayList, create) {
    arraylist *list = arraylist_create();
    cr_assert(list);
    cr_assert(arraylist_is_empty(list));
    cr_assert(arraylist_count(list) == 0);

    arraylist_delete(list);
}

Test(ArrayList, grow) {
    arraylist *list = NULL;

    cr_assert(arraylist_grow(list) == -1);

    list = arraylist_create();
    cr_assert(arraylist_grow(list) == 0);
    cr_assert(list->length == 150);
    cr_assert(arraylist_grow(list) == 0);
    cr_assert(list->length == 225);

    arraylist_delete(list);
}

Test(ArrayList, insert_front) {
    int rc = 0;
    int found = 0;
    arraylist *list = arraylist_create();

    rc = arraylist_insert_front(list, 42);
    cr_assert(rc != -1);
    rc = arraylist_get(list, 0, &found);
    cr_assert(found == 1);
    cr_assert(rc == 42);
    cr_assert(arraylist_count(list) == 1);

    rc = arraylist_insert_front(list, 24);
    cr_assert(rc != -1);
    rc = arraylist_get(list, 0, &found);
    cr_assert(found == 1);
    cr_assert(rc == 24);
    cr_assert(arraylist_count(list) == 2);

    rc = arraylist_get(list, 1, &found);
    cr_assert(found == 1);
    cr_assert(rc == 42);

    arraylist_delete(list);
}

Test(ArrayList, insert_last) {
    int rc = 0;
    int found = 0;
    arraylist *list = arraylist_create();

    rc = arraylist_insert_last(list, 42);
    cr_assert(rc != -1);
    rc = arraylist_get(list, 0, &found);
    cr_assert(found == 1);
    cr_assert(rc == 42);
    cr_assert(arraylist_count(list) == 1);

    rc = arraylist_insert_last(list, 24);
    cr_assert(rc != -1);
    rc = arraylist_get(list, 1, &found);
    cr_assert(found == 1);
    cr_assert(rc == 24);
    cr_assert(arraylist_count(list) == 2);

    rc = arraylist_get(list, 0, &found);
    cr_assert(found == 1);
    cr_assert(rc == 42);

    arraylist_delete(list);
}

Test(ArrayList, insert_at_start) {
    int rc = 0;
    int found = 0;
    arraylist *list = arraylist_create();

    rc = arraylist_insert_at(list, 0, 42);
    cr_assert(rc != -1);
    rc = arraylist_get(list, 0, &found);
    cr_assert(found == 1);
    cr_assert(rc == 42);
    cr_assert(arraylist_count(list) == 1);

    rc = arraylist_insert_at(list, 0, 24);
    cr_assert(rc != -1);
    rc = arraylist_get(list, 0, &found);
    cr_assert(found == 1);
    cr_assert(rc == 24);
    cr_assert(arraylist_count(list) == 2);

    rc = arraylist_get(list, 1, &found);
    cr_assert(found == 1);
    cr_assert(rc == 42);

    arraylist_delete(list);
}

Test(ArrayList, insert_at_end) {
    int rc = 0;
    int found = 0;
    arraylist *list = arraylist_create();

    rc = arraylist_insert_at(list, 0, 42);
    cr_assert(rc != -1);
    rc = arraylist_get(list, 0, &found);
    cr_assert(found == 1);
    cr_assert(rc == 42);
    cr_assert(arraylist_count(list) == 1);

    rc = arraylist_insert_at(list, 1, 24);
    cr_assert(rc != -1);
    rc = arraylist_get(list, 1, &found);
    cr_assert(found == 1);
    cr_assert(rc == 24);
    cr_assert(arraylist_count(list) == 2);

    rc = arraylist_get(list, 0, &found);
    cr_assert(found == 1);
    cr_assert(rc == 42);

    rc = arraylist_insert_at(list, 10, 12);
    cr_assert(rc != -1);
    rc = arraylist_get(list, 2, &found);
    cr_assert(found == 1);
    cr_assert(rc == 12);
    cr_assert(arraylist_count(list) == 3);

    rc = arraylist_get(list, 1, &found);
    cr_assert(found == 1);
    cr_assert(rc == 24);

    arraylist_delete(list);
}

Test(ArrayList, insert_at_middle) {
    int rc = 0;
    int found = 0;
    arraylist *list = arraylist_create();

    rc = arraylist_insert_last(list, 42);
    rc = arraylist_insert_last(list, 24);
    rc = arraylist_insert_last(list, 12);
    cr_assert(arraylist_count(list) == 3);

    rc = arraylist_insert_at(list, 1, 12);
    cr_assert(rc != -1);
    rc = arraylist_get(list, 0, &found);
    cr_assert(found == 1);
    cr_assert(rc == 42);
    cr_assert(arraylist_count(list) == 4);

    rc = arraylist_get(list, 1, &found);
    cr_assert(found == 1);
    cr_assert(rc == 12);

    rc = arraylist_get(list, 2, &found);
    cr_assert(found == 1);
    cr_assert(rc == 24);

    arraylist_delete(list);
}

Test(ArrayList, remove_value) {
    int rc = 0;
    int found = 0;
    arraylist *list = arraylist_create();

    rc = arraylist_insert_front(list, 42);
    rc = arraylist_remove_value(list, 42);
    cr_assert(rc != -1);
    cr_assert(arraylist_is_empty(list));
    cr_assert(arraylist_count(list) == 0);

    rc = arraylist_insert_front(list, 42);
    rc = arraylist_insert_front(list, 24);
    rc = arraylist_remove_value(list, 42);
    cr_assert(rc != -1);
    rc = arraylist_get(list, 0, &found);
    cr_assert(rc == 24);
    cr_assert(arraylist_count(list) == 1);

    rc = arraylist_insert_front(list, 42);
    rc = arraylist_remove_value(list, 24);
    cr_assert(rc != -1);
    rc = arraylist_get(list, 0, &found);
    cr_assert(rc == 42);
    cr_assert(arraylist_count(list) == 1);

    arraylist_delete(list);
}

Test(ArrayList, remove_at_empty) {
    int rc = 0;
    arraylist *list = arraylist_create();

    rc = arraylist_insert_front(list, 42);
    rc = arraylist_remove_at(list, 0);
    cr_assert(rc != -1);
    cr_assert(arraylist_is_empty(list));

    arraylist_delete(list);
}

Test(ArrayList, remove_at_start) {
    int rc = 0;
    int found = 0;
    arraylist *list = arraylist_create();

    rc = arraylist_insert_front(list, 42);
    rc = arraylist_insert_front(list, 24);
    rc = arraylist_remove_at(list, 0);
    cr_assert(rc != -1);
    cr_assert(arraylist_count(list) == 1);
    rc = arraylist_get(list, 0, &found);
    cr_assert(found == 1);
    cr_assert(rc == 42);

    arraylist_delete(list);
}

Test(ArrayList, remove_at_end) {
    int rc = 0;
    int found = 0;
    arraylist *list = arraylist_create();

    rc = arraylist_insert_front(list, 42);
    rc = arraylist_insert_front(list, 24);
    rc = arraylist_remove_at(list, 1);
    cr_assert(rc != -1);
    cr_assert(arraylist_count(list) == 1);
    rc = arraylist_get(list, 0, &found);
    cr_assert(found == 1);
    cr_assert(rc == 24);

    arraylist_delete(list);
}

Test(ArrayList, remove_at_middle) {
    int rc = 0;
    int found = 0;
    arraylist *list = arraylist_create();

    rc = arraylist_insert_front(list, 42);
    rc = arraylist_insert_front(list, 24);
    rc = arraylist_insert_front(list, 12);
    rc = arraylist_remove_at(list, 1);
    cr_assert(rc != -1);
    cr_assert(arraylist_count(list) == 2);
    rc = arraylist_get(list, 0, &found);
    cr_assert(found == 1);
    cr_assert(rc == 12);
    rc = arraylist_get(list, 1, &found);
    cr_assert(found == 1);
    cr_assert(rc == 42);

    arraylist_delete(list);
}

Test(ArrayList, contains) {
    int rc = 0;
    arraylist *list = arraylist_create();

    rc = arraylist_contains(list, 1);
    cr_assert(rc == 0);

    rc = arraylist_insert_last(list, 42);
    rc = arraylist_insert_last(list, 24);
    rc = arraylist_insert_last(list, 12);
    rc = arraylist_insert_last(list, 6);
    rc = arraylist_insert_last(list, 3);
    rc = arraylist_contains(list, 42);
    cr_assert(rc == 1);
    rc = arraylist_contains(list, 3);
    cr_assert(rc == 1);
    rc = arraylist_contains(list, 12);
    cr_assert(rc == 1);

    rc = arraylist_remove_at(list, 0);
    rc = arraylist_contains(list, 42);
    cr_assert(rc == 0);
    rc = arraylist_contains(list, 24);
    cr_assert(rc == 1);
    rc = arraylist_contains(list, 3);
    cr_assert(rc == 1);

    arraylist_delete(list);
}

Test(ArrayList, fast_get) {
    int rc = 0;
    arraylist *list = arraylist_create();

    arraylist_insert_last(list, 42);
    rc = arraylist_fast_get(list, 0);
    cr_assert(rc == 42);

    arraylist_insert_last(list, 24);
    rc = arraylist_fast_get(list, 1);
    cr_assert(rc == 24);
    rc = arraylist_fast_get(list, 0);
    cr_assert(rc == 42);

    arraylist_delete(list);
}

Test(ArrayList, get_empty) {
    int found = 0;
    arraylist *list = arraylist_create();

    arraylist_get(list, 0, &found);
    cr_assert(found == 0);

    arraylist_delete(list);
}

Test(ArrayList, get) {
    int rc = 0;
    int found = 0;
    arraylist *list = arraylist_create();

    arraylist_insert_last(list, 42);
    rc = arraylist_get(list, 0, &found);
    cr_assert(found);
    cr_assert(rc == 42);

    arraylist_insert_last(list, 24);
    rc = arraylist_get(list, 1, &found);
    cr_assert(found);
    cr_assert(rc == 24);
    rc = arraylist_get(list, 0, &found);
    cr_assert(found);
    cr_assert(rc == 42);

    arraylist_insert_last(list, 12);
    rc = arraylist_get(list, 1, &found);
    cr_assert(found);
    cr_assert(rc == 24);

    arraylist_remove_at(list, 0);
    rc = arraylist_get(list, 1, &found);
    cr_assert(found);
    cr_assert(rc == 12);

    arraylist_delete(list);
}

#endif

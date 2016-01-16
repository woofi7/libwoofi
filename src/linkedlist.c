#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define not !

struct list {
	struct list *next;
	int element;
};

struct dynamicList {
	struct list *first;
	struct list *last;
	int size;
};

/**
 * Chack if the list is empty
 * Return 0 if not empty
 * Return 1 if empty
 */
int dynamlicList_is_empty(struct dynamicList *my_list) {
	return my_list->size == 0;
}

/**
 * Print the vector at format [X,Y,Z]\n
 */
void dynamicList_print(struct dynamicList *my_list) {
	struct list *tempo = my_list->first;
	int i = 0;

	printf("[");
	if (not dynamlicList_is_empty(my_list)) {
		do {
			printf("%d, ", tempo->element);
			tempo = tempo->next;
			i++;
		} while(i < my_list->size);
	}
	printf("]\n");
}

/**
 * Allocate and initialize a new list and return it
 */
struct dynamicList *dynamicList_create() {
	struct dynamicList *new_list = NULL;

	new_list = malloc(1);
	if (new_list == NULL) {
		return NULL;
	}

	return new_list;
}

/**
 * Allocate and initialize a new element of the list and return it
 */
struct list *dynamicList_create_element(int value) {
	struct list *new_list = NULL;

	new_list = malloc(1);
	if (new_list == NULL) {
		return NULL;
	}

	new_list->element = value;
	return new_list;
}

/**
 * Insert element at the start of the list
 * Return 0 if element was added to the list
 *	     -1 on error, if it failed to allocate requested memory (see errno)
 */
int dynamicList_insert_front(struct dynamicList *my_list, int value) {
	struct list *new_list = dynamicList_create_element(value);
	if (new_list == NULL) {
		return -1;
	}

	new_list->next = my_list->first;
	my_list->first = new_list;

	if (my_list->last == NULL) {
		my_list->last = new_list;
	}

	my_list->size++;

	return 0;
}

/**
 * Insert element at the end of the list
 * Return 0 if element was added to the list
 *	     -1 on error, if it failed to allocate requested memory (see errno)
 */
int dynamicList_insert_last(struct dynamicList *my_list, int value) {
	struct list *new_list = dynamicList_create_element(value);
	if (new_list == NULL) {
		return -1;
	}

	if (my_list->last != NULL) {
		my_list->last->next = new_list;
	}
	else {
		my_list->first = new_list;
	}

	my_list->last = new_list;
	my_list->size++;

	return 0;
}

/**
 * Insert element at the index of the list
 * Return 0 if element was added to the list
 *	     -1 on error, if it failed to allocate requested memory (see errno)
 */
int dynamicList_insert_at(struct dynamicList *my_list, int index, int value) {
	struct list *tempo = my_list->first;
	struct list *new_list = dynamicList_create_element(value);
	if (new_list == NULL) {
		return -1;
	}

	if (not dynamlicList_is_empty(my_list)) {
		if (index > my_list->size) {
			index = my_list->size;
		}

		for (int i = 1; i < index; ++i) {
			tempo = tempo->next;
		}

		new_list->next = tempo->next;
		tempo->next = new_list;
	}
	else {
		my_list->first = new_list;
		my_list->last = new_list;
	}

	my_list->size++;

	return 0;
}

/**
 * Remove the value from the list
 * Return 0 if element was removed to the list
 */
int dynamicList_remove_value(struct dynamicList *my_list, int value) {
	struct list *previous;
	struct list *tempo = my_list->first;

	if (not dynamlicList_is_empty(my_list)) {
		for (int i = 0; tempo->element != value; ++i) {
			previous = tempo;
			tempo = tempo->next;

			if (i == my_list->size) {
				return 0;
			}
		}

		if (tempo == my_list->last) {
			my_list->last = previous;
			my_list->last->next = NULL;
		}
		else {
			previous->next = tempo->next;
		}

		free(tempo);
		my_list->size--;
	}

	return 0;
}

/**
 * Remove the value at the index from the list
 * Return 0 if element was removed to the list
 */
int dynamicList_remove_at(struct dynamicList *my_list, int index) {
    struct list *previous;
    struct list *tempo = my_list->first;

        if (not dynamlicList_is_empty(my_list)) {
            if (index == 0) {
                my_list->first = my_list->first->next;
            }
            else {
				if (index > my_list->size) {
					index = my_list->size;
            	}
				
				for (int i = 0; i < index; ++i) {
					previous = tempo;
					tempo = tempo->next;
				}

				previous->next = tempo->next;
            }
            free(tempo);
            my_list->size--;
        }

        return 0;
}

/**
 * Count the number of element in list and return it
 */
int dynamicList_count(struct dynamicList *my_list) {
	return my_list->size;
}

/**
 * Check if the value is in list
 * Return 0 if the value is not in list
 *        1 if the value is in list
 */
int dynamicList_contains(struct dynamicList *my_list, int value) {
	struct list *tempo = my_list->first;

	do {
		if(tempo->element == value) {
			return 1;
		}

		tempo = tempo->next;
	} while (tempo != my_list->last);

    return 0;
}

/**
 * Free all used memory by the list
 */
void dynamicList_delete(struct dynamicList *my_list) {
	struct list *previous;
	struct list *tempo = my_list->first;

	if (not dynamlicList_is_empty(my_list)) {
		for (int i = 0; i < my_list->size; ++i) {
			previous = tempo;
			tempo = tempo->next;

			free(previous);
		}

		free(my_list);
	}

}


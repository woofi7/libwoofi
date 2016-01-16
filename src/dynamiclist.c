#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct dynamicList {
	struct dynamicList *next;
	int element;
};

/**
 * Chack if the list is empty
 * Return 0 if not empty
 * Return 1 if empty
 */
int dynamlicList_is_empty(struct dynamicList *my_list) {
	return my_list == NULL;
}

/**
 * Print the vector at format [X,Y,Z]\n
 */
void dynamicList_print(struct dynamicList *my_list) {
	printf("[");
	while(my_list != NULL) {
		printf("%d, ", my_list->element);
		my_list = my_list->next;
	}
	printf("]\n");
}

/**
 * Allocate and initialize a new list and return it
 */
struct dynamicList *dynamicList_create(int value) {
	struct dynamicList *new_list = NULL;

	new_list = calloc(1, sizeof(struct dynamicList));
	if (new_list == NULL) {
		return NULL;
	}

	new_list->next = NULL;
	new_list->element = value;

	return new_list;
}

/**
 * Insert element at the start of the list and return the head
 * Return NULL on error, if it failed to allocate requested memory (see errno)
 */
struct dynamicList *dynamicList_insert_front(struct dynamicList *my_list, int value) {
	struct dynamicList *new_list = dynamicList_create(value);
	if (new_list == NULL) {
		return NULL;
	}

	new_list->next = my_list;

	return new_list;
}

/**
 * Insert element at the end of the list and return the head
 * Return NULL on error, if it failed to allocate requested memory (see errno)
 */
struct dynamicList *dynamicList_insert_last(struct dynamicList *my_list, int value) {
	struct dynamicList *head = my_list;
	struct dynamicList *new_list = dynamicList_create(value);
	if (new_list == NULL) {
		return NULL;
	}

	if (my_list == NULL) {
		head = new_list;
	}
	else {
		while(my_list->next != NULL) {
			my_list = my_list->next;
		}

		my_list->next = new_list;
	}

	return head;
}

/**
 * Insert element at the index of the list and return the head
 * Return NULL on error, if it failed to allocate requested memory (see errno)
 */
struct dynamicList *dynamicList_insert_at(struct dynamicList *my_list, int index, int value) {
	struct dynamicList *head = my_list;
	struct dynamicList *previous = my_list;
	struct dynamicList *new_list = dynamicList_create(value);
	if (new_list == NULL) {
		return NULL;
	}

	if (dynamlicList_is_empty(my_list)) {
		new_list->next = my_list;
		head = new_list;
	}
	else  {
		for (int i = 0; i < index && my_list ; ++i) {
			previous = my_list;
			my_list = my_list->next;
		}

		if (my_list == NULL) {
			previous->next = new_list;
		}
		else {
			new_list->next = my_list->next;
			my_list->next = new_list;
		}
	}

	return head;
}

/**
 * Remove the value from the list and return the head
 * Return NULL on error, if it failed to allocate requested memory (see errno)
 */
struct dynamicList *dynamicList_remove_value(struct dynamicList *my_list, int value) {
	struct dynamicList *previous = my_list;
	struct dynamicList *head = my_list;

	if (my_list->element == value) {
		head = my_list->next;

		free(my_list);
	}
	else {
		while(my_list != NULL) {
			if(my_list->element == value) {
				previous->next = my_list->next;
	            free(my_list);
				break;
			}

			previous = my_list;
			my_list = my_list->next;
		}
	}

	return head;
}

/**
 * Remove the value at the index from the list and return the head
 * Return NULL on error, if it failed to allocate requested memory (see errno)
 */
struct dynamicList *dynamicList_remove_at(struct dynamicList *my_list, int index) {
	struct dynamicList *previous = my_list;
	struct dynamicList *head = my_list;
	int i = 0;

	while(1) {
		if(i == index) {
			if (i == 0) {
				head = my_list->next;

				free(my_list);
			}
			else {
				previous->next = my_list->next;
	            free(my_list);
			}

			break;
		}
		else if (my_list->next == NULL) {
			previous->next  = NULL;
			free(my_list);

			break;
		}

		previous = my_list;
		my_list = my_list->next;
		i++;
	}

	return head;
}

/**
 * Count the number of element in list and return it
 */
int dynamicList_count(struct dynamicList *my_list) {
	int i = 0;

	while(my_list != NULL) {
		my_list = my_list->next;
		i++;
	}

	return i;
}

/**
 * Check if the value is in list
 * Return 0 if the value is not in list
 *        1 if the value is in list
 */
int dynamicList_contains(struct dynamicList *my_list, int value) {

	while(my_list != NULL) {
		if(my_list->element == value) {
			return 1;
		}
		my_list = my_list->next;
	}

    return 0;
}

/**
 * Free all used memory by the list
 */
void dynamicList_delete(struct dynamicList *my_list) {
	struct dynamicList *previous = NULL;

	while(my_list != NULL) {
		previous = my_list;
		my_list = my_list->next;

		free(previous);
	}

	free(my_list);
}

int main(void) {
	struct dynamicList *my_list = NULL;

	printf("is_empty: %d\n", dynamlicList_is_empty(my_list));

	dynamicList_print(my_list);
	//[] -> [1]
	my_list = dynamicList_insert_front(my_list, 1);

	printf("is_empty: %d\n", dynamlicList_is_empty(my_list));

	dynamicList_print(my_list);
	//[1] -> [1, 2]
	my_list = dynamicList_insert_last(my_list, 2);

    dynamicList_print(my_list);
	//[1, 2] -> [3, 1, 2]
	my_list = dynamicList_insert_front(my_list, 3);

	dynamicList_print(my_list);
	//[3, 1, 2] -> [3, 5343, 1, 2]
	my_list = dynamicList_insert_at(my_list, 1, 5343);

	dynamicList_print(my_list);
	//[3, 5343, 1, 2] -> [3, 5343, 1, 2, 4]
    my_list = dynamicList_insert_last(my_list, 4);

    dynamicList_print(my_list);
	//[3, 5343, 1, 2, 4] -> [3, 5343, 1, 4]
	my_list = dynamicList_remove_value(my_list, 2);

	dynamicList_print(my_list);

	printf("Count : %d \n", dynamicList_count(my_list));

	assert(dynamicList_contains(my_list, 2) == 0);
	assert(dynamicList_contains(my_list, 4) == 1);

	dynamicList_delete(my_list);

	my_list = NULL;
	my_list = dynamicList_insert_at(my_list, 0, 42);
	dynamicList_print(my_list);
	my_list = dynamicList_insert_at(my_list, 1, 52);
	dynamicList_print(my_list);
	my_list = dynamicList_insert_at(my_list, 8, 52);
	dynamicList_print(my_list);
	my_list = dynamicList_insert_at(my_list, 0, 42);
	dynamicList_print(my_list);
	dynamicList_delete(my_list);

	return 0;
}

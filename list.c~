#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void list_print(const struct node *list) {
    int i = 0;
    printf("In list_print\n");
    while (list != NULL) {
        printf("List item %d: %d\n", i++, list->number);
        list = list->next;
    }
}



void list_append(const int *val, struct node **head) {	
	struct node *new_node = malloc(sizeof(struct node));		
	new_node -> number = val;
	new_node -> next = *head;		
	*head = new_node;
}



void list_sort(struct node **head) {
	int temp; 
	struct node *current = (*head); 
	struct node *following = current->next; 
	for ( ; current->next != NULL; current = current->next) {
		for (following = current->next; following != NULL; following = following->next) { 
			if (current->number < following->number) {
				temp = current->number; 
				current->number = following->number;
				following->number = temp;
				}
			}
	}
}



void list_clear(const struct node *list) {
    while (list != NULL) {
        struct node *tmp = list;
        list = list->next;
        free(tmp);
    }
}




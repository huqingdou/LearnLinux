/*************************************************************************
    > File Name: producer-consumer.c
    > Author: hm
    > Mail: 15686175080@163.com 
    > Created Time: Sat 16 Jul 2016 12:07:09 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

typedef struct list
{
	int _data;
	struct list *_next;
}product_list;

product_list *head = NULL;

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

product_list *buyNode(int data)
{
	product_list *tmp = malloc(sizeof(product_list));
	tmp->_data = data;
	tmp->_next = NULL;
	return tmp;
}

void push_front(product_list **phead, int data)
{
	product_list *cur = buyNode(data);
	cur->_next = *phead;
	*phead = cur;
}

void pop_front(product_list **phead, int* data)
{
	if(*phead != NULL)
	{
		product_list *del = *phead;
		*phead = del->_next;
		*data = del->_data;
		free(del);
		del = NULL;
	}
}

void show_list(product_list *phead)
{
	product_list *cur = phead;
	while(cur)
	{
		printf("%d->", cur->_data);
		cur = cur->_next;
	}
	printf("->NULL\n");
}

void* product(void *val)
{
	while(1)
	{
		pthread_mutex_lock(&lock);
		int num = rand()%1000;
		push_front(&head, num);
		pthread_mutex_unlock(&lock);
		printf("call consumer! product success! val is: %d\n", num);
		pthread_cond_signal(&cond);
		sleep(5);
	}
}

void* consume(void *val)
{
	while(1)
	{
		pthread_mutex_lock(&lock);
		while(NULL == head)
		{
			pthread_cond_wait(&cond, &lock);
		}
		int data = 0;
		pop_front(&head, &data);
		pthread_mutex_unlock(&lock);
		printf("consum success! val is: %d\n", data);
		sleep(1);
	}
}

int main()
{
//	int i = 0;
//	for(; i < 10; i++)
//	{
//		push_front(&head, i);
//		show_list(head);
//	}
//	for(i = 0; i < 10; i++)
//	{
//		pop_front(&head, i);
//		show_list(head);
//	}
//
	pthread_t t_producer;
	pthread_t t_consumer;
	pthread_create(&t_producer, NULL, product, NULL);
	pthread_create(&t_consumer, NULL, consume, NULL);

	pthread_join(t_producer, NULL);
	pthread_join(t_consumer, NULL);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "rtree.h"

int IsBefore(const void *data1, const void *data2);	
int ActionFunc(void *data, void *param);

void InitNodeArr(rtn_t *node_arr[], size_t arr[], size_t arr_len);

int main(int argc, char* argv[], char** envp)
{
	rtree_t *rt1 = NULL;
	size_t arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	size_t arr_len = sizeof(arr) / sizeof(size_t);

	size_t find1 = arr[0];
	size_t find2 = arr[9];
		
	rtn_t *node_arr[10];
	InitNodeArr(node_arr, arr, arr_len);
	
		
	
	
	printf("\n[%s %s %d]RTreeCreate\n", __FILE__, __func__, __LINE__);
	/*__________  RTreeCreate  __________*/
	rt1 = RTreeCreate(&IsBefore);
	printf("sizeof(rt1):%lu\n", sizeof(rt1));
	/*_________________________  END RTreeCreate _______________________*/
	
	printf("\n[%s %s %d]TDestroy\n", __FILE__, __func__, __LINE__);
	/*_____________________________  TDestroy  _______________________*/
	rt1->r[0] = node_arr[0];
	rt1->r[1] = node_arr[1];
	/*_________________________  END TDestroy _______________________*/
	
	printf("\n[%s %s %d]RTreeIsEmpty\n", __FILE__, __func__, __LINE__);
	/*_____________________________  RTreeIsEmpty  _______________________*/

	printf("RTreeIsEmpty(rt1): %i,	expected: 1\n", RTreeIsEmpty(rt1));	
	
	rt1->r.data = (void*)&arr[1];
	
	printf("RTreeIsEmpty(rt1): %i,	expected: 0\n", RTreeIsEmpty(rt1));	
	
	rt1->r.data = NULL;
	/*_________________________  END RTreeIsEmpty _______________________*/
	
	printf("\n[%s %s %d]RTreeHeight\n", __FILE__, __func__, __LINE__);
	/*_____________________________  RTreeHeight  _______________________*/
	
	rt1->r.data = NULL;
	printf("RTreeHeight(rt1): %d,  excpected: -1\n", RTreeHeight(rt1));
	
	
	rt1->r.data = (void*)&arr[1];
	printf("RTreeHeight(rt1): %d,  excpected: 9\n", RTreeHeight(rt1));
	
	rt1->r.data = (void*)&arr[1];
	rt1->r.l = NULL;
	rt1->r.r = NULL;
	printf("RTreeHeight(rt1): %d,  excpected: 0\n", RTreeHeight(rt1));
	
/*	rt1->r.l = node_arr[0];*/
/*	rt1->r.r = node_arr[1];*/
	
	/*_________________________  END RTreeHeight _______________________*/
	
	printf("\n[%s %s %d]RTreeSize\n", __FILE__, __func__, __LINE__);
	/*________________  RTreeSize  _______________________*/
	
	printf("RTreeSize(rt1): %lu, expected:1\n", RTreeSize(rt1));
	
	rt1->r.l = node_arr[0];
	rt1->r.r = node_arr[1];
	
	printf("RTreeSize(rt1): %lu, expected:11\n", RTreeSize(rt1));
	
	rt1->r.l = NULL;
	printf("RTreeSize(rt1): %lu, expected:10\n", RTreeSize(rt1));
	
	rt1->r.l = node_arr[0];
	/*______________END RTreeSize _______________________*/


	printf("\n[%s %s %d]RTreeFind\n", __FILE__, __func__, __LINE__);
	/*________________  RTreeFind  _______________________*/
	rt1->r.data = (void*)&arr[0];
	printf("RTreeFind(rt1, (void*)&find1: %lu, 		expected:1\n", 
		   *(size_t*)RTreeFind(rt1, (void*)&find1));
		   
	printf("RTreeFind(rt1, (void*)&find2: %lu, 		expected:10\n", 
		   *(size_t*)RTreeFind(rt1, (void*)&find2));
	
	/*______________END RTreeFind _______________________*/
	
	printf("\n[%s %s %d]RTreeForEach\n", __FILE__, __func__, __LINE__);
	/*________________  RTreeForEach  _______________________*/
	
	RTreeForEach(rt1, &ActionFunc, (void*)1);
	
	/*______________END RTreeForEach _______________________*/
	
	printf("\n[%s %s %d]RTreeDestroy\n", __FILE__, __func__, __LINE__);
	/*________________  RTreeDestroy  __________*/
	RTreeDestroy(rt1);
	/*____________  END RTreeDestroy _______________________*/
	return(0);
}

/* returns 1 if data stored in add_data1 is smaller then data stored add_data2*/
/* else 0*/
int IsBefore(const void *add_data1, const void *add_data2)
{
	return(*(size_t*)add_data1 < *(size_t*)add_data2);
}

int ActionFunc(void *data, void *param)
{
	printf("data: %lu\n", *(size_t*)data);
	
	return(0);
}

void InitNodeArr(rtn_t *node_arr[], size_t arr[], size_t arr_len)
{
	size_t i = 0;
	
	for( ; i < arr_len; ++i)
	{
		node_arr[i] = (rtn_t*)malloc(sizeof(rtn_t));
		assert(node_arr[i]);
		
		node_arr[i]->data = (void*)(&arr[i]);
		printf("node_arr[%lu]->data:%lu\n", i, *(size_t*)node_arr[i]->data);
		node_arr[i]->bf = 0;
		node_arr[i]->l = NULL;
		node_arr[i]->r = NULL;
	}
	
	node_arr[0]->l = NULL;
	node_arr[0]->r = NULL; 
	
	node_arr[1]->l = NULL;
	node_arr[1]->r = node_arr[2]; 
	
	node_arr[2]->l = NULL;
	node_arr[2]->r = node_arr[3]; 

	node_arr[3]->l = NULL;
	node_arr[3]->r = node_arr[4];

	node_arr[4]->l = NULL;
	node_arr[4]->r = node_arr[5];
	
	node_arr[5]->l = NULL;
	node_arr[5]->r = node_arr[6];   	
	
	node_arr[6]->l = NULL;
	node_arr[6]->r = node_arr[7];
	
	node_arr[7]->l = NULL;
	node_arr[7]->r = node_arr[8];
	
	node_arr[8]->l = NULL;
	node_arr[8]->r = node_arr[9];
	
	node_arr[9]->l = NULL;
	node_arr[9]->r = NULL;
	
/*	return node_arr;*/
}

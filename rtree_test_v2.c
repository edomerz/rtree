#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "rtree.h"

int IsBefore(const void *data1, const void *data2);	
int ActionFunc(void *data, void *param);

int main(int argc, char* argv[], char** envp)
{
	rtree_t *rt1 = NULL;
	size_t arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	size_t arr_len = sizeof(arr) / sizeof(size_t);

	size_t find1 = arr[0];
	size_t find2 = arr[9];
		
		
	
	
	printf("\n[%s %s %d]RTreeCreate\n", __FILE__, __func__, __LINE__);
	/*__________  RTreeCreate  __________*/
	rt1 = RTreeCreate(&IsBefore);
	printf("sizeof(rt1):%lu\n", sizeof(rt1));
	/*_________________________  END RTreeCreate _______________________*/
	
	
	printf("\n[%s %s %d]RTreeIsEmpty\n", __FILE__, __func__, __LINE__);
	/*_____________________________  RTreeIsEmpty  _______________________*/

	printf("RTreeIsEmpty(rt1): %i,	expected: 0\n", RTreeIsEmpty(rt1));	
	
	/*_________________________  END RTreeIsEmpty _______________________*/
	
	printf("\n[%s %s %d]RTreeHeight\n", __FILE__, __func__, __LINE__);
	/*_____________________________  RTreeHeight  _______________________*/
	
	printf("RTreeHeight(rt1): %d,  excpected: -1\n", RTreeHeight(rt1));
	
	/*_________________________  END RTreeHeight _______________________*/
	
	printf("\n[%s %s %d]RTreeSize\n", __FILE__, __func__, __LINE__);
	/*________________  RTreeSize  _______________________*/
	
	printf("RTreeSize(rt1): %lu, expected:0\n", RTreeSize(rt1));
	
	/*______________END RTreeSize _______________________*/


	printf("\n[%s %s %d]RTreeFind\n", __FILE__, __func__, __LINE__);
	/*________________  RTreeFind  _______________________*/
	rt1->r.data = (void*)&arr[0];
	printf("RTreeFind(rt1, (void*)&find1: %p, 		expected:nil\n", 
		   *(size_t*)RTreeFind(rt1, (void*)&find1));
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

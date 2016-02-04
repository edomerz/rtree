/*
	RecBinTree
*/
	
#ifndef __RTREE_H__
#define __RTREE_H__

#include <stdio.h>

/* return 1 if data1 preccedes data2, else 0*/
typedef int (*is_before)(const void *data1, const void *data2);
typedef int (*action_func)(void *data, void *param);

/*typedef struct rtn_t*/
/*{*/
/*	size_t bf;*/
/*	void* data;*/
/*	struct rtn_t *next[2];*/
/*}rtn_t;*/

/*struct rtree_adt*/
/*{*/
/*	rtn_t r;*/
/*	is_before func_ptr;*/
/*}rtree_adt;*/


typedef struct rtree_adt rtree_t;



/* creates rtree_t*/
rtree_t *RTreeCreate(is_before func_ptr);
/* destroys the tree*/
void RTreeDestroy(rtree_t *tree);

/* return 1 if empty, else 0*/
int RTreeIsEmpty(const rtree_t *tree);

/* Returns the height of the tree (-1 if empty) */
int RTreeHeight(const rtree_t *rtree);

/* return number of node in tree*/
size_t RTreeSize(const rtree_t *tree);

/* insert node to tree according to key field*/
/* 0 for success*/
int RTreeInsert(rtree_t *tree, void *key);

/* removes node with given key*/
void RTreeRemove(rtree_t *tree, void *key);

/* 
   Searches after 'seek' within the tree. 
   Returns a pointer to the found data or NULL.  
*/
void *RTreeFind(const rtree_t *rtree, const void *seek);

/* Performs an action_func on each element, stops upon failure.
returns the exit status of action_func */
int RTreeForEach(const rtree_t *rtree, action_func fp, void *param);

#endif /* ifndef __RTREE_H__*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "rtree.h"

#define MAX2(x, y) (((x) >= (y)) ? (x) : (y))

/*typedef struct rtn_t*/
/*{*/
/*	void* data;*/
/*	size_t bf;*/
/*	struct rtn_t *r;*/
/*	struct rtn_t *l;*/
/*}rtn_t;*/

/*struct rtree_adt*/
/*{*/
/*	rtn_t r;*/
/*	is_before func_ptr;*/
/*}rtree_adt;*/

/*	*/
/*typedef struct rtree_adt rtree_t;*/

/* creates rtree_t*/
rtree_t *RTreeCreate(is_before func_ptr)
{
	rtree_t *rt = (assert(func_ptr), (rtree_t*)calloc(1, sizeof(rtree_t)));
	if(!rt)
	{
		return(NULL);
	}
	
	rt->func_ptr = func_ptr;
	
	return(rt);
}

/* destroy subtree*/
static void TDestroy(rtn_t *root)
{
	if(!root)
	{
		return;
	}
	
	TDestroy(root->l);
	root->next[0] = NULL;
	
	TDestroy(root->r);
	root->next[1] = NULL;
	
	free(root);
}
/* destroys the tree*/
void RTreeDestroy(rtree_t *tree)
{
/*	TO DO - */
	if(!tree)
	{
		return;
	}
	
	TDestroy(tree->r.next[0]);
	
	TDestroy(tree->r.next[1]);
	
	free(tree);
}

/* return 1 if empty, else 0*/
int RTreeIsEmpty(const rtree_t *tree)
{
	return(!(tree->r.data));
}

static size_t THeight(rtn_t *root)
{
	if(!root)
	{
		return(0);	
	}
	
	return(1 + MAX2((THeight(root->l)), 
			   	   (THeight(root->r))) );
}

/* Returns the height of the tree (-1 if empty) */
int RTreeHeight(const rtree_t *tree)
{
	if(!tree || RTreeIsEmpty(tree))
	{
		return(-1);
	}
	
	return(MAX2( THeight(tree->r.l), THeight(tree->r.r) ));
}

static size_t TSize(rtn_t *root)
{
	if(!root)
	{
		return(0);
	}
	
	return(1 + TSize(root->l) + TSize(root->r));
}

/* return number of node in tree*/
size_t RTreeSize(const rtree_t *tree)
{
	assert(tree);
	if(RTreeIsEmpty(tree))
	{
		return(0);
	}
	
	return(1 + TSize(tree->r.l) + TSize(tree->r.r));
}

/* insert node to tree according to key field*/
/* 0 for success*/
/*int RTreeInsert(rtree_t *tree, void *key);*/

/* removes node with given key*/
/*void RTreeRemove(rtree_t *tree, void *key);*/

/* return 1 if data == seek, else 0*/
static int IsEqual(is_before fp, const void *data, const void *seek)
{
	/* (data >= seek) && (seek >= data)		 => seek == data*/
	return(!(fp(data, seek)) && !(fp(seek, data)));
}

static void *TFind(is_before fp, rtn_t *root, const void *seek)
{
	void *ret = NULL;
	
	if(!root)
	{
		return(NULL);
	}
	
	if(fp(root->data, seek))
	{
		ret = TFind(fp, root->r, seek);
	}
	else
	{
		if(IsEqual(fp, root->data, seek))
		{
			return(root->data);
		}
		ret = TFind(fp, root->l, seek);
	}
	
	return(ret);	
}

/* 
   Searches after 'seek' within the tree. 
   Returns a pointer to the found data or NULL.  
*/
void *RTreeFind(const rtree_t *tree, const void *seek)
{
	void *ret = NULL;
	
	if(!tree || RTreeIsEmpty(tree))
	{
		return(NULL);
	}
	
	if(IsEqual(tree->func_ptr, tree->r.data, seek))
	{
		return(tree->r.data); 
	}
	
	if(tree->func_ptr(tree->r.data, seek))/* data < seek go to right sub tree */
	{
		ret = TFind(tree->func_ptr, tree->r.r, seek);
	}
	else                                  /* data > seek go to left sub tree */
	{
		ret = TFind(tree->func_ptr, tree->r.l, seek);
	}
	
	return(ret);
}

static int TForEach(rtn_t *root, action_func fp, void *param)
{
	int ret = 1;
	
	if(!root)
	{
		return(0);
	}
	
	ret = TForEach(root->l, fp, param);
	
	if(ret)
	{
		return(ret);
	}
	
	ret = TForEach(root->r, fp, param);
	
	if(ret)
	{
		return(ret);
	}
	
	ret = fp(root->data, param);
	
	return(ret);
}

/* Performs an action_func on each element, stops upon failure.
returns the exit status of action_func */
int RTreeForEach(const rtree_t *tree, action_func fp, void *param)
{
	int ret = 1;
	
	if(!tree || !fp || RTreeIsEmpty(tree))
	{
		return(1);
	}
	
	if((ret = fp(tree->r.data, param)))
	{
		return(ret);
	}
	
	if((ret = TForEach(tree->r.l, fp, param)))
	{
		return(ret);
	}
	
	if((ret = TForEach(tree->r.r, fp, param)))
	{
		return(ret);
	}
	
	return(ret);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <unistd.h>

#include "rtree.h"

#define MAX2(x, y) (((x) >= (y)) ? (x) : (y))

typedef struct rtn_t
{
	ssize_t bf;
	void* data;
	struct rtn_t *next[2];
}rtn_t;

struct rtree_adt
{
	rtn_t r;
	is_before func_ptr;
}rtree_adt;

static ssize_t TBalance(rtn_t **root);

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
	
	TDestroy(root->next[0]);
	root->next[0] = NULL;
	
	TDestroy(root->next[1]);
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
	
	return(1 + MAX2((THeight(root->next[0])), 
			   	   (THeight(root->next[1]))) );
}

/* Returns the height of the tree (-1 if empty) */
int RTreeHeight(const rtree_t *tree)
{
	if(!tree || RTreeIsEmpty(tree))
	{
		return(-1);
	}
	
	return(MAX2( THeight(tree->r.next[0]), THeight(tree->r.next[1]) ));
}

static size_t TSize(rtn_t *root)
{
	if(!root)
	{
		return(0);
	}
	
	return(1 + TSize(root->next[0]) + TSize(root->next[1]));
}

/* return number of node in tree*/
size_t RTreeSize(const rtree_t *tree)
{
	assert(tree);
	if(RTreeIsEmpty(tree))
	{
		return(0);
	}
	
	return(1 + TSize(tree->r.next[0]) + TSize(tree->r.next[1]));
}

int TChangeRoot(rtree_t *tree, rtn_t *new_next[], void *key)
{
	memmove(tree->r.next, new_next, sizeof(tree->r.next));
	tree->r.data = key;
	
	return(0);
}

/* return 1 if data == seek, else 0*/
static int IsEqual(is_before fp, const void *data, const void *seek)
{
	/* (data >= seek) && (seek >= data)		 => seek == data*/
	return(!(fp(data, seek)) && !(fp(seek, data)));
}

/* finds where to insert new key, no duplicates allowed*/
static rtn_t *TFindPlace(rtn_t *root, void *key, is_before fp)
{
	int dir = -1;
	if(!root)
	{
		rtn_t *new_node = (rtn_t*)calloc(1, sizeof(rtn_t));
		if(!new_node)
		{
			return(NULL);
		}
		
		new_node->data = key;
		
		return(new_node);
	}
	
	dir = fp(root->data, key);
	printf("dir: %d\n", dir);
	if((!dir) && (IsEqual(fp, root->data, key)))
	{
		return(NULL);
	}
	
	root->next[dir] = TFindPlace(root->next[dir], key, fp);
	
	TBalance(&root);
	
	return(root);
}
rtn_t *SingleRot(rtn_t *root, int dir)
{
	rtn_t *pivot = root->next[dir];
	rtn_t *tmp = pivot->next[!dir];
	pivot->next[!dir] = root;
	
	root->next[dir] = tmp;
	
	return(pivot);
}

rtn_t *DoubleRot(rtn_t *root, int dir)
{
	return(root);
}


static ssize_t TBalance(rtn_t **root)
{
	if(!(*root))
	{
		return(-1);
	}
	
	(*root)->bf = TBalance(&((*root)->next[0])) - TBalance(&((*root)->next[1]));
	(*root)->bf = THeight((*root)->next[0]) - THeight((*root)->next[1]);
	
	printf("before rot - root: %p, root->bf: %ld\n", (void*)*root, (*root)->bf);
	
	if((*root)->bf >= 2)
	{
		int dir = 0;
		
		if((*root)->next[0]->bf == 1)
		{
			*root = SingleRot(*root, dir);
		}
		else
		{
			*root = DoubleRot(*root, dir);
		}
	}
	else if((*root)->bf <= -2)
	{
		int dir = 1;
		
		if((*root)->next[1]->bf == -1)
		{
			*root = SingleRot(*root, dir);
		}
		else
		{
			*root = DoubleRot(*root, dir);
		}
	}
	
	printf("after rot - root: %p, root->bf: %ld\n", (void*)(*root), (*root)->bf);
	
	return((*root)->bf);
}

/* insert node to tree according to key field*/
/* 0 for success*/
int RTreeInsert(rtree_t *tree, void *key)
{
	if(!tree || !key)
	{
		return(1);
	}
	
	if(RTreeIsEmpty(tree))
	{
		int ret = -1;
		
		rtn_t *next[] ={NULL, NULL};
		ret = TChangeRoot(tree, next, key);
		
		return(ret);
	}	
	
	TFindPlace(&tree->r, key, tree->func_ptr);

/*	TBalance(&tree->r);*/
	
	
		
	return(0);
}

/* removes node with given key*/
/*void RTreeRemove(rtree_t *tree, void *key);*/


static void *TFind(is_before fp, rtn_t *root, const void *seek)
{
	void *ret = NULL;
	
	if(!root)
	{
		return(NULL);
	}
	
	if(fp(root->data, seek))
	{
		ret = TFind(fp, root->next[1], seek);
	}
	else
	{
		if(IsEqual(fp, root->data, seek))
		{
			return(root->data);
		}
		ret = TFind(fp, root->next[0], seek);
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
	int dir = -1;
	
	if(!tree || RTreeIsEmpty(tree))
	{
		return(NULL);
	}
	
	if(IsEqual(tree->func_ptr, tree->r.data, seek))
	{
		return(tree->r.data); 
	}
/*	if 'data' before 'seek' go right, else go left*/
	dir = tree->func_ptr(tree->r.data, seek);
	TFind(tree->func_ptr, tree->r.next[dir], seek);
	
	return(ret);
}

static int TForEach(rtn_t *root, action_func fp, void *param)
{
	int ret = 1;
	
	if(!root)
	{
		return(0);
	}
	
	ret = TForEach(root->next[0], fp, param);
	
	if(ret)
	{
		return(ret);
	}
	
	ret = TForEach(root->next[1], fp, param);
	
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
	
	if((ret = TForEach(tree->r.next[0], fp, param)))
	{
		return(ret);
	}
	
	if((ret = TForEach(tree->r.next[1], fp, param)))
	{
		return(ret);
	}
	
	return(ret);
}

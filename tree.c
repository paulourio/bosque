/* This work is licensed under the Creative Commons Attribution-Noncommercial
 * 3.0 Brazil License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc/3.0/br/deed.en
 * or send a letter to Creative Commons, 171 Second Street, Suite 300,
 * San Francisco, California, 94105, USA.
 *
 * Author: Paulo Roberto Urio (August 2011)
 *
 * This is a binary search tree optimazed to conversion to latex.
 */
#include <stdlib.h>
#include "tree.h"

struct bstree {
	void    *parent;
	void    *lchild;
	void    *rchild;
	int 	value;
	float	sibling_distance;
};


/* Allocate memory for a new node */
static struct bstree *tree_new_node(const int value)
{
	struct bstree   *bst;

	bst = malloc(sizeof(struct bstree));
	if (bst == NULL) {
		err("Error: Can't allocate memory to node of value '%d'\n", 
			value);
		return NULL;
	}
	bst->parent = NULL;
	bst->lchild = NULL;
	bst->rchild = NULL;
	bst->value = value;
	return bst;
}


/* Initialize an empty binary search tree */
void *tree_new(void)
{
	return NULL;
}


/* Free all tree nodes */
void tree_free(void **ptr)
{
	struct bstree	*tree;

	if (*ptr == NULL)
		return;
	tree = *ptr;
	tree_free(&tree->lchild);
	tree_free(&tree->rchild);
	free(*ptr);
	*ptr = NULL;
}


/* Insert a value in the tree */
void tree_insert(void **ptree, const int value)
{
	struct bstree   *prev = NULL, *bst = *ptree,
			*node = tree_new_node(value);

	while (bst != NULL) {
		prev = bst;
		if (value == bst->value)
			err("Warning: Inserting a duplicated value.\n");
		bst = (value < bst->value)?  bst->lchild:  bst->rchild;
	}
	node->parent = prev;
	if (prev == NULL)
		*ptree = node;		/* The tree is empty. */
	else if (value < prev->value)
		prev->lchild = node;
	else
		prev->rchild = node;
}


#define	max(a,b)	(a > b?	a: b)
static int tree_calculate_distances(struct bstree *node) {
	if (node == NULL)
		return 0;
	if (node->lchild == NULL && node->rchild == NULL)
		return 0;
	int left = tree_calculate_distances(node->lchild);
	int right = tree_calculate_distances(node->rchild);
	node->sibling_distance = (max(left, right) + 7) * 1.65;
	return node->sibling_distance;
}


static void tree_latex_walk(void *ptree, int first)
{
	if (ptree == NULL)
		return;
	struct bstree	*node = ptree;
	
	if (first) {
		printf("\\node {%d}\n", node->value);
	} else {
		printf("child {[anchor=north,sibling distance=%fmm]\n", 
			node->sibling_distance);
		printf("node {%d} ", node->value);
	}
	tree_latex_walk(node->lchild, 0);
	if (node->lchild == NULL)
		printf(" child [missing] ");
	tree_latex_walk(node->rchild, 0);
	if (node->rchild == NULL)
		printf(" child [missing] ");
	if (!first)
		printf("}\n");
}


/* Walk the tree in three orders */
void tree_to_latex(void *ptree)
{
	tree_calculate_distances(ptree);
	printf("\\centering\n");
	printf("\\begin{tikzpicture}");
	
	if (ptree != NULL) {
		printf("[every node/.style={circle,draw}, ");
		printf("sibling distance=%fmm]\n", 
			((struct bstree *) ptree)->sibling_distance);
	}
	tree_latex_walk(ptree, 1);
	printf(";\\end{tikzpicture}\n");
}

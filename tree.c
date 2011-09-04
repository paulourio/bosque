/* This work is licensed under the Creative Commons Attribution-Noncommercial
 * 3.0 Brazil License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc/3.0/br/deed.en
 * or send a letter to Creative Commons, 171 Second Street, Suite 300,
 * San Francisco, California, 94105, USA.
 *
 * Author: Paulo Roberto Urio (August 2011)
 */
#include <stdlib.h>
#include "tree.h"

struct bstree {
	void    *parent;
	void    *lchild;
	void    *rchild;
	int 	value;
};


/* Allocate memory for a new node */
static struct bstree *tree_new_node(const int value)
{
	struct bstree   *bst;

	bst = malloc(sizeof(struct bstree));
	if (bst == NULL) {
		debug("Can't allocate memory to node of value '%d'\n", value);
		return NULL;
	}
	bst->parent = NULL;
	bst->lchild = NULL;
	bst->rchild = NULL;
	bst->value = value;
	return bst;
}


/* Search a value in the tree and returns its node */
static struct bstree *tree_search(struct bstree *ptree, const int valor)
{
	while (ptree != NULL && valor != ptree->value)
		if (valor < ptree->value)
			ptree = ptree->lchild;
		else
			ptree = ptree->rchild;
	return ptree;
}


/* Returns the minimum value node in the tree */
static struct bstree *tree_min(struct bstree *ptree)
{
	if (ptree != NULL && ptree->lchild)
		return tree_min(ptree->lchild);
	else
		return ptree;
}


/* Returns the maximum value node in the tree */
static struct bstree *tree_max(struct bstree *ptree)
{
	if (ptree != NULL && ptree->rchild)
		return tree_max(ptree->rchild);
	else
		return ptree;
}


/* Finds the successor node */
static struct bstree *tree_successor(struct bstree *bst)
{
	struct bstree   *pai = bst->parent;

	if (bst->rchild != NULL)
		return tree_min(bst->rchild);
	while (pai != NULL && bst == pai->rchild)
		pai = (bst = pai)->parent;
	return pai;
}


/* Finds the predecessor node */
static struct bstree *tree_predecessor(struct bstree *bst)
{
	struct bstree   *pai = bst->parent;

	if (bst->lchild != NULL)
		return tree_max(bst->lchild);
	while (pai != NULL && bst == pai->lchild)
		pai = (bst = pai)->parent;
	return pai;
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


/*
 * If the node has only one child, this child will be child of the
 * parent node, else will be found the successor node of node. 
 * Returns which node will be removed.
 */
static struct bstree *tree_which_node(struct bstree *node)
{
	if (node->lchild == NULL || node->rchild == NULL)
		return node;
	else
		return tree_successor(node);
}


/* Select which node switch with node. */
static struct bstree *tree_which_son_node(struct bstree *node)
{
	return node->lchild != NULL?  node->lchild:  node->rchild;
}


/*
 * Updates the connection of parent with new child.
 * If the node to be removed is the root, update the tree's root.
 * Else will be updated the son, depending whether the node to be removed
 * is on left or right of parent node.
 */
static void tree_update_father_node(void **ptree, struct bstree *node,
					struct bstree *new_son)
{
	struct bstree	*pai = node->parent;

	if (node->parent == NULL)
		*ptree = new_son;
	else if (node == pai->lchild)
		pai->lchild = new_son;
	else
		pai->rchild = new_son;
}


/*
 * Delete a tree's node
 * Finds the y node to be removed and its x child that will be the new
 * child of y's parent.  There's a case that y node isn't the node within
 * value to be removed.  In this case, y node is a leaf and its value
 * should be switched with the real node to be removed.
 */
void tree_delete(void **ptree, const int value)
{
	struct bstree   *node, *y, *x;

	if (*ptree == NULL) {
		debug("Empty tree.\n");
		return;
	}
	node = tree_search(*ptree, value);
	if (node == NULL) {
		debug("Node '%d' was not found.\n", value);
		return;
	}
	y = tree_which_node(node);
	x = tree_which_son_node(y);
	tree_update_father_node(ptree, y, x);
	if (y != node)
		node->value = y->value;
	free(y);
}


/* Walk the tree in three orders */
void tree_walk(void *ptree, register const fbst_print cblk,
		register const enum TREE_WALKORDER worder)
{
	if (ptree != NULL) {
		struct bstree	*node = ptree;

		if (worder == WALK_PREORDER)
			cblk(node->value);
			
		tree_walk(node->lchild, cblk, worder);
		
		if (worder == WALK_INORDER)
			cblk(node->value);
			
		tree_walk(node->rchild, cblk, worder);
		
		if (worder == WALK_POSORDER)
			cblk(node->value);
	}
}


static void tree_latex_walk(void *ptree, int first)
{
	if (ptree != NULL) {
		struct bstree	*node = ptree;
		
		if (first)
			printf("\\node {%d}\n", node->value);
		else
			printf("child { node {%d} ", node->value);
		tree_latex_walk(node->lchild, 0);
		tree_latex_walk(node->rchild, 0);
		if (!first)
			printf("}\n");
	}
}


/* Walk the tree in three orders */
void tree_to_latex(void *ptree)
{
	printf("\\documentclass[a4paper,landscape]{scrartcl}\n");
	printf("\\usepackage{fancybox}\n");
	printf("\\usepackage{tikz}\n");
	printf("\\thispagestyle{empty}\n");
	printf("\\begin{document}\n");
	printf("\\begin{tikzpicture}[level/.style={sibling distance=42mm/#1}]\n");
	printf("\\tikzstyle{every node}=[circle,draw]\n");
	tree_latex_walk(ptree, 1);
	printf(";\\end{tikzpicture}\n\\end{document}\n");
}


/* Returns the minimum value in the tree */
int tree_min_value(void *ptree)
{
	return ptree != NULL?  tree_min(ptree)->value:  NOT_FOUND;
}


/* Returns the maximum value in the tree */
int tree_max_value(void *ptree)
{
	return ptree != NULL?  tree_max(ptree)->value:  NOT_FOUND;
}


/* Returns the successor value in the tree */
int tree_successor_value(void *ptree, const int value)
{
	struct bstree   *no, *succ;

	if (ptree == NULL)
		return NOT_FOUND;
	no = tree_search(ptree, value);
	succ = tree_successor(no);
	return succ != NULL?  succ->value:  NOT_FOUND;
}


/* Returns the predecessor value in the tree */
int tree_predecessor_value(void *ptree, const int value)
{
	struct bstree   *no, *pred;

	if (ptree == NULL)
		return NOT_FOUND;
	no = tree_search(ptree, value);
	pred = tree_predecessor(no);
	return pred != NULL?  pred->value:  NOT_FOUND;
}

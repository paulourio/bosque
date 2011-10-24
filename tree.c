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
#include <string.h>
#include "tree.h"

struct bstree {
	void    *parent;
	void    *lchild;
	void    *rchild;
	int 	value;
	int	color;
	char	*meta; /* Used for Huffman's code, for instance. */
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
		exit(1);
	}
	bst->parent = NULL;
	bst->lchild = NULL;
	bst->rchild = NULL;
	bst->value = value;
	bst->color = WHITE;
	bst->meta = NULL;
	bst->sibling_distance = 0.0;
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
	struct bstree	*tree = *ptr;

	if (tree == NULL)
		return;
	tree_free(&tree->lchild);
	tree_free(&tree->rchild);
	if (tree->meta != NULL)
		free(tree->meta);
	free(*ptr);
	*ptr = NULL;
}


static struct bstree *tree_insert_node(void **ptree, const int value)
{
	struct bstree   *prev = NULL, *bst = *ptree,
			*node = tree_new_node(value);

	while (bst != NULL) {
		prev = bst;
		if (value == bst->value)
			err("Warning: Inserting duplicated value %d.\n", value);
		bst = (value < bst->value)?  bst->lchild:  bst->rchild;
	}
	node->parent = prev;
	if (prev == NULL)
		*ptree = node;		/* The tree is empty. */
	else if (value < prev->value)
		prev->lchild = node;
	else
		prev->rchild = node;
	return node;
}

/* Insert a value in the tree */
void tree_insert(void **ptree, const int value)
{
	(void) tree_insert_node(ptree, value);
}

/* Insert an extended value in the tree */
void tree_insert_ex(void **ptree, const int value, const char *meta)
{
	char *copy = malloc(sizeof(char) * (strlen(meta) + 1));
	
	if (copy == NULL) {
		fprintf(stderr, "Can't allocate memory for meta string.\n");
		exit(1);
	}
	tree_insert_node(ptree, value)->meta = strcpy(copy, meta);
}

/* Insert an colored node in the tree */
void tree_insert_colored(void **ptree, const int value, const int color)
{
	tree_insert_node(ptree, value)->color = color;
}

/* Forced method to set childs. */
void tree_set_childs(void *node, void *left, void *right)
{
	struct bstree *parent = node;

	parent->lchild = left;
	parent->rchild = right;
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


/* Verify for possible problems in the tree, and print warnings. */
static void node_check_red_node(struct bstree *node)
{
	struct bstree *parent = node->parent;
	
	if (parent == NULL)
		fprintf(stderr,	"Warning: The tree's root should be "
			"a black node.\n");
	else if (parent->color == RED)
		fprintf(stderr, "Warning: Red nodes should only have black "
			"childs. (node %d).\n", node->value);
}


static char *node_color_to_str(struct bstree *node)
{
	char *str = malloc(40 * sizeof(char));
	
	if (str == NULL) {
		fprintf(stderr, "Can't allocate memory for color string.\n");
		exit(1);
	}
	if (node->color == WHITE) {
		strcpy(str, " ");
	} else {
		strcpy(str, "[fill=");
		switch (node->color) {
		case BLACK:
			strcat(str, "black!100, font=\\color{white}");
			break;
		case RED:
			node_check_red_node(node);
			strcat(str, "red!60");
			break;
		default:
		case UNKNOWN:
			fprintf(stderr, "Warning: Unknown color for node "
				"\"%d\". I'm only accepting \"BLACK\" "
				"and \"RED\" values.\n", node->value);
			strcat(str, "yellow!50");
			break;
		}
		strcat(str, "] ");
	}
	return str;
}

static void tree_latex_print_node(struct bstree *node)
{
	char *color = node_color_to_str(node);
	
	fprintf(stdout, "node%s{%d}", color, node->value);
	free(color);
}

static void tree_latex_walk(struct bstree *node, int first)
{
	if (node == NULL)
		return;
	
	if (first) {
		if (node->meta != NULL) 
			printf("\\node {%s}\n", node->meta);
		else {
			printf("\\");
			tree_latex_print_node(node);
			printf("\n");
		}
	} else {
		printf("child {[anchor=north,sibling distance=%fmm]\n", 
			node->sibling_distance);
		if (node->meta != NULL) {
			if (node->meta[0] == '/')
				printf("node {%s} ", node->meta);
			else
				printf("node [rectangle] {%s} ", node->meta);
		} else {
			tree_latex_print_node(node);
		}
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
	struct bstree *tree = ptree;
	
	tree_calculate_distances(tree);
	printf("\\centering\n");
	printf("\\begin{tikzpicture}");
	
	if (tree != NULL) {
		printf("[every node/.style={circle,draw}, ");
		printf("sibling distance=%fmm]\n", tree->sibling_distance);
	}
	tree_latex_walk(tree, 1);
	printf(";\\end{tikzpicture}\n");
}

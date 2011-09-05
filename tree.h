/* This work is licensed under the Creative Commons Attribution-Noncommercial
 * 3.0 Brazil License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc/3.0/br/deed.en
 * or send a letter to Creative Commons, 171 Second Street, Suite 300,
 * San Francisco, California, 94105, USA.
 *
 * Author: Paulo Roberto Urio (August 2011)
 */
#ifndef TREE_H_INCLUDE
#define TREE_H_INCLUDE

#define DEBUG

#ifdef DEBUG
#	include	<stdio.h>
#	define	err(...)	fprintf(stderr, __VA_ARGS__)
#	define	debug(...)	err("Debug: " __VA_ARGS__)
#else
#	define	debug
#endif

#define NOT_FOUND	(-1)

enum TREE_WALKORDER {
	WALK_INORDER,
	WALK_PREORDER,
	WALK_POSORDER
};

typedef void (*fbst_print)(const int);
typedef void (*fbst_print_ex)(const int, const int, const int, const int);

void *tree_new(void);
void tree_free(void **ptr) __nonnull ((1));
void tree_insert(void **ptree, const int value) __nonnull ((1));
void tree_delete(void **ptree, const int value) __nonnull ((1));
void tree_walk(void *ptree, register const fbst_print cblk,
		register const enum TREE_WALKORDER worder);
void tree_to_latex(void *ptree);		
int tree_min_value(void *ptree);
int tree_max_value(void *ptree);
int tree_successor_value(void *ptree, const int value);
int tree_predecessor_value(void *ptree, const int value);

#endif

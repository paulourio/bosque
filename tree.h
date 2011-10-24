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

#include <stdio.h>
#define	err(...)	fprintf(stderr, __VA_ARGS__)

#define DEBUG

#ifdef DEBUG
#	define	debug(...)	err("Debug: "__VA_ARGS__)
#else
#	define	debug
#endif

#define NOT_FOUND	(-1)

#define	WHITE	-1
#define	BLACK	0
#define RED	1
#define UNKNOWN	2

extern void *tree_new(void);
extern void tree_free(void **ptr) __nonnull ((1));
extern void tree_insert(void **ptree, const int value) __nonnull ((1));
extern void tree_insert_ex(void **ptree, const int value, const char *meta);
extern void tree_insert_colored(void **ptree, const int value, const int color);
extern void tree_to_latex(void *ptree);	
extern void tree_set_childs(void *node, void *left, void *right);

#endif

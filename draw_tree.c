/* This work is licensed under the Creative Commons Attribution-Noncommercial
 * 3.0 Brazil License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc/3.0/br/deed.en
 * or send a letter to Creative Commons, 171 Second Street, Suite 300,
 * San Francisco, California, 94105, USA.
 *
 * Author: Paulo Roberto Urio (September 2011)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

struct options {
	int huffman;
	int redblack;
	char **argv;
	int argc;
	void *arvore;
} opt;

static void header(void)
{
	printf("\\documentclass[]{minimal}\n");
	printf("\\usepackage{tikz}\n");
	printf("\\usepackage[paperwidth=30in, paperheight=100in]{geometry}\n");
	printf("\\thispagestyle{empty}\n");
	printf("\\begin{document}\n");
}


static void end(void)
{
	printf("\\end{document}\n");
}


static void show_help(void)
{
	puts("Use: desenhar [OPTION] | [TREE] [. [TREE]..]");
	puts("Read one or more trees from arguments, or");
	puts("depending on option, read from standard input.");
	puts("A tree is should be described with integer numbers, ");
	puts("separated by space, in preorder.  To draw more");
	puts("than one tree, separate each tree with a dot.");
	puts("Example: desenhar 4 3 2 1 6 5 7 . 2 3 1 .\n");
	puts("Controle de entrada:");
	puts("  --huffman        Tell to the program to draw a tree");
	puts("                   used in the huffman's code algorithm.");
	puts("                   The tree will be read from the standard");
	puts("                   input and any additional data passed");
	puts("                   as program option will be ignored.");
	puts("  --help           Show this message.\n");
	puts("Report bugs to: paulo@archlinux.us");
}


static void process_option(const char *option)
{
	/* Should be a option */
	if (strcmp(option, "--huffman") == 0) {
		//puts("Huffman coding selected.");
		opt.huffman = 1;
		return;
	}
	if (strcmp(option, "--help") == 0) {
		show_help();
		exit(0);
	}	
	fprintf(stderr, "I don't understand the option %s\n", option);
}


static void finish_tree(void)
{
	tree_to_latex(opt.arvore);
	tree_free(&opt.arvore);
	opt.arvore = tree_new();
}

static void insert_next_value(void)
{
	int i;

	if (opt.huffman) {
		char s[100];
		//puts("from input");
		if (scanf("%d %s", &i, s) == 2) {
			if (s[0] == '.') {
				finish_tree();
			} else {
				//printf("insert %d %s\n",i , s);
				tree_insert_ex(&opt.arvore, i, s);
			}
		}
	} else {
		sscanf(*opt.argv, "%d", &i);
		tree_insert(&opt.arvore, i);
	}
}

static void start_reading(void)
{
	opt.arvore = tree_new();
	while (opt.argc--) {
		if (*opt.argv[0] == '-') {
			opt.argv++;
			continue;
		}
		if (*opt.argv[0] == '.') {
			finish_tree();
			opt.argv++;
			continue;
		}
		if (!opt.huffman)
			insert_next_value();		
	}
	if (opt.huffman) {
		while (!feof(stdin))
			insert_next_value();
	}
	tree_to_latex(opt.arvore);
	tree_free(&opt.arvore);
	end();	
}


int main(int argc, char *argv[])
{
	argv++;	
	argc--;
	header();
	opt.argv = argv;
	opt.argc = argc;
	opt.huffman = 0;
	opt.redblack = 0;

	while (argc--) {
		if (*argv[0] == '-')
			process_option(*argv);
		argv++;	
	}
	start_reading();
	return EXIT_SUCCESS;
}

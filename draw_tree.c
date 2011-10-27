/* This work is licensed under the Creative Commons Attribution-Noncommercial
 * 3.0 Brazil License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc/3.0/br/deed.en
 * or send a letter to Creative Commons, 171 Second Street, Suite 300,
 * San Francisco, California, 94105, USA.
 *
 * Author: Paulo Roberto Urio (September 2011)
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tree.h"

struct options {
	int huffman;
	int redblack;
	int stdin; /* If is in normal mode and wants to use stdin anyway */
	char **argv;
	int argc;
	int arg_curr;
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
	puts("Format options:");
	puts("  -                Read values from standard input.");
	puts("                   This option is recommended for everyone,");
	puts("                   because it has less bugs.  Reading trees");
	puts("                   from argument line can be painful.");
	puts("  --huffman        Tell to the program to draw a tree");
	puts("                   used in the huffman's code algorithm.");
	puts("                   The tree will be read from the standard");
	puts("                   input and any additional data passed");
	puts("                   as program option will be ignored.");
	puts("                   Format: <id> <value>");
	puts("                   Eg: 4 /:6 2 /:3 1 b:1 3 n:2 5 a:3");
	puts("  --redblack       Tell to the program to draw a redblack tree.");
	puts("                   The format is similar to the huffman format,");
	puts("                   you have to define the node id and its color.");
	puts("                   Format: <id> <color>");
	puts("                   Eg: 5 black 2 red 7 black");
	puts("                   Acceptable colors (case sensitive):");
	puts("                   For a black node: 0 black BLACK;");
	puts("                   For a red node:   1 red RED.");
	puts("Drawing options:");
	puts("  --factor 0.0     Set the float value to be used to calculate");
	puts("                   the siblings distance.");
	printf("                   Default value: %f.\n", tree_get_factor());
	puts("                   Eg: --factor 3.2");
	puts("  --max-distance 0 Set the float value to be used to limit");
	puts("                   the siblings distance.");
	printf("                   Default value: %f.\n", tree_get_max_distance());
	puts("                   Eg: --max-distance 50.0");
	puts("Other options:");
	puts("  --help           Show this message.\n");
	puts("Report bugs to: paulourio@gmail.com");
	exit(0);
}

static void arg_error(void)
{
	fprintf(stderr, "Argument error: you must define correct parameters."
		" Please, use option --help to get program help.\n");
	exit(1);
}

static float args_read_float(void)
{
	float factor = 0.0;

	if (opt.argc == opt.arg_curr - 1)
		arg_error();
	if (sscanf(opt.argv[opt.arg_curr], "%f", &factor) != 1)
		arg_error();
	return factor;
}

static void process_option(const char *option)
{
	/* Should be a option */
	if (strcmp(option, "-") == 0) {
		/* Read from standard input */
		opt.stdin = 1;
		return;
	}
	if (strcmp(option, "--huffman") == 0) {
		//puts("Huffman coding selected.");
		opt.huffman = 1;
		opt.stdin = 1;
		return;
	}
	if (strcmp(option, "--redblack") == 0) {
		opt.redblack = 1;
		opt.stdin = 1;
		return;
	}
	if (strcmp(option, "--factor") == 0) {
		/* Bug on: if reading trees from argument line */
		tree_set_distance_factor(args_read_float());
		return;
	}
	if (strcmp(option, "--max-distance") == 0) {
		/* Bug on: if reading trees from argument line */
		tree_set_distance_max(args_read_float());
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

static int indetify_node_color(const char *value)
{
	#define check(s)	(strcmp(value, s) == 0)
	if (check("red") || check("RED") || check("1"))
		return RED;
	if (check("black") || check("BLACK") || check("0"))
		return BLACK;
	return UNKNOWN;
}

static int insert_next_value(void)
{
	int i;

	if (opt.stdin) {
		int c = ' ';

		while ((c = getc(stdin)) == ' ')
			; /* VOID */

		if ((char) c == '.')
			finish_tree();
		else
			ungetc(c, stdin);
	}

	if (opt.huffman) {
		char s[100];
		if (scanf("%d %s", &i, s) == 2)
			tree_insert_ex(&opt.arvore, i, s);
		if (feof(stdin))
			return 0;
	} else if (opt.redblack) {
		char s[100];
		if (scanf("%d %s", &i, s) == 2) {
			int c = indetify_node_color(s);
			tree_insert_colored(&opt.arvore, i, c);
		}
		if (feof(stdin))
			return 0;
	} else {
		int ret;
		if (opt.stdin) {
			ret = scanf("%d", &i);
		} else
			ret = sscanf(*opt.argv++, "%d", &i);
		if (ret == 1)
			tree_insert(&opt.arvore, i);
		else
			return 0;
	}
	return 1;
}

static void start_reading(void)
{
	opt.arvore = tree_new();
	if (opt.stdin == 0) {
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
			insert_next_value();
		}
	} else {
		while (insert_next_value())
			; /* VOID */
	}
	tree_to_latex(opt.arvore);
	tree_free(&opt.arvore);
	end();
}


int main(int argc, char *argv[])
{
	argv++;
	argc--;
	opt.argv = argv;
	opt.argc = argc;
	opt.arg_curr = 0;
	opt.huffman = 0;
	opt.redblack = 0;
	opt.stdin = 0;

	while (argc--) {
		opt.arg_curr++;
		if (*argv[0] == '-')
			process_option(*argv);
		argv++;
	}
	header();
	start_reading();
	return EXIT_SUCCESS;
}

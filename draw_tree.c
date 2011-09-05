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
#include "tree.h"

static void header(void)
{
	printf("\\documentclass[]{minimal}\n");
	printf("\\usepackage{fancybox}\n");
	printf("\\usepackage{tikz}\n");
	printf("\\usepackage[paperwidth=30in, paperheight=100in]{geometry}\n");
	printf("\\thispagestyle{empty}\n");
	printf("\\begin{document}\n");
}

static void end(void)
{
	printf("\\end{document}\n");
}

#define distance(count)	(42 + count/2 * 10)
int main(int argc, char *argv[])
{
	void	*arvore;
	int	count = 0;
	
	argv++;	
	header();
	arvore = tree_new();
	while (argc--) {
		if (argc == 0)
			break;
		int i;
		if (*argv[0] == '.') {
			tree_to_latex(arvore, distance(count));
			tree_free(&arvore);
			arvore = tree_new();
			argv++;
			count = 0;
			continue;
			/*printf("\n*** New tree ***\n");*/
		}
		sscanf(*argv++, "%d", &i);
		tree_insert(&arvore, i);
		count++;
	}
	tree_to_latex(arvore, distance(count));
	tree_free(&arvore);
	end();
	return EXIT_SUCCESS;
}

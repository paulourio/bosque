Bosque
======

This program has intent to draw binary search trees using LaTeX. The 
application receives one or more trees in preorder format as parameter and draw this
tree with LaTeX resulting in a SVG image file.

Dependencies
------------

If you are using Ubuntu Linux you will need this following packages:

* [texlive-full](http://packages.ubuntu.com/search?keywords=texlive-full) -- `apt-get install textlive-full`
* [texlive-extra-utils](http://packages.ubuntu.com/search?keywords=texlive-extra-utils) -- `apt-get install texlive-extra-utils`
* [pdf2svg](http://packages.ubuntu.com/search?keywords=pdf2svg) -- `apt-get install pdf2svg`

Otherwise you have to install the latex base `texlive-base`, the 
`tikzpicture` package and `pdf2svg`.

Installing
----------

In order to install you will need to compile the tree generator. Go to the source directory.

	$ cd drawbstree
	
Type `make` to compile the program.

	$ make
	
Type `make install` as root user to install the application.

	$ sudo make install
	
Using
-----

### Basic

After installed the application will have a program called `desenhar`. 
You can type it with the desired tree in preorder. For instance:

	$ desenhar 5 3 1 4 6
	
Specifying the output file:

	$ desenhar -o out.svg 5 3 1 4 6

### More than one tree

You can draw more than one tree. They will be converted all in the same
image since they fit in one page. You should identify each new tree with
a dot at start of it.

	$ desenhar 15 5 3 12 10 6 7 13 16 20 18 23 . 15 6 3 12 10 7 13 16 20 18 23
	
### Integrating

Using bash you can set up your exercise or testing application to
print out the tree in preorder. And with a pipe we can redirect
the application output as `desenhar` execution parameters.

C

	$ ./binary_tree | xargs desenhar
	
Java

	$ java tree/Main | xargs desenhar
	
Ruby

	$ ruby tree_test.rb | xargs desenhar
	
Also, you can use `tee` to get your application output before 
redirect to `desenhar`:

	$ ./my_avl_tree | tee | xargs desenhar -o output.svg
	

This example also showed how define the output file to the trees.

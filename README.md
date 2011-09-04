Drawing Binary Search Tree
==========================

This program has intent to draw a binary search tree using latex.

Dependencies
------------

If you are using Ubuntu Linux you will need this following packages:

* [texlive-full](http://packages.ubuntu.com/search?keywords=texlive-full) -- `apt-get install textlive-full`
* [texlive-extra-utils](http://packages.ubuntu.com/search?keywords=texlive-extra-utils) -- `apt-get install texlive-extra-utils`
* [pdf2svg](http://packages.ubuntu.com/search?keywords=pdf2svg) -- `apt-get install pdf2svg`

Otherwise you should have latex base `texlive-base` and the `tikzpicture` package installed and `pdf2svg`.

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
You can type it with the desired tree in preorder. For instance,
if you want to draw this tree:

 5
 
 /\
 
3  6

/\

1 4

Type this:

	$ desenhar 5 3 1 4 6
	
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

	$ ./binary_tree | tee | xargs desenhar
	


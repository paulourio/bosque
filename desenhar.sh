#!/bin/bash

function usage
{
    echo "usage: desenhar [-o outputfile] [tree values in preorder]"
    echo "eg: desenhar 5 3 1 4 6"
}

ext="svg"
DIR=`pwd`
DIR=$DIR/arvores
FILE=$DIR/bstree-$RANDOM
FILEPDFCROP=$FILE-crop.pdf
FINAL=$FILE.$ext
CUSTOM=0

HELP=0

STANDARDINPUT=0

trees=""

while [ "$1" != "" ]; do
    case $1 in
        -o )            shift
                        FINAL=$1
                        CUSTOM=1
                        if [ "$FINAL" = "" ]; then
                            echo "You must define an output file."
                            exit 1
                        fi
                        ;;
        --help )
			HELP=1
			trees="$trees --help"
			;;
        --huffman )
        		STANDARDINPUT=1
        		trees="$trees $1"
        		;;
        --redblack )
        		STANDARDINPUT=1
        		trees="$trees $1"
        		;;
        --factor )      shift
			FACTOR="$1"
                        if [ "$FACTOR" = "" ]; then
                            echo "You must define an float value."
                            exit 1
                        fi
			trees="$trees --factor $FACTOR"
			;;
        --max-distance ) shift
			MAXDIST="$1"
                        if [ "$MAXDIST" = "" ]; then
                            echo "You must define an float value."
                            exit 1
                        fi
			trees="$trees --max-distance $MAXDIST"
			;;
        * )             trees="$trees $1"
                        ;;
    esac
    shift
done

if [ $CUSTOM -eq 0 ]; then
    if [ ! -d "$DIR" ] ; then
        mkdir $DIR
    fi

    cd $DIR
else
    EXT=`echo "$FINAL" | awk -F . '{print $NF}' | tr 'A-Z' 'a-z'`
    if [ "$EXT" != "$ext" ]; then
        FINAL=$FINAL.svg
    fi
    DIR=`pwd`
    FILE=$DIR/bstree-$RANDOM
    FILEPDFCROP=$FILE-crop.pdf
fi

RM=`which rm`

if [ "$trees" = "" ]; then
	usage
	exit 1
fi

PDFLATEX=`which pdflatex`
if [ $? -eq 1 ]; then
	echo "Packaged texlive not installed"
	exit 1
fi

PDFCROP=`which pdfcrop`
if [ $? -eq 1 ]; then
	echo "Package pdfcrop not installed"
	exit 1
fi

PDF2SVG=`which pdf2svg`
if [ $? -eq 1 ]; then
	echo "Package pdf2svg not installed"
	exit 1
fi

OPEN=`which eog`
if [ ! -f $OPEN ]; then
	OPEN=`which gnome-open`
fi
# For some obscure reason, when I open a SVG file in nautilus it
# will be open on eog. Though, when i use gnome-open Inkscape will
# be used. So, I'm avoiding open inkscape doing this workaround.
if [ ! -f $OPEN ]; then
	echo "Are you using GNOME? Can't find gnome-open"
	OPEN=`which xdg-open`
fi

DRAW=`which drawbstree`
if [ ! -f $DRAW ]; then
	echo "File drawbstree not found. Did you compile Bosque?"
	exit 1
fi

if [ $HELP -eq 1 ]; then
	$DRAW $trees
	exit 0
fi

if [ $STANDARDINPUT -ne 0 ]; then
	cat - | $DRAW $trees > $FILE.tex
else
	$DRAW $trees > $FILE.tex
fi

if [ $? -ne 0 ]; then
	echo "Error while trying to create the tree"
	exit 1
fi

$PDFLATEX $FILE > /dev/null
if [ $? -ne 0 ]; then
	echo "Error on latex"
    	$RM $FILE.tex $FILE.aux $FILE.log
	exit 1
fi

$RM $FILE.tex $FILE.aux $FILE.log

$PDFCROP $FILE.pdf $FILEPDFCROP > /dev/null
if [ $? -ne 0 ]; then
	echo "Error on pdfcrop"
    	$RM $FILE.pdf
	exit 1
fi

$RM $FILE.pdf

$PDF2SVG $FILEPDFCROP $FINAL
if [ $? -ne 0 ]; then
	echo "Failed to convert image to SVG"
  	$RM $FILEPDFCROP
	exit 1
fi

$RM $FILEPDFCROP

echo "$FINAL"
$OPEN $FINAL 1,2> /dev/null &

if [ $CUSTOM -eq 0 ]; then
    cd ..
fi

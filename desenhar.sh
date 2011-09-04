#!/bin/bash

function usage
{
    echo "usage: desenhar [-o outputfile] [tree values]"
}

ext="svg"
DIR=`pwd`
DIR=$DIR/arvores
FILE=$DIR/bstree-$RANDOM
FILEPDFCROP=$FILE-crop.pdf
FINAL=$FILE.$ext
CUSTOM=0

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
        -h | --help )   usage
                        exit
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
	echo "Sintaxe: desenhar [árvore em pré-ordem]"
	echo "Exemplo: desenhar 5 3 1 4 6"
	exit 1
fi

PDFLATEX=`which pdflatex`
if [ $? -eq 1 ]; then
	echo "Necessário o pacote texlive"
	exit 1
fi

PDFCROP=`which pdfcrop`
if [ $? -eq 1 ]; then
	echo "Necessário o pacote pdfcrop"
	exit 1
fi

PDF2SVG=`which pdf2svg`
if [ $? -eq 1 ]; then
	echo "Necessário o pacote pdf2svg"
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
	echo "Você está usando GNOME? Não encontrei gnome-open"
	OPEN=`which xdg-open`
fi

DRAW=`which drawbstree`
if [ ! -f $DRAW ]; then
	echo "Arquivo ./drawtree não encontrado. Compilou?"
	exit 1
fi

$DRAW $trees > $FILE.tex
if [ $? -ne 0 ]; then
	echo "Erro ao gerar árvore"
	exit 1
fi

$PDFLATEX $FILE > /dev/null
if [ $? -ne 0 ]; then
	echo "Erro executar latex"
    $RM $FILE.tex $FILE.aux $FILE.log
	exit 1
fi

$RM $FILE.tex $FILE.aux $FILE.log

$PDFCROP $FILE.pdf $FILEPDFCROP > /dev/null
if [ $? -ne 0 ]; then
	echo "Erro ao executar pdfcrop"
    $RM $FILE.pdf
	exit 1
fi

$RM $FILE.pdf

$PDF2SVG $FILEPDFCROP $FINAL
if [ $? -ne 0 ]; then
	echo "Erro ao converter para SVG"
    $RM $FILEPDFCROP
	exit 1
fi

$RM $FILEPDFCROP

echo "$FINAL"
$OPEN $FINAL

if [ $CUSTOM -eq 0 ]; then
    cd ..
fi

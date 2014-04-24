#!/bin/sh
options='f:n:r:t:g:o:'
while getopts $options option
do
    case $option in
        f   )
            FOLDER=$OPTARG
            ;;
        n   )
            NAME=$OPTARG
            ;;
        r   )
            REAL=$OPTARG
            ;;
        t   )
            TRAINING=$OPTARG
            ;;
        g   )
            GRID=$OPTARG:
            ;;
        o   )
            OUTPUT=$OPTARG
            ;;
        *   )
            error
            ;;
    esac
done

# Gnuplot script
(

echo "#!/usr/bin/gnuplot -p -"
case $OUTPUT in
    eps )
        echo "set terminal postscript eps enhanced color"
        ;;
    * )
        ;;
esac

if [ ! -z "$ASPECT_RATIO" ]
then
    echo "set size 1, $ASPECT_RATIO"
fi

case $GRID in
    x   )
        echo 'set grid xtics lt 0 lw 1 lc rgb "#bbbbbb"'
        ;;
    y   )
        echo 'set grid ytics lt 0 lw 1 lc rgb "#bbbbbb"'
        ;;
    xy  )
        echo 'set grid ytics lt 0 lw 1 lc rgb "#bbbbbb"'
        echo 'set grid xtics lt 0 lw 1 lc rgb "#bbbbbb"'
        ;;
    yx  )
        echo 'set grid ytics lt 0 lw 1 lc rgb "#bbbbbb"'
        echo 'set grid xtics lt 0 lw 1 lc rgb "#bbbbbb"'
        ;;
    *   )
        ;;
esac


echo "set style fill solid 1 border 0"
echo "set key left top Left"
if [ -z $FONTSIZE ]
then
    FONTSIZE=12
fi
echo "set xtics font \"Arial, $FONTSIZE\""
echo "set ytics font \"Arial, $FONTSIZE\""

if [ ! -z $XRANGE ]
then
    echo "set xrange [$XRANGE]"
fi
if [ ! -z $YRANGE ]
then
    echo "set yrange [$YRANGE]"
fi

echo "set title \"Training results for $NAME\""

echo "plot '$FOLDER/$TRAINING' using 1:2 with lines notitle, '$REAL' using :2 with lines notitle"

) | gnuplot -p


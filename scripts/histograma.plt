# script para a criação de um histograma usando o software gnuplot
# http://gnuplot.info

set terminal pngcairo
set output "../relatorio/histograma.png"

set datafile separator ","

set style data histograms
set style fill solid
set boxwidth 0.9

unset key
set term png font 'CMU Sans, 12'
set xlabel 'Componente' textcolor rgb "black"
set ylabel 'Tamanho' textcolor rgb "black"
set yrange [200:270]
plot "histograma.csv" using 2:xtic(1)


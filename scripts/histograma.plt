# script para a criação de um histograma usando o software gnuplot
# http://gnuplot.info

set terminal pngcairo
set output "../relatorio/imagens/histograma.png"

set datafile separator ","

set style data histograms
set style fill solid
set boxwidth 0.9

unset key
set term png font 'CMU Sans, 12'
set xlabel 'Componente' textcolor rgb "black"
set ylabel 'Tamanho' textcolor rgb "black"
set yrange [000:1600]
plot "histograma.csv" using 2:xtic(1)


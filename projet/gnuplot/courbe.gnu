unset key
set title 'Trajectoire du Robot'
set xlabel 'x en pixels' 
set ylabel 'y en pixels' 
set grid
plot [0:640] [0:480] "courbe.dat" 
pause 100000 

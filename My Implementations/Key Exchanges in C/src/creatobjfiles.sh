mkdir obj
#CFLAGS= -Wall -g -std=c11
gcc -c jintailwe.c -Wall -g -std=c11 -I../include -o jintailwe.o
gcc -c dgs_bern.c -Wall -g -std=c11 -I../include -o dgs_bern.o
gcc -c dgs_gauss_dp.c -Wall -g -std=c11 -I../include -o dgs_gauss_dp.o
gcc -c dgs_gauss_mp.c -Wall -g -std=c11 -I../include -o dgs_gauss_mp.o
gcc -c dgs_rround_dp.c -Wall -g -std=c11 -I../include -o dgs_rround_dp.o
gcc -c dgs_rround_mp.c -Wall -g -std=c11 -I../include -o dgs_rround_mp.o
mv *.o obj/

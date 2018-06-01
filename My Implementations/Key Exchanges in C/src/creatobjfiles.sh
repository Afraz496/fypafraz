mkdir obj
gcc -c jintailwe.c -I../include -o jintailwe.o
gcc -c dgs_bern.c -I../include -o dgs_bern.o
gcc -c dgs_gauss_dp.c -I../include -o dgs_gauss_dp.o
gcc -c dgs_gauss_mp.c -I../include -o dgs_gauss_mp.o
gcc -c dgs_rround_dp.c -I../include -o dgs_rround_dp.o
gcc -c dgs_rround_mp.c -I../include -o dgs_rround_mp.o
mv *.o obj/

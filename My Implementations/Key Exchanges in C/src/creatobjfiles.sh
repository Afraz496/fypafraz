mkdir obj
gcc -c jintailwe.c -o jintailwe.o
gcc -c dgs_bern.c -o dgs_bern.o
gcc -c dgs_gauss_dp.c -o dgs_gauss_dp.o
gcc -c dgs_gauss_mp.c -o dgs_gauss_mp.o
gcc -c dgs_rround_dp.c -o dgs_rround_dp.o
gcc -c dgs_rround_mp.c -o dgs_rround_mp.o
mv *.o obj/

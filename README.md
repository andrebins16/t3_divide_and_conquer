# t3_divide_and_conquer

# Rodar sequencial:
gcc sequential.c -o sequential
./sequential

# Rodar naive: 
mpicc -o naive naive.c -lm
mpirun -np <cores> ./naive 

# Rodar otimizado:
mpicc optimized.c -lm -o optimized 
mpirun -np <cores> ./optimized <size>
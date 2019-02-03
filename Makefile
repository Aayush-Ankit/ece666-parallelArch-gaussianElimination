
seq:
	g++ -g -std=c++0x gauss.cc -o gauss.o


run_seq:
	./gauss.o 64 inputs/256.txt > logs/seq_64.txt
	./gauss.o 64 inputs/256.txt > logs/seq_64.txt
	./gauss.o 256 inputs/256.txt > logs/seq_256.txt
	./gauss.o 256 inputs/256.txt > logs/seq_256.txt
	./gauss.o 1024 inputs/1024.txt > logs/seq_1024.txt
	./gauss.o 1024 inputs/1024.txt > logs/seq_1024.txt


pth_bcast:
	g++ -g -std=c++0x pth_files/gauss_inter_pthreads_n64_t2.cc -o pth_files/gauss_pth_n64_t2.o -lpthread
	g++ -g -std=c++0x pth_files/gauss_inter_pthreads_n256_t2.cc -o pth_files/gauss_pth_n256_t2.o -lpthread
	g++ -g -std=c++0x pth_files/gauss_inter_pthreads_n1024_t2.cc -o pth_files/gauss_pth_n1024_t2.o -lpthread

	g++ -g -std=c++0x pth_files/gauss_inter_pthreads_n64_t4.cc -o pth_files/gauss_pth_n64_t4.o -lpthread
	g++ -g -std=c++0x pth_files/gauss_inter_pthreads_n256_t4.cc -o pth_files/gauss_pth_n256_t4.o -lpthread
	g++ -g -std=c++0x pth_files/gauss_inter_pthreads_n1024_t4.cc -o pth_files/gauss_pth_n1024_t4.o -lpthread

	g++ -g -std=c++0x pth_files/gauss_inter_pthreads_n64_t8.cc -o pth_files/gauss_pth_n64_t8.o -lpthread
	g++ -g -std=c++0x pth_files/gauss_inter_pthreads_n256_t8.cc -o pth_files/gauss_pth_n256_t8.o -lpthread
	g++ -g -std=c++0x pth_files/gauss_inter_pthreads_n1024_t8.cc -o pth_files/gauss_pth_n1024_t8.o -lpthread

	g++ -g -std=c++0x pth_files/gauss_inter_pthreads_n64_t16.cc -o pth_files/gauss_pth_n64_t16.o -lpthread
	g++ -g -std=c++0x pth_files/gauss_inter_pthreads_n256_t16.cc -o pth_files/gauss_pth_n256_t16.o -lpthread
	g++ -g -std=c++0x pth_files/gauss_inter_pthreads_n1024_t16.cc -o pth_files/gauss_pth_n1024_t16.o -lpthread


run_pth_bcast:
	./pth_files/gauss_pth_n64_t2.o 1 inputs/256.txt > logs/pth_bcast_n64_t2.txt
	./pth_files/gauss_pth_n256_t2.o 1 inputs/256.txt > logs/pth_bcast_n256_t2.txt
	./pth_files/gauss_pth_n1024_t2.o 1 inputs/1024.txt > logs/pth_bcast_n1024_t2.txt

	./pth_files/gauss_pth_n64_t4.o  1 inputs/256.txt > logs/pth_bcast_n64_t4.txt
	./pth_files/gauss_pth_n256_t4.o  1 inputs/256.txt > logs/pth_bcast_n256_t4.txt
	./pth_files/gauss_pth_n1024_t4.o  1 inputs/1024.txt > logs/pth_bcast_n1024_t4.txt

	./pth_files/gauss_pth_n64_t8.o  1 inputs/256.txt > logs/pth_bcast_n64_t8.txt
	./pth_files/gauss_pth_n256_t8.o  1 inputs/256.txt > logs/pth_bcast_n256_t8.txt
	./pth_files/gauss_pth_n1024_t8.o  1 inputs/1024.txt > logs/pth_bcast_n1024_t8.txt

	./pth_files/gauss_pth_n64_t16.o  1 inputs/256.txt > logs/pth_bcast_n64_t16.txt
	./pth_files/gauss_pth_n256_t16.o  1 inputs/256.txt > logs/pth_bcast_n256_t16.txt
	./pth_files/gauss_pth_n1024_t16.o  1 inputs/1024.txt > logs/pth_bcast_n1024_t16.txt


pth_pipe:
	g++ -g -std=c++0x pth_pipe_files/gauss_inter_pipe_pthreads_n64_t2.cc -o pth_pipe_files/gauss_pth_n64_t2.o -lpthread
	g++ -g -std=c++0x pth_pipe_files/gauss_inter_pipe_pthreads_n256_t2.cc -o pth_pipe_files/gauss_pth_n256_t2.o -lpthread
	g++ -g -std=c++0x pth_pipe_files/gauss_inter_pipe_pthreads_n1024_t2.cc -o pth_pipe_files/gauss_pth_n1024_t2.o -lpthread

	g++ -g -std=c++0x pth_pipe_files/gauss_inter_pipe_pthreads_n64_t4.cc -o pth_pipe_files/gauss_pth_n64_t4.o -lpthread
	g++ -g -std=c++0x pth_pipe_files/gauss_inter_pipe_pthreads_n256_t4.cc -o pth_pipe_files/gauss_pth_n256_t4.o -lpthread
	g++ -g -std=c++0x pth_pipe_files/gauss_inter_pipe_pthreads_n1024_t4.cc -o pth_pipe_files/gauss_pth_n1024_t4.o -lpthread

	g++ -g -std=c++0x pth_pipe_files/gauss_inter_pipe_pthreads_n64_t8.cc -o pth_pipe_files/gauss_pth_n64_t8.o -lpthread
	g++ -g -std=c++0x pth_pipe_files/gauss_inter_pipe_pthreads_n256_t8.cc -o pth_pipe_files/gauss_pth_n256_t8.o -lpthread
	g++ -g -std=c++0x pth_pipe_files/gauss_inter_pipe_pthreads_n1024_t8.cc -o pth_pipe_files/gauss_pth_n1024_t8.o -lpthread

	g++ -g -std=c++0x pth_pipe_files/gauss_inter_pipe_pthreads_n64_t16.cc -o pth_pipe_files/gauss_pth_n64_t16.o -lpthread
	g++ -g -std=c++0x pth_pipe_files/gauss_inter_pipe_pthreads_n256_t16.cc -o pth_pipe_files/gauss_pth_n256_t16.o -lpthread
	g++ -g -std=c++0x pth_pipe_files/gauss_inter_pipe_pthreads_n1024_t16.cc -o pth_pipe_files/gauss_pth_n1024_t16.o -lpthread


run_pth_pipe:
	./pth_pipe_files/gauss_pth_n64_t2.o 1 inputs/256.txt > logs/pth_pipe_n64_t2.txt
	./pth_pipe_files/gauss_pth_n256_t2.o 1 inputs/256.txt > logs/pth_pipe_n256_t2.txt
	./pth_pipe_files/gauss_pth_n1024_t2.o 1 inputs/1024.txt > logs/pth_pipe_n1024_t2.txt

	./pth_pipe_files/gauss_pth_n64_t4.o  1 inputs/256.txt > logs/pth_pipe_n64_t4.txt
	./pth_pipe_files/gauss_pth_n256_t4.o  1 inputs/256.txt > logs/pth_pipe_n256_t4.txt
	./pth_pipe_files/gauss_pth_n1024_t4.o  1 inputs/1024.txt > logs/pth_pipe_n1024_t4.txt

	./pth_pipe_files/gauss_pth_n64_t8.o  1 inputs/256.txt > logs/pth_pipe_n64_t8.txt
	./pth_pipe_files/gauss_pth_n256_t8.o  1 inputs/256.txt > logs/pth_pipe_n256_t8.txt
	./pth_pipe_files/gauss_pth_n1024_t8.o  1 inputs/1024.txt > logs/pth_pipe_n1024_t8.txt

	./pth_pipe_files/gauss_pth_n64_t16.o  1 inputs/256.txt > logs/pth_pipe_n64_t16.txt
	./pth_pipe_files/gauss_pth_n256_t16.o  1 inputs/256.txt > logs/pth_pipe_n256_t16.txt
	./pth_pipe_files/gauss_pth_n1024_t16.o  1 inputs/1024.txt > logs/pth_pipe_n1024_t16.txt


mpi_bcast:
	mpic++ gauss_inter_mpi.cc -o gauss_bcast_mpi.o

run_mpi_bcast:
	mpirun -n 2 gauss_bcast_mpi.o 64 inputs/256.txt > logs/mpi_bcast_n64_t2.txt
	mpirun -n 2 gauss_bcast_mpi.o 256 inputs/256.txt > logs/mpi_bcast_n256_t2.txt
	mpirun -n 2 gauss_bcast_mpi.o 1024 inputs/1024.txt > logs/mpi_bcast_n1024_t2.txt

	mpirun -n 4 gauss_bcast_mpi.o 64 inputs/256.txt > logs/mpi_bcast_n64_t4.txt
	mpirun -n 4 gauss_bcast_mpi.o 256 inputs/256.txt > logs/mpi_bcast_n256_t4.txt
	#mpirun -n 4 gauss_bcast_mpi.o 1024 inputs/1024.txt > logs/mpi_bcast_n1024_t4.txt

	mpirun -n 8 gauss_bcast_mpi.o 64 inputs/256.txt > logs/mpi_bcast_n64_t8.txt
	mpirun -n 8 gauss_bcast_mpi.o 256 inputs/256.txt > logs/mpi_bcast_n256_t8.txt
	#mpirun -n 8 gauss_bcast_mpi.o 1024 inputs/1024.txt > logs/mpi_bcast_n1024_t8.txt

	mpirun -n 16 gauss_bcast_mpi.o 64 inputs/256.txt > logs/mpi_bcast_n64_t16.txt
	mpirun -n 16 gauss_bcast_mpi.o 256 inputs/256.txt > logs/mpi_bcast_n256_t16.txt
	#mpirun -n 16 gauss_bcast_mpi.o 1024 inputs/1024.txt > logs/mpi_bcast_n1024_t16.txt

mpi_pipe:
	mpic++ gauss_inter_pipe_mpi.cc -o gauss_pipe_mpi.o

run_mpi_pipe:
	mpirun -n 2 gauss_pipe_mpi.o 64 inputs/256.txt > logs/mpi_pipe_n64_t2.txt
	mpirun -n 2 gauss_pipe_mpi.o 256 inputs/256.txt > logs/mpi_pipe_n256_t2.txt
	mpirun -n 2 gauss_pipe_mpi.o 1024 inputs/1024.txt > logs/mpi_pipe_n1024_t2.txt

	mpirun -n 4 gauss_pipe_mpi.o 64 inputs/256.txt > logs/mpi_pipe_n64_t4.txt
	mpirun -n 4 gauss_pipe_mpi.o 256 inputs/256.txt > logs/mpi_pipe_n256_t4.txt
	#mpirun -n 4 gauss_pipe_mpi.o 1024 inputs/1024.txt > logs/mpi_pipe_n1024_t4.txt

	mpirun -n 8 gauss_pipe_mpi.o 64 inputs/256.txt > logs/mpi_pipe_n64_t8.txt
	mpirun -n 8 gauss_pipe_mpi.o 256 inputs/256.txt > logs/mpi_pipe_n256_t8.txt
	#mpirun -n 8 gauss_pipe_mpi.o 1024 inputs/1024.txt > logs/mpi_pipe_n1024_t8.txt

	mpirun -n 16 gauss_pipe_mpi.o 64 inputs/256.txt > logs/mpi_pipe_n64_t16.txt
	mpirun -n 16 gauss_pipe_mpi.o 256 inputs/256.txt > logs/mpi_pipe_n256_t16.txt
	#mpirun -n 16 gauss_pipe_mpi.o 1024 inputs/1024.txt > logs/mpi_pipe_n1024_t16.txt

clean:
	rm *.o
	rm pth_files/*.o
	rm pth_pipe_files/*.o

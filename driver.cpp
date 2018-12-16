#include "seive_algos.cpp"

int main(int argc, char const *argv[])
{
	MPI_Init(NULL,NULL);
	double start_time,computation_end_time,final_end_time;
	start_time = MPI_Wtime();
	int num_processes;
	MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
	int my_rank;
	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	if(ceil(log2(num_processes)) != floor(log2(num_processes)))
	{
		if(my_rank == 0)
			printf("Please give number of processes as a power of 2 for better scalability\n");
		MPI_Finalize();
		return 0;
	}
	long int n;
	n = atol(argv[1]);
	long int sqrtN = ceil(sqrt(n));

	vector<long int> primeTillSqrtN = sqrtNseqSeive(sqrtN);
	long int numSegmentsperProcess = ceil((double)(sqrtN - 1)/num_processes);
	long int numPointsperProcess  = numSegmentsperProcess*sqrtN;
	long int firstNum = sqrtN + (my_rank*numPointsperProcess) + 1;
	long int lastNum = 0;
	if(my_rank != num_processes - 1)
	{
		lastNum = sqrtN + (my_rank*numPointsperProcess) + 1 + numPointsperProcess;
	}
	else
	{
		lastNum = n;
	}
	// printf("first:%ld last:%ld process_id:%d \n",firstNum,lastNum,my_rank);

	partitionedSieve(firstNum,lastNum,primeTillSqrtN,my_rank,sqrtN,n,start_time);
	MPI_Barrier(MPI_COMM_WORLD);
	int height_tree = (int)log2(num_processes);
	int level = 1;
	while(level <= height_tree)
	{
		if(my_rank%(int)pow(2,level) == 0)
		{
			string file_name1 = "file_primes" + to_string(my_rank) + ".txt";
			string file_name2 = "file_primes" + to_string(my_rank+(int)pow(2,level-1)) + ".txt";
			// cout << file_name2 << "\n";
			ofstream myfile1 {file_name1, ios::out | ios::app};
			ifstream myfile2 {file_name2, ios::in};
			myfile1 << myfile2.rdbuf();
			myfile1.close();
			myfile2.close();
		}
		MPI_Barrier(MPI_COMM_WORLD);
		level++;
	}
	if(my_rank != 0)
	{
		string file_to_delete = "file_primes" + to_string(my_rank) + ".txt";
		const char* file_char = file_to_delete.c_str();
		remove(file_char);
	}
	else
	{
		final_end_time = MPI_Wtime();
		printf("Total time taken (including File Writing):  %lf seconds\n",final_end_time - start_time);
	}
	MPI_Finalize();
	return 0;
}

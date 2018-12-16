#include "seive_algos.h"

vector<long int> sqrtNseqSeive(long int num)
{
	vector<long int> primeTillNum;// = new vector<long int>();
	vector<bool> isprime(num,true);
	vector<long int> smallestPrimeFactorVec(num);

	isprime[0] = false;
	isprime[1] = false;
	long int i;
	for (i = 2; i < (num/2) + 1; i++)
	{
		if(isprime[i])
		{
			primeTillNum.push_back(i);
			smallestPrimeFactorVec[i] = i;
		}
		long int j;
		for(long int j = 0; (j < primeTillNum.size()) && (i*primeTillNum[j] < num) && (primeTillNum[j] <= smallestPrimeFactorVec[i]); j++)
		{
			isprime[i*primeTillNum[j]] = false;
			smallestPrimeFactorVec[i*primeTillNum[j]] = primeTillNum[j];
		}
	}
	for(i = (num/2)+1; i < isprime.size(); i++)
	{
		if(isprime[i])
		{
			primeTillNum.push_back(i);
		}
	}
	return primeTillNum;
}

void partitionedSieve(long int firstNum,long int lastNum,vector<long int> primeTillSqrtN,int my_rank,long int sqrtN,long int n,double start_time)
{

	long int lower_num = firstNum;
	long int higher_num = firstNum + sqrtN;
	bool isprime[sqrtN];
	vector<long int> primesFound;
	if(lastNum > n)
	{
		lastNum = n;
	}
	while(lower_num < lastNum && lastNum <= n)
	{
		memset(isprime,true,sizeof(isprime));
		for(long int i = 0; i < primeTillSqrtN.size(); i++)
		{
			long int lowerLimit = floor(lower_num/primeTillSqrtN[i])*primeTillSqrtN[i];
			if(lowerLimit < lower_num)
			{
				lowerLimit += primeTillSqrtN[i];
			}
			for(long int j = lowerLimit; j < higher_num; j += primeTillSqrtN[i])
			{
				isprime[j-lower_num] = false;
			}
		}

		for(long int i = lower_num; i < higher_num; i++)
		{
			if(isprime[i - lower_num])
			{
				primesFound.push_back(i);
			}
		}
		lower_num += sqrtN;
		higher_num += sqrtN;
		if(higher_num >= lastNum)
		{
			higher_num = lastNum;
		}
	}
	double computation_end_time = MPI_Wtime();
	printf("Computation time of process %d:  %lf seconds\n",my_rank,computation_end_time - start_time);
	string file_name = "file_primes" + to_string(my_rank) + ".txt";
	ofstream myfile {file_name, ios::out};
	if(my_rank == 0)
	{
		copy(begin(primeTillSqrtN),end(primeTillSqrtN),ostream_iterator<long int>(myfile,"\n"));
	}
	copy(begin(primesFound),end(primesFound),ostream_iterator<long int>(myfile,"\n"));

	myfile.close();
}

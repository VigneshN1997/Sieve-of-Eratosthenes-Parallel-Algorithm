#include <mpi.h>
#include <iostream>
#include <bits/stdc++.h>
#include <cmath>
#include <string>
#include <cstdio>
#include <fstream>
using namespace std;


vector<long int> sqrtNseqSeive(long int num);
void partitionedSieve(long int firstNum,long int lastNum,vector<long int> primeTillSqrtN,int my_rank,long int sqrtN,long int n,double start_time);
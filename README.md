# Sieve-of-Eratosthenes-Parallel-Algorithm
Implementation of Sieve of Eratosthenes Algorithm in MPI
Designed a parallel version of the Sieve of Eratosthenes algorithm to find all primes less than N.
Implemented this parallel algorithm using MPI in C

Measured the performance for different values of N in the range 10^5 to 10^10. For each value of N, measured the performance for p = 1, 2, 4, and 8 where p is the number of (physical) nodes used and plotted a curve indicating how performance (of a single input) varies as p increases.

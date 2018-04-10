#include <iostream>
#include<pthread.h>
#include <stdlib.h>
#include <sys/time.h>			//using struct timeval

/*struct timeval {
 *  long float tv_sec;
 *  long float tv_usec;}*/


struct timeval start_time, stop_time;

using namespace std;
#define N  100000000
int    No_of_threads;
long long int SUM =0;
int    start[500];
double x[N];
pthread_t tid[500];

//mutex ensures mutual exclusion by locking the critical section
//If a mutex is unlocked and a thread calls lock, the mutex locks and the thread continues.
//If however the mutex is locked, the thread blocks until the thread 'holding' the lock calls unlock.
pthread_mutex_t lock;


//routine to be called just after creating a thread to execute required functions on array
//takes the start[i] i.e. the thread sequence number as argument here
//Calculates the partial sum of N/No_of_threads numbers
void *worker(void *arg)
{
   int i, s;
   int n, start, stop;
   double temp_sum;

   n = N/No_of_threads; //get the no of numbers to be processed in current thread

   s = * (int *) arg; 	//get thread sequence number 

   start = s * n;		//get the starting index of the numbers to be processed

   if ( s != (No_of_threads-1) ) //if this is not the last created thread
   {
      stop = start + n;	  //get the end index
   }
   else				//if this is  the last thread-->calculate the rest
   {
      stop = N;
   }

   //calculate the partial sum
   temp_sum = x[start];			
   for (i = start+1; i < stop; i++ )
   {

         temp_sum+= x[i];
   }
   
   //lock mutex
   pthread_mutex_lock(&lock);
   //critical section
   SUM += temp_sum;			//add partial sum to total sum  
   //unlock mutex
   pthread_mutex_unlock(&lock);
   
   return(NULL);     
}


int main(int argc, char *argv[])
{
   int i;
   double temp_sum;
   float elapsed;
   cout<< "\n\n\t\tSumming array elements using Thread programming\n\n " << endl;
   
   if ( argc != 2 )
   {
      std::cout << "Number of argument is wrong\nNo_of_threads is missing" << std::endl;
      exit(1);
   }

   //Initialize random number generator with current time as seed
   srand(time(NULL));
   
   //produce the random numbers -->5<=n<105
   for (i = 0; i < N; i++)	   	
	x[i] =rand()%100+5;

   //string to integer function atoi -->get no. of threads
   No_of_threads = atoi(argv[1]);
   std::cout << "Number of threads: " << No_of_threads << std::endl;

   //get current time
   gettimeofday(&start_time, NULL);

   //create the threads 
   /*int pthread_create(pthread_t *thread, const pthread_attr_t *attr,void *(*start_routine) (void *), void *arg);
	*return 0 if success else return error number 
	*/
   for (i = 0; i < No_of_threads; i = i + 1)
   {
      start[i] = i;
	  //worker routine is called after thread creation
      if ( pthread_create(&tid[i], NULL, worker, (void *)&start[i]) )   //tid-->thread id, start is global array to keep track of track sequence
      {
		//control comes inside loop only if thread creation fails i.e. returns non-zero value
        std::cout << "Cannot create thread" << std::endl;				
        exit(1);
      }
   }

   for (i = 0; i < No_of_threads; i = i + 1)
	  //One Thread can wait on the termination of another by using pthread_join()
      pthread_join(tid[i], NULL);
   
   //Deallocates any memory or other resources associated with the mutex.
   pthread_mutex_destroy(&lock);

   gettimeofday(&stop_time, NULL); //get current time


   //calculate time elapsed
   elapsed = ((stop_time.tv_sec*1000000 + stop_time.tv_usec) -
   		(start_time.tv_sec*1000000 + start_time.tv_usec));	

	//print
   std::cout << "Count of Numbers: "<<N<<std::endl;
   std::cout << "Sum: " << SUM  << std::endl; 
   std::cout << "Time taken: " << elapsed/1000000 << " seconds" << std::endl;
}



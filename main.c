#include "stdio.h"
#include "heuristiqueErkang.c"
#include "init.c"
#include "job.c"
#include "time.h"

 
int main()
{
    char* inputfile = "data2.txt";
    Job **jobs = malloc(MAX_JOB * sizeof(Job*));
    int nbJobs=initJobs(inputfile, jobs);
    smithOrder(jobs,nbJobs,1);
    smithOrder(jobs,nbJobs,0);
    Kheuristique(jobs, nbJobs);
    Rheuristique(jobs,nbJobs);
    return 0;

}

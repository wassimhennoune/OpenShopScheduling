//
// Created by zhang on 14/01/17.
//

#include "heuristiqueErkang.h"
#include "tool.c"
#include "time.h"


void smithOrder(Job** jobs, int nbJobs,int desc)
{
    /*
     * result: 2d-array of [NB_MACHINES][NB_JOBS]
     * Each row stands for a scheduling sequence for a machine
     * Initialisation with 0 values
     */
    printf("*************************************\n");
    printf("Smith order's heuristic\n");

    int ** result = (int**) malloc(NB_MACHINES * sizeof(int*));
    if (result == NULL)
    {
        exit(1);
    }
    initializeResultTable(result, nbJobs);


    // 1. calculate the critical sequence
    /*
     * Critical sequence: the smith order on the critical machine
     * Smith order: sorting jobs by (processing time / weight)
     */
    printf("*************************************\n");
    printf("Step 1: calculate the critical sequences (smith values)\n");
    double smithValues[NB_MACHINES][nbJobs];

    for(int j=0; j <NB_MACHINES;j++)
    for(int i = 0; i < nbJobs; i++)
    {
        smithValues[j][i]=(double)jobs[i]->pt[j] / jobs[i]->w;
    }

    int m1_sequenceSmith[nbJobs];
    int m2_sequenceSmith[nbJobs];
    int m3_sequenceSmith[nbJobs];
    int m4_sequenceSmith[nbJobs];

    sortArray(smithValues[0], nbJobs, m1_sequenceSmith);
    sortArray(smithValues[1], nbJobs, m2_sequenceSmith);
    sortArray(smithValues[2], nbJobs, m3_sequenceSmith);
    sortArray(smithValues[3], nbJobs, m4_sequenceSmith);


    // 3. apply the critical sequence to all the machines
    /*
     * Key point of heuristic
     */
    printf("*************************************\n");
    printf("Step 2: apply the critical sequences to all the machines\n");
        int k ;
        for (int i = 0; i < nbJobs; i++)
        {
            if(desc) k = nbJobs - 1 - i;
            else  k = i;
            result[0][i] = m1_sequenceSmith[k];
            result[1][i] = m2_sequenceSmith[k];
            result[2][i] = m3_sequenceSmith[k];
            result[3][i] = m4_sequenceSmith[k];
        }

    printResultTable(result, nbJobs);


    // 4. scheduling
    /*
     * Key function: scheduling. Scheduling order must be defined before calling scheduling.
     *
     * This step is for calculating the start time for each job on each machine according to
     * the result table.
     *
     */
    printf("*************************************\n");
    printf("Step 3: Scheduling\n");

    //scheduleOnAllMachines(result, jobs, nbJobs, indexCriticalMachine);
    int order[NB_MACHINES] = {0,1,2,3};
    printf("Scheduling order is: %d -> %d -> %d -> %d\n\n",
                          order[0], order[1], order[2], order[3]);
    schedule(result, order, jobs, nbJobs);

    // 5. print result
    double wct = weightedCompleteTime(jobs, nbJobs);
    printf("Weighted completion time: %f \n", wct);
    printf("\nEnd\n\n");
    //save the result for visualisation
    if(desc)   saveJobs(jobs, nbJobs, "desc_smithOrder.txt");
    else   saveJobs(jobs, nbJobs, "smithOrder.txt");
    // 6. clean memory usage
    for (int i = 0; i < NB_MACHINES; i++)
    {
        free(result[i]);
    }
    free(result);
}

void Kheuristique(Job** jobs, int nbJobs)
{
    /*
     * result: 2d-array of [NB_MACHINES][NB_JOBS]
     * Each row stands for a scheduling sequence for a machine
     * Initialisation with 0 values
     */
    printf("*************************************\n");
    printf("K's heuristic\n");

    int ** result = (int**) malloc(NB_MACHINES * sizeof(int*));
    if (result == NULL)
    {
        exit(1);
    }
    initializeResultTable(result, nbJobs);


    // 1. order the jobs
    printf("*************************************\n");
    printf("Step 1. order the jobs\n");
    double m1_smithValues[nbJobs];
    double m2_smithValues[nbJobs];
    for(int i = 0; i < nbJobs; i++)
    {
        m1_smithValues[i]=(double)jobs[i]->pt[0] / jobs[i]->w;
       
        m2_smithValues[i]=(double)jobs[i]->pt[1] / jobs[i]->w;
         printf("job %d : %d / %d = %.2f \n",i,jobs[i]->pt[1] , jobs[i]->w ,  m2_smithValues[i]);
    }
    int m1_sequenceSmith[nbJobs];
    int m2_sequenceSmith[nbJobs];
    sortArray(m1_smithValues, nbJobs, m1_sequenceSmith); //trie le tableau par ordre decroissant
    sortArray(m2_smithValues, nbJobs, m2_sequenceSmith);

    printf("\nThe Smith sequence calculated for the first machine: (ordre croissant) \n");
    for (int i = nbJobs - 1; i >= 0; i--)
    {
            printf("-> (%d,%.2f) ", m1_sequenceSmith[i],m1_smithValues[m1_sequenceSmith[i]]);
    }
    printf("\nThe Smith sequence calculated for the second machine: (ordre decroissant) \n");
    for (int i = 0; i < nbJobs; i++)
    {
             printf("-> (%d,%.2f) ", m2_sequenceSmith[i],m2_smithValues[m2_sequenceSmith[i]]);
    }
     // 2. apply the critical sequence to all the machines
    /*
     * Key point of heuristic
     */
    printf("*************************************\n");
    printf("Step 2: apply the critical sequence to all the machines\n");

    for (int i = 0; i < nbJobs; i++)
    {
        result[0][i] = m1_sequenceSmith[nbJobs - 1 - i];
        result[1][i] = m2_sequenceSmith[i];
        result[2][i] = m1_sequenceSmith[i];
        result[3][i] = m2_sequenceSmith[nbJobs - 1 - i];
    }
    printResultTable(result, nbJobs);
    // 4. scheduling

    printf("*************************************\n");
    printf("Step 4: Scheduling\n");

    int order[NB_MACHINES] = {0,1,2,3};
    printf("Scheduling order is: %d -> %d -> %d -> %d\n\n",
                          order[0], order[1], order[2], order[3]);
    schedule(result, order, jobs, nbJobs);

    // 5. print result
    double wct = weightedCompleteTime(jobs, nbJobs);
    printf("Weighted completion time: %f \n", wct);
    printf("\nEnd\n\n");
    //save the result for visualisation
    saveJobs(jobs, nbJobs, "KOrder.txt");
    // 6. clean memory usage
    for (int i = 0; i < NB_MACHINES; i++)
    {
        free(result[i]);
    }
    free(result);
  
}

void Rheuristique(Job** jobs, int nbJobs)
{
    /*
     * result: 2d-array of [NB_MACHINES][NB_JOBS]
     * Each row stands for a scheduling sequence for a machine
     * Initialisation with 0 values
     */
    printf("*************************************\n");
    printf("R's heuristic\n");

    int ** result = (int**) malloc(NB_MACHINES * sizeof(int*));
    if (result == NULL)
    {
        exit(1);
    }
    initializeResultTable(result, nbJobs);


    // 1. order the jobs randomly
    printf("*************************************\n");
    printf("Step 1: order the jobs randomly on each machine\n");

    for(int i = 0 ; i < NB_MACHINES;i++)
    {
        for(int j = 0 ; j<nbJobs;j++)
        {
            result[i][j]=j;
        }
    }
    //shuffle the order 
    srand ( time(NULL) );
    for(int i = 0 ; i < NB_MACHINES;i++)
    {
        for(int j = 0 ; j<nbJobs;j++)
        {
            size_t k = j + rand() / (RAND_MAX / (nbJobs - j) + 1);
            int t = result[i][k];
            result[i][k] = result[i][j];
            result[i][j] = t;
        }
    }
    printResultTable(result, nbJobs);
    // 4. scheduling

    printf("*************************************\n");
    printf("Step 2: Scheduling\n");

    int order[NB_MACHINES] = {0,1,2,3};
    printf("Scheduling order is: %d -> %d -> %d -> %d\n\n",
                          order[0], order[1], order[2], order[3]);
    schedule(result, order, jobs, nbJobs);

    // 5. print result
    double wct = weightedCompleteTime(jobs, nbJobs);
    printf("Weighted completion time: %f \n", wct);
    printf("\nEnd\n\n");
    //save the result for visualisation
    saveJobs(jobs, nbJobs, "ROrder.txt");
    // 6. clean memory usage
    for (int i = 0; i < NB_MACHINES; i++)
    {
        free(result[i]);
    }
    free(result);
  
}
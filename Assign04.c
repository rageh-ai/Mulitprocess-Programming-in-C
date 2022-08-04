#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct process
{
    int processNumber;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int waitTime;
};


void roundRobinScheduling(struct process[50], int, FILE*);

void fcfsScheduling(struct process[50], int, FILE*);

void nsfjScheduling(struct process[50], int, FILE*);

void psfjScheduling(struct process[50], int, FILE*);

void resetStructParameters(struct process[50], int);



struct process processes[50];





int main()
{
    FILE* taskInputFile = fopen("TaskSpec.txt", "r");
    char str[50][7];
    int lineCounter = 0;
    int i = 0;


    while (fgets(str[i], 7, taskInputFile) != NULL)
    {
        ++i;
        fgets(str[i], 7, taskInputFile);
    }
    i-= 1;
  
    

    while (lineCounter < i)
    {
        processes[lineCounter].processNumber = atoi(&str[lineCounter][1]);
        processes[lineCounter].arrivalTime = atoi(&str[lineCounter][3]);
        processes[lineCounter].burstTime = atoi(&str[lineCounter][5]);

        lineCounter++;
    }

    

    fclose(taskInputFile);

    FILE* outputStorageFile = fopen("Output.txt", "w");


    fcfsScheduling(processes, i, outputStorageFile);
    fprintf(outputStorageFile, "\n");
    resetStructParameters(processes, i);
    roundRobinScheduling(processes, i, outputStorageFile);
    fprintf(outputStorageFile, "\n");
    resetStructParameters(processes, i);    
    nsfjScheduling(processes, i, outputStorageFile);
    fprintf(outputStorageFile, "\n");
    resetStructParameters(processes, i);
    psfjScheduling(processes, i, outputStorageFile);

    

    return 0;
}

void resetStructParameters(struct process schedulingList[50], int length)
{
    for (int y = 0; y < length; ++y)
    {
        schedulingList[y].remainingTime = schedulingList[y].burstTime;
        schedulingList[y].waitTime = 0;

    }
}



void psfjScheduling(struct process schedulingList[50], int length, FILE* outputStorageFile)
{
    fprintf(outputStorageFile, "PSFJ:\n");
    int startTime;
    int endTime = 0;
    int finishedCounter = 0;
    float avgWaitTime = 0;
    int waitTime;
    int timeAfterArrival;
    struct process tempProccess;
    int i = 0;
    int j = i+1; 
    int sumTime = 0;
    int k = 0;


    for (int i = 0; i < length - 1; ++i)
    {
        for (int j = 0; j < length -1; ++j)
        {
            if (schedulingList[j].arrivalTime > schedulingList[j+1].arrivalTime)
            {
                tempProccess = schedulingList[j+1];
                schedulingList[j+1] = schedulingList[j];
                schedulingList[j] = tempProccess;
            }
        }

    }
    

    
    
    while(j< length)
    {
        startTime = endTime;
        timeAfterArrival = schedulingList[i].remainingTime - schedulingList[j].arrivalTime;

        if (timeAfterArrival > schedulingList[j].remainingTime)
        {
            endTime += schedulingList[j].arrivalTime;
            schedulingList[i].remainingTime -= schedulingList[j].arrivalTime;
            for (int h = 0; h < length; ++h)
            {
                if (h != i && schedulingList[h].remainingTime != 0)
                {
                    schedulingList[h].waitTime += endTime - startTime - schedulingList[i+1].arrivalTime;
                }
            }
            
            //print neccessary details
            //increment i since the time is shorter we move on to the next one.
            fprintf(outputStorageFile, "T%d", schedulingList[i].processNumber);
            fprintf(outputStorageFile, "%5d", startTime);
            fprintf(outputStorageFile, "%5d\n", endTime);
            ++i;
        }

            
        

        // what if the time isnt shorter then we will continue execution

        
        else if (timeAfterArrival <= schedulingList[j].remainingTime && j == length - 1)
        {
            endTime += schedulingList[i].burstTime;
            fprintf(outputStorageFile, "T%d", schedulingList[i].processNumber);
            fprintf(outputStorageFile, "%5d", startTime);
            fprintf(outputStorageFile, "%5d\n", endTime);
            for (int h = 0; h < length; ++h)
            {
                if (h != i && schedulingList[h].remainingTime != 0)
                {
                    schedulingList[h].waitTime += endTime - schedulingList[i+1].arrivalTime;;
                }
            }
            schedulingList[i].remainingTime = 0;
            //then this value is printed and we should break from this loop and switch methods to the sjf algortihm
        }
        ++j;
       
    }
    
    int min = 0;
    int index;

    while (k < length)
    {

        startTime = endTime;

        while (schedulingList[k].remainingTime == 0 && k < length)
        {
            k++;
        }
        min = schedulingList[k].remainingTime;

        for (int x = 0; x < length; ++x)
        {
            if (schedulingList[x].remainingTime < min && schedulingList[x].remainingTime != 0 )
            {
                min = schedulingList[x].remainingTime;
                index = x;   
            }
        }

        endTime += min;



        



        for (int z = 0; z < length; ++z)
        {
            if (z != index && schedulingList[z].remainingTime != 0 )
            {
                if (schedulingList[z].remainingTime == schedulingList[z].burstTime )
                {
                    if (z != length -1)
                    {
                        schedulingList[z].waitTime = endTime - schedulingList[z+1].arrivalTime;
                    }
                    else
                    {
                        schedulingList[z].waitTime = endTime;
                    }
                }
                else
                {
                    schedulingList[z].waitTime = endTime - (schedulingList[z].burstTime - schedulingList[z].remainingTime);
                }
            }
        }


        
        fprintf(outputStorageFile, "T%d", schedulingList[index].processNumber);
        fprintf(outputStorageFile, "%5d", startTime);
        fprintf(outputStorageFile, "%5d\n", endTime);
        schedulingList[index].remainingTime = 0;
            
        k++;
    
    }
    




    for (int f = 0; f < length; ++f)
    {
        fprintf(outputStorageFile, "Waiting Time T%d: %d \n", schedulingList[f].processNumber, schedulingList[f].waitTime);
        avgWaitTime += schedulingList[f].waitTime;
    }

    avgWaitTime = avgWaitTime/length;
    fprintf(outputStorageFile,"Average Waiting Time: %0.2f", avgWaitTime);


    fprintf(outputStorageFile, "\n" );
}
    

    
    
    



void nsfjScheduling(struct process schedulingList[50], int length, FILE* outputStorageFile)
{
    fprintf(outputStorageFile, "NSFJ:\n");
    struct process tempProccess = schedulingList[0];
    int startTime;
    int endTime = 0;
    int k = 0;
    int finishedCounter = 0;
    float avgWaitTime = 0;
    int counter = 1;
    int waitTime;


    //implement bubble sort algorthim from previous Assignment.

    for (int i = 0; i < length - 1; ++i)
    {
        for (int j = 0; j < length -1; ++j)
        {
            if (schedulingList[j].burstTime > schedulingList[j+1].burstTime)
            {
                tempProccess = schedulingList[j+1];
                schedulingList[j+1] = schedulingList[j];
                schedulingList[j] = tempProccess;
            }
        }

    }

    for (int j = 0; j < length; ++j)
    {
        if (finishedCounter == length)
        {
            break;
        }

        fprintf(outputStorageFile, "T%d", schedulingList[j].processNumber);


        schedulingList[j].remainingTime = 0;
        schedulingList[j].waitTime = endTime;
        
        startTime = endTime;
        fprintf(outputStorageFile, "%5d", startTime);


        endTime += schedulingList[j].burstTime;
        fprintf(outputStorageFile, "%5d", endTime);

        fprintf(outputStorageFile,"\n");
        finishedCounter++;


     

    }

    for (int z = 0; z < length; ++z)
    {
      if (schedulingList[z].processNumber == counter)
      {
          avgWaitTime += schedulingList[z].waitTime;

          fprintf(outputStorageFile, "Waiting Time T%d: %d\n", counter, schedulingList[z].waitTime);
          z = -1;
          counter++;    
      }
    }

    avgWaitTime = avgWaitTime / length;

    fprintf(outputStorageFile, "Average Waiting Time: %0.2f\n", avgWaitTime);

    

}




 

void fcfsScheduling(struct process schedulingList[50], int length, FILE* outputStorageFile)
{
    fprintf(outputStorageFile, "FCFS:\n");


    int k = 0;
    int startTime;
    int endTime = 0;
    int finishedCounter = 0;
    float avgWaitTime = 0;


    for (int i = 0; i < length; ++i)
    {
        k = i+1;
        if (finishedCounter == length)
        {
            break;
        }

        schedulingList[i].remainingTime = 0;

        fprintf(outputStorageFile, "T%d", schedulingList[i].processNumber);
        startTime = endTime;
        fprintf(outputStorageFile, "%5d", startTime);
        endTime += schedulingList[i].burstTime;
        fprintf(outputStorageFile, "%5d\n", endTime);

        


        while (k < length)
        {
            
            schedulingList[k].waitTime += schedulingList[i].burstTime;
            k++;
        }
        finishedCounter++;
    }

    
    

    for (int i = 0; i < length; ++i)
    {
        fprintf(outputStorageFile, "Waiting Time T%d: %d \n", schedulingList[i].processNumber, schedulingList[i].waitTime);
        avgWaitTime += schedulingList[i].waitTime;
    }

    avgWaitTime = avgWaitTime / length;

    fprintf(outputStorageFile, "Average Waiting Time: %0.2f\n", avgWaitTime);


}



















void roundRobinScheduling( struct process schedulingList[50], int length, FILE* outputStorageFile )
{
    int timeQuantum = 4;
    int waitTime;
    int startTime = 0;
    int endTime = 0;
    int finishedCounter = 0;
    int j = 0;
    int k = 0;
    int avgWaitTime;


    fprintf(outputStorageFile, "RR:\n");


    for (int i = 0; i < length; ++i)
    {
         schedulingList[i].remainingTime = schedulingList[i].burstTime;
         schedulingList[i].waitTime = 0;
    }

    


    while(length != finishedCounter)
    {
        k = 0;
        if (schedulingList[j].remainingTime != 0)
        {
            schedulingList[j].remainingTime = schedulingList[j].remainingTime - timeQuantum;
            startTime = endTime;
            fprintf(outputStorageFile, "T%d", schedulingList[j].processNumber);
            fprintf(outputStorageFile, "%5d", startTime);

            if (schedulingList[j].remainingTime > 0)
            {
                endTime += timeQuantum;
                fprintf(outputStorageFile,"%5d", endTime );
            }


            else if (schedulingList[j].remainingTime == 0)
            {   
                endTime += timeQuantum;
                fprintf(outputStorageFile,"%5d", endTime );
                schedulingList[j].remainingTime = 0;
                finishedCounter++;
            }

            else if (schedulingList[j].remainingTime < 0)
            {
                endTime += schedulingList[j].remainingTime + timeQuantum;
                fprintf(outputStorageFile,"%5d", endTime );
                schedulingList[j].remainingTime = 0;
                finishedCounter++;
            }
            waitTime = endTime - startTime;

            while (k < length)
            {
                if (k != j && !(schedulingList[k].remainingTime <= 0))
                {
                    schedulingList[k].waitTime += waitTime;
                }
                k++;
            }
            

            

            fprintf(outputStorageFile, "\n");
        }
        j++;
        
        if (j == length)
        {
            j = 0;
        }
        
    }
    float waitTimeValue = 0;

    for (int z = 0; z < length; z++)
    {

        waitTimeValue += schedulingList[z].waitTime ;
        fprintf(outputStorageFile, "Waiting Time T%d: %d\n",z+1,  schedulingList[z].waitTime );
    }
    waitTimeValue = waitTimeValue / length;
    fprintf(outputStorageFile, "Average Waiting Time: %0.2f\n", waitTimeValue);


}








    

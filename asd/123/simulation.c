#include "_fake_defines.h"
#include "_fake_typedefs.h"
















//function to read input
void readInit(struct vec3 *output)
{
    FILE *data;
    data = fopen("Trajectory.txt", "r");
    int i, count = 0;
    if (data == NULL)
    {
        printf("Failed to open file: \"Trajectory.txt\"\nExiting..");
        exit(1);
    }

    printf("Reading the Initial Positions of the N bodies...\n");
    fseek(data, 249, SEEK_SET);

    i = 0;
    while (i < 1000)
    {
        fscanf(data, "%f %f %f", &output[i].x, &output[i].y, &output[i].z);
        i++;
    }
    fclose(data);
}

void generateOutputFile(struct vec3 ** outLog, int numIters)
{
    FILE *trajec;
    trajec = fopen("New_Trajectory.txt", "w");

    if (trajec == NULL)
    {
        printf("Failed to open file: \"New_Trajectory.txt\"\nExiting..");
        exit(1);
    }

    for (int i = 0; i < numIters / 100; i++)
    {
        for (int j = 0; j < 1000; j++)
        {
            fprintf(trajec, "%-8.2f", outLog[i][j].x);
        }
        fprintf(trajec, "\n");
        for (int j = 0; j < 1000; j++)
        {
            fprintf(trajec, "%-8.2f", outLog[i][j].y);
        }
        fprintf(trajec, "\n");
        for (int j = 0; j < 1000; j++)
        {
            fprintf(trajec, "%-8.2f", outLog[i][j].z);
        }
        fprintf(trajec, "\n\n");
    }
}

int main()
{
    //defining parameters
    int numParticles = 1000;
    float delT = 0.01;
    int numIters = 360000; // 720000;
    float M = 1;
    float xSize = 100, ySize = 200, zSize = 400;
    float bodyRadius = 0.5;

    //defining and allocating arrays to be used during simulation
    struct vec3 *posN, *posN1, *velN, *velNhalf, **outLog;
    posN = (struct vec3 *)malloc(numParticles * sizeof(struct vec3));
    posN1 = (struct vec3 *)malloc(numParticles * sizeof(struct vec3));
    velN = (struct vec3 *)malloc(numParticles * sizeof(struct vec3));
    velNhalf = (struct vec3 *)malloc(numParticles * sizeof(struct vec3));
    outLog = (struct vec3 **)malloc((numIters / 100) * sizeof(struct vec3 *));  //stores trajectories to be written in output file
    for (int i = 0; i < numIters / 100; i++)
    {
        outLog[i] = (struct vec3 *)malloc(numParticles * sizeof(struct vec3));
    }

    readInit(posN); //read input file

    omp_set_num_threads(8);

    double totalTime = omp_get_wtime(); //variables to count time

    //looping over all timesteps
    for (int i = 0; i < numIters; i++)
    {
        if(i%10000 == 0)printf("%d\n", i);
        //first parallel loop over all particles to calculate force and update speed and positions
        #pragma omp parallel for
        for (int j = 0; j < numParticles; j++)
        {
            //fN stores force on the particle
            struct vec3 fN;
            fN.x = 0, fN.y = 0, fN.z = 0;

            //looping through all other particles and adding up forces
            for (int k = 0; k < numParticles; k++)
            {
                if (k == j)
                    continue;

                float distSq = lenSquared(diff(posN[k], posN[j]));
                struct vec3 dir = normalize(diff(posN[k], posN[j]));
                struct vec3 force = constMul(dir, (M * M / distSq));

                fN = sum(fN, force);
            }

            struct vec3 vHalf = sum(velN[j], constMul(fN, delT / (2 * M)));   //calculate half step velocity
            posN1[j] = sum(posN[j], constMul(vHalf, delT));    //update position, store it in posN1 (not posN)
            velNhalf[j] = vHalf;                               
            velNhalf[j] = checkBounds(posN1[j], velNhalf[j], xSize, ySize, zSize, bodyRadius);   //check if particle is outside bounds, update if needed
        }

        //second parallel loop to update velocities and forces
        #pragma omp parallel for
        for (int j = 0; j < numParticles; j++)
        {
            struct vec3 fN;
            fN.x = 0, fN.y = 0, fN.z = 0;
            
            //loop through all other particles
            for (int k = 0; k < numParticles; k++)
            {
                if (k == j)
                    continue;

                float distSq = lenSquared(diff(posN1[k], posN1[j]));
                struct vec3 dir = normalize(diff(posN1[k], posN1[j]));
                struct vec3 force = constMul(dir, (M * M / distSq));

                // chheck for collisions wiht other particles, exchange velocities on collision
                // if (distSq <= 2 * bodyRadius)
                // {
                //     struct vec3 temp = velNhalf[k];
                //     velNhalf[k] = velNhalf[j];
                //     velNhalf[j] = temp;
                // }

                fN = sum(fN, force);
            }
            //update the full step velcity and position
            struct vec3 vFull = sum(velNhalf[j], constMul(fN, delT / (2 * M)));
            velN[j] = vFull;
            posN[j] = posN1[j];

            //save in output trajectory at every 100th iteration
            if (i % 100 == 0)
            {
                outLog[i / 100][j] = (posN[j]);
            }
        }
    }

    totalTime = omp_get_wtime() - totalTime;
    printf("Total Simulation Time: %lf\n", totalTime);
    printf("Average Step Time: %lf\n", totalTime/numIters);

    //save output to file
    generateOutputFile(outLog, numIters);
}
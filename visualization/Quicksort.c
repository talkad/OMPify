#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <time.h>

// array init modes -> "random", "crescent", "decrescent"
#define ARRAY_INIT_MODE "random"
#define ARRAY_SIZE 500000
// quicksort modes -> "sequential", "tasks", "tasks_and_for", "sections"
#define QUICKSORT_MODE "tasks_and_for"
#define parallelStop partitionSize <= 2500
/*
 * 5000
 * 2500 **
 * 1200
 * 300
 */

void swap(int index1, int index2, int *array)
{
    int copy1 = array[index1];
    
    array[index1] = array[index2];
    array[index2] = copy1;
}

// ------------------- QUICKSORT PARALELO COM TAREFAS

void parallelDivideTasks(int initialLeft, int initialRight, int *array)
{
    int left = initialLeft;
    int right = initialRight;
    int pivot = array[ (left + right) / 2 ];
    int leftElement;
    int rightElement;
    const int partitionSize = initialRight - initialLeft + 1;
    
    while (left <= right)
    {
        leftElement = array[left];
        rightElement = array[right];

        // cria uma tarefa para alguma thread executar e
        // compartilha com essa tarefa as variaveis left e
        // leftElement da thread atual
        //#pragma omp task shared(left, leftElement)
        while (left < initialRight && leftElement < pivot)
        {
            leftElement = array[++left];
        }

        //#pragma omp task shared(right, rightElement)
        while (right > initialLeft && rightElement > pivot)
        {
            rightElement = array[--right];
        }
        // espera as tarefas que essa thread criou terminar
        //#pragma omp taskwait

        if (left <= right)
        {
            swap(left++, right--, array);
        }
    }
    
    if (parallelStop)
    {
        if (right > initialLeft)
        {
            parallelDivideTasks(initialLeft, right, array);
        }

        if (left < initialRight)
        {
            parallelDivideTasks(left, initialRight, array);
        }
    }
    
    else
    {
        if (right > initialLeft)
        {
            #pragma omp task
            parallelDivideTasks(initialLeft, right, array);
        }

        if (left < initialRight)
        {
            #pragma omp task
            parallelDivideTasks(left, initialRight, array);
        }
    }
}

void parallelQuicksortTasks(int *array, int arrayLength)
{
    #pragma omp parallel // omp parallel -> cria uma regiao paralela
    // omp single -> apenas uma thread executa a proxima linha,
    // alem disso, as outras threads so' iniciam suas atividades
    // depois que essa linha for executada
    #pragma omp single
    parallelDivideTasks(0, arrayLength - 1, array);
}

// ------------------- FIM DO QUICKSORT PARALELO COM TAREFAS

// ------------------- QUICKSORT PARALELO COM FOR E TAREFAS

void parallelDivideFor(int initialLeft, int initialRight, int *array)
{
    const int partitionSize = initialRight - initialLeft + 1;
    int pivot = array[ (initialLeft + initialRight) / 2 ];
    int lessThanPivot[partitionSize];
    int greaterThanPivot[partitionSize];
    int ltPivotCounter = 0;
    int gtPivotCounter = 0;
    int currentElement;
    int i;
    
    if (parallelStop)
    {
        // percorre o arranjo separando os elementos que
        // sao menores que o pivo e maiores ou iguais a ele
        for (i = 0; i < partitionSize; i++)
        {
            currentElement = array[initialLeft + i];

            if (currentElement < pivot)
            {
                lessThanPivot[ltPivotCounter++] = currentElement;
            }

            else
            {
                greaterThanPivot[gtPivotCounter++] = currentElement;
            }
        }
        
        // percorre o arranjo dos menores que o pivo e os
        // coloca na parte esquerda do arranjo original
        for (i = 0; i < ltPivotCounter; i++)
        {
            array[initialLeft + i] = lessThanPivot[i];
        }
        
        // percorre o arranjo dos maiores ou iguais ao pivo
        // e os coloca na parte direita do arranjo original
        for (i = 0; i < gtPivotCounter; i++)
        {
            array[initialLeft + ltPivotCounter + i] = greaterThanPivot[i];
        }
        
        if (ltPivotCounter > 1)
        {
            parallelDivideFor(initialLeft, initialLeft + ltPivotCounter - 1, array);
        }

        if (gtPivotCounter > 1)
        {
            parallelDivideFor(initialLeft + ltPivotCounter, initialLeft + ltPivotCounter + gtPivotCounter - 1, array);
        }
    }
    
    else
    {
        #pragma omp parallel for private(currentElement)
        for (i = 0; i < partitionSize; i++)
        {
            currentElement = array[initialLeft + i];

            if (currentElement < pivot)
            {
                lessThanPivot[ltPivotCounter++] = currentElement;
            }

            else
            {
                greaterThanPivot[gtPivotCounter++] = currentElement;
            }
        }
        
        // cria uma tarefa para alguma thread executar e,
        // ao mesmo tempo, uma copia local de i para a thread
        // que for executar a tarefa
        #pragma omp task private(i)
        for (i = 0; i < ltPivotCounter; i++)
        {
            array[initialLeft + i] = lessThanPivot[i];
        }
        
        #pragma omp task private(i)
        for (i = 0; i < gtPivotCounter; i++)
        {
            array[initialLeft + ltPivotCounter + i] = greaterThanPivot[i];
        }
        // espera as tarefas que essa thread criou terminar
        #pragma omp taskwait
        
        if (ltPivotCounter > 1)
        {
            #pragma omp task
            parallelDivideFor(initialLeft, initialLeft + ltPivotCounter - 1, array);
        }

        if (gtPivotCounter > 1)
        {
            #pragma omp task
            parallelDivideFor(initialLeft + ltPivotCounter, initialLeft + ltPivotCounter + gtPivotCounter - 1, array);
        }
    }
}

void parallelQuicksortFor(int *array, int arrayLength)
{
    #pragma omp parallel // omp parallel -> cria uma regiao paralela
    // omp single -> apenas uma thread executa a proxima linha,
    // alem disso, as outras threads so' iniciam suas atividades
    // depois que essa linha for executada
    #pragma omp single
    parallelDivideFor(0, arrayLength - 1, array);
}

// ------------------- FIM DO QUICKSORT PARALELO COM FOR E TAREFAS

// ------------------- QUICKSORT PARALELO COM SECOES

void parallelDivideSections(int initialLeft, int initialRight, int *array)
{
    int left = initialLeft;
    int right = initialRight;
    int pivot = array[ (left + right) / 2 ];
    int leftElement;
    int rightElement;
    const int partitionSize = initialRight - initialLeft + 1;
    
    while (left <= right)
    {
        leftElement = array[left];
        rightElement = array[right];
        
        while (left < initialRight && leftElement < pivot)
        {
            leftElement = array[++left];
        }
        
        while (right > initialLeft && rightElement > pivot)
        {
            rightElement = array[--right];
        }
        
        if (left <= right)
        {
            swap(left++, right--, array);
        }
    }
    
    if (parallelStop)
    {
        if (right > initialLeft)
        {
            parallelDivideSections(initialLeft, right, array);
        }

        if (left < initialRight)
        {
            parallelDivideSections(left, initialRight, array);
        }
    }
    
    else
    {
        #pragma omp parallel sections
        {
            #pragma omp section // cada secao e' enviada para uma thread
            right > initialLeft ? parallelDivideSections(initialLeft, right, array) : 0;

            #pragma omp section
            left < initialRight ? parallelDivideSections(left, initialRight, array) : 0;
        }
    }
}

void parallelQuicksortSections(int *array, int arrayLength)
{
    parallelDivideSections(0, arrayLength - 1, array);
}

// ------------------- FIM DO QUICKSORT PARALELO COM SECOES

// ------------------- QUICKSORT SEQUENCIAL

void sequentialDivide(int initialLeft, int initialRight, int *array)
{
    int left = initialLeft;
    int right = initialRight;
    int pivot = array[ (left + right) / 2 ];
    int leftElement;
    int rightElement;
    
    while (left <= right)
    {
        leftElement = array[left];
        rightElement = array[right];
        
        while (left < initialRight && leftElement < pivot)
        {
            leftElement = array[++left];
        }
        
        while (right > initialLeft && rightElement > pivot)
        {
            rightElement = array[--right];
        }
        
        if (left <= right)
        {
            swap(left++, right--, array);
        }
    }
    
    if (right > initialLeft)
    {
        sequentialDivide(initialLeft, right, array);
    }

    if (left < initialRight)
    {
        sequentialDivide(left, initialRight, array);
    }
}

void sequentialQuicksort(int *array, int arrayLength)
{
    sequentialDivide(0, arrayLength - 1, array);
}

// ------------------- FIM DO QUICKSORT SEQUENCIAL

// ------------------- FIM DOS QUICKSORTS

// ------------------- FUNCOES DE UTILIDADE

// preenchimento do array  -> { 1, 2, 3, ..., arrayLength }
void initCrescentArray(int *array, int arrayLength)
{
    int i;
    
    #pragma omp parallel for
    for (i = 0; i < arrayLength; i++)
    {
        array[i] = i + 1;
    }
}

// preenchimento do array  -> { arrayLength, arrayLength - 1, ..., 2, 1 }
void initDecrescentArray(int *array, int arrayLength)
{
    int i;
    
    #pragma omp parallel for
    for (i = 0; i < arrayLength; i++) 
    {
        array[i] = arrayLength - i;
    }
}

void initRandomArray(int *array, int arrayLength)
{
    int i;
    srand( time(NULL) );
    
    // a paralelizacao desse looping gera problemas
    // na geracao dos numeros aleatorios
    for (i = 0; i < arrayLength; i++)
    {
        array[i] = rand() % arrayLength + 1;
    }
}

void initArray(int *array, const char *mode, int arrayLength)
{
    if (strcmp(mode, "random") == 0)
        initRandomArray(array, arrayLength);
    
    else if (strcmp(mode, "crescent") == 0)
        initCrescentArray(array, arrayLength);
    
    else if (strcmp(mode, "decrescent") == 0)
        initDecrescentArray(array, arrayLength);
}

// checa se a ordenacao do arranjo esta' incorreta
int ordenationIsWrong(int *array, int arrayLength)
{
    int wrong = 0;
    int i;
    
    #pragma omp parallel for reduction(+:wrong)
    for (i = 0; i < arrayLength - 1; i++)
    {
        wrong = ( wrong ? 1 : array[i] > array[i + 1] );
    }
    
    return wrong > 0;
}

void quicksort(int *array, const char *mode, int arrayLength)
{
    if (strcmp(mode, "sequential") == 0)
        sequentialQuicksort(array, arrayLength);
    
    else if (strcmp(mode, "tasks") == 0)
        parallelQuicksortTasks(array, arrayLength);
    
    else if (strcmp(mode, "tasks_and_for") == 0)
        parallelQuicksortFor(array, arrayLength);
    
    else if (strcmp(mode, "sections") == 0)
        parallelQuicksortSections(array, arrayLength);
}

int main()
{
    int array[ARRAY_SIZE];
    
    initArray(array, ARRAY_INIT_MODE, ARRAY_SIZE);
    
    quicksort(array, QUICKSORT_MODE, ARRAY_SIZE);
    
    if (ordenationIsWrong(array, ARRAY_SIZE)) printf("Ordenacao errada\n");
    
    return EXIT_SUCCESS;
}
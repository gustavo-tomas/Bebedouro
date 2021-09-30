#include "../include/student.h"

void start_student()
{
    system("clear");
    printf("\n\n----------- OS ALUNOS PROCURAM ÁGUA -----------\n\n");
    printf("\tOs alunos da UnB procuram água após um dos bebedouros do ICC quebrar. A fila para o outro bebedouro\n");
    printf("aumenta de tal modo que o ICC fica intransitável. Alunos sedentos, então, vão para o Lago Paranoá\n");
    printf("buscar água. Os que não perecem durante a jornada precisam evitar ataques de jacaré para beber a água\n");
    printf("do lago. A jornada desses alunos é descrita a seguir.\n");
    sleep(7);

    printf("\n\tNúmero de alunos: %2d", N_STUDENTS);
    printf("\n\n----------- A JORNADA COMEÇA -----------\n\n");
    sleep(2);

    pthread_t thr_students[N_STUDENTS], thr_fountain, thr_lake;
    unsigned int i, id[N_STUDENTS];

    // Bebedouro
    sem_init(&queue, 0, QUEUE_SIZE);        // Fila de espera do bebedouro
    sem_init(&fountain, 0, 1);              // Bebedouro
    sem_init(&stop_fill, 0, 0);             // Condiçao para o bebedouro ativar
    sem_init(&bottle, 0, 0);                // Condiçao para encher a garrafa

    // Lago Paranoá
    sem_init(&l_queue, 0, L_QUEUE_SIZE);    // Fila de espera do lago
    sem_init(&lake, 0, 1);                  // Lago
    sem_init(&l_stop_fill, 0, 0);           // Condiçao para o bebedouro ativar
    sem_init(&l_bottle, 0, 0);              // Condiçao para encher a garrafa

    for (i = 0; i < N_STUDENTS; i++)
    {
        id[i] = i;
        pthread_create(&thr_students[i], NULL, student, (void *)&id[i]);
    }

    pthread_create(&thr_fountain, NULL, water_fountain, NULL);
    pthread_create(&thr_lake, NULL, lake_fountain, NULL);

    for (i = 0; i < N_STUDENTS; i++)
    {
        pthread_join(thr_students[i], NULL);
    }
}

void *water_fountain(void *v)
{
    while (1)
    {
        sem_wait(&stop_fill); // Fonte fica desativada até alguém ativar
        sleep(TIME_FILL_FNT); // Tempo para encher a garrafa
        printf(BLU "Uma garrafa foi enchida no bebedouro\n" RESET);
        sem_post(&bottle);    // Aluno vai embora com a garrafa cheia
    }
    pthread_exit(0);
}

void *lake_fountain(void *v)
{
    while (1)
    {
        sem_wait(&l_stop_fill); // Lago fica vazio até alguém chegar
        sleep(TIME_FILL_LAKE);  // Tempo para encher a garrafa
        printf(BLU "Uma garrafa foi enchida no lago\n" RESET);
        sem_post(&l_bottle);    // Aluno vai embora com a garrafa cheia
    }
    pthread_exit(0);
}

void *student(void *v)
{
    unsigned int id = *(int *)v;
    sleep(id / 3);
    if (sem_trywait(&queue) == 0)
    {   // Aluno conseguiu entrar na fila
        printf(YLW "Aluno %2u entrou na fila do bebedouro\n" RESET, id);
        sem_wait(&fountain);  // Bloqueia o bebedouro
        sem_post(&queue);     // Libera uma posição na fila
        sem_post(&stop_fill); // Ativa o bebedouro
        sem_wait(&bottle);    // Espera a garrafa encher
        printf(GRN "Aluno %2u encheu a garrafa no bebedouro e foi embora\n" RESET, id);
        sem_post(&fountain);  // Libera o bebedouro
    }
    else
    {   // Fila cheia, vai para o Lago
        printf(WHT "ICC lotado, aluno %2u vai para o lago...\n" RESET, id);
        if (sem_trywait(&l_queue) == 0)
        {   // Aluno conseguiu entrar na fila do lago
            printf(YLW "Aluno %2u entrou na fila do lago\n" RESET, id);
            sem_wait(&lake);        // Bloqueia a margem do lago
            sem_post(&l_queue);     // Libera uma posição na fila
            crocodile(id);          // Testa a sorte
            sem_post(&l_stop_fill); // Mergulha a garrafa na água
            sem_wait(&l_bottle);    // Espera a garrafa encher
            printf(CYN "Aluno %2u encheu a garrafa no lago e foi embora\n" RESET, id);
            sem_post(&lake);        // Libera o lago
        }
        else
        {
            // Aluno não conseguiu entrar na fila do lago, vai para o submundo
            printf(RED "Lago lotado, aluno %2u desidratou\n" RESET, id);
            damned_count++;
            pthread_exit(0);
        }
    }
}

void crocodile(unsigned int id)
{
    if (rand() % 100 >= DANGER)
    {   // O jacaré ataca!
        printf(RED "O aluno %2u foi atacado pelo jacaré!\n" RESET, id);
        damned_count++;     // Aluno vai para o submundo
        sem_post(&lake);    // Libera o lago
        pthread_exit(0);
    }
}
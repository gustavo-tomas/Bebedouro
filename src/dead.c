#include "../include/dead.h"

void start_dead()
{
    if (damned_count <= 0)  // Sem mortos para lutar
    {
        return;
    }

    sleep(2);
    system("clear");
    printf("\n\n----------- OS ALUNOS QUE MORRERAM SE JUNTAM PARA DERROTAR HADES -----------\n\n");
    printf("\tOs %d alunos que morreram se juntam para destronar Hades e escapar do submundo.\n", damned_count);
    printf("A batalha será difícil. Caso um morto seja atingido por Hades, ele será lançado para o Tártaro,\n");
    printf("onde sofrerá uma eternidade de tortura. Porém, caso sejam vitoriosos, podem se banhar no Rio Estige\n");
    printf("e voltar para o mundo dos vivos como semideuses.\n");
    sleep(7);
    
    hades_hp = 27 * damned_count;
    min_h = max(damned_count / 5, 1);
    max_h = min_h * 3;

    printf("\n\tNúmero de mortos: %2d", damned_count);
    printf("\n\tVida de Hades: %2d", hades_hp);
    printf("\n\tDano mínimo de Hades: %2d", min_h);
    printf("\n\tDano máximo de Hades: %2d", max_h);
    printf("\n\n----------- A BATALHA COMEÇA -----------\n\n");
    sleep(2);

    // Inicializa os locks
    pthread_mutex_init(&lock_attack, NULL);
    pthread_mutex_init(&lock_damned, NULL);
    pthread_mutex_init(&lock_turn, NULL);

    pthread_t h, d[damned_count];   // Hades e Mortos

    int i, *id;

    // Cria threads dos mortos
    for (i = 0; i < damned_count; i++)
    {
        id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&d[i], NULL, dead, (void *)(id));
    }

    // Thread de Hades
    id = (int *)malloc(sizeof(int));
    *id = 0;
    pthread_create(&h, NULL, hades, (void *)(id));
    
    for (int i = 0; i < damned_count; i++)
    {
        pthread_join(d[i], NULL);
    }
    pthread_join(h, NULL);
}

void *dead(void *arg)
{
    int i = *((int *)arg);
    while (1)
    {
        sleep(rand() % 2 + 1);              // Tempo para atacar
        pthread_mutex_lock(&lock_turn);     // Turno dos mortos
        pthread_mutex_lock(&lock_damned);   // Bloqueia o contador de ataques
        dead_hit(i);                        // Verifica se morto foi atingido
        attack_count++;
        if (attack_count == 1)
        {
            pthread_mutex_lock(&lock_attack);   // Garante o ataque dos mortos
        }
        pthread_mutex_unlock(&lock_damned);     // Libera o contador
        pthread_mutex_unlock(&lock_turn);       // Troca o turno

        dead_attack(i);                         // Calcula dano e desconta vida de Hades

        pthread_mutex_lock(&lock_damned);
        attack_count--;
        if (attack_count == 0)                  // Todos os mortos atacaram
        {
            pthread_mutex_unlock(&lock_attack); // O ataque agora é de Hades
        }
        pthread_mutex_unlock(&lock_damned);
    }
}

void *hades(void *arg)
{
    int i = *((int *)arg);
    while (1)
    {
        sleep(4);                           // Tempo para preparar o ataque
        pthread_mutex_lock(&lock_turn);     // Hades prepara seu turno
        pthread_mutex_lock(&lock_attack);   // Ataca os mortos
        hades_attack();                     // Calcula número de mortos atingidos
        pthread_mutex_unlock(&lock_attack); // Termina o ataque
        pthread_mutex_unlock(&lock_turn);   // Passa o turno
    }
}

// Dano dos mortos varia de MIN_D a MAX_D
void dead_attack(int i)
{
    sleep(damned_count / (i+1));
    pthread_mutex_lock(&lock_hp);
    int dmg = (rand() % MAX_D) + 1;
    dmg = dmg < MIN_D ? dmg + MIN_D : dmg;
    hades_hp -= dmg;
    printf(GRN "Morto %2d ataca! Dano causado: %2d\n" RESET, i, dmg);
    hades_hit();
    pthread_mutex_unlock(&lock_hp);
}

void dead_hit(int i)
{
    if (dead_hit_count > 0)                 // Verifica se aluno foi atingido
    {
        printf(RED "Morto %2d foi atingido por Hades e caiu no Tártaro!\n" RESET, i);
        dead_hit_count--;                   // Desconta um ataque de Hades
        damned_count--;                     // Desconta um morto atingido
        pthread_mutex_unlock(&lock_damned); // Libera o contador
        pthread_mutex_unlock(&lock_turn);   // Troca o turno
        if (damned_count > 0)
        {
            printf(YLW "A batalha continua! Mortos restantes: %2d\n" RESET, damned_count);
        }
        else
        {
            printf(RED "Derrota! Todos os mortos foram lançados ao Tártaro!\n" RESET);
            exit(0);
        }
        pthread_exit(0);                    // Morto cai no Tártaro
    }
}

// Hades atinge de MIN a MAX alunos
void hades_attack()
{
    sleep(1);
    dead_hit_count = (rand() % max_h) + 1;
    dead_hit_count = dead_hit_count < min_h ? dead_hit_count + min_h : dead_hit_count;
    printf(RED "Hades ataca! Mortos atingidos: %2d\n" RESET, min(dead_hit_count,damned_count));
}

void hades_hit()
{
    if (hades_hp <= 0)
    {
        printf(CYN "Vitória! Os mortos restantes voltam para o mundo dos vivos!\n" RESET);
        exit(0);
    }
    else
    {
        printf(YLW "Hades respira! Vida restante: %2d\n" RESET, hades_hp);
    }
}
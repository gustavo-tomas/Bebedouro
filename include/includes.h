#ifndef INCLUDES_H
#define INCLUDES_H

// Bibliotecas
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Macros
#define max(a, b) (a >= b ? a : b)  // Função que retorna o máximo entre inteiros
#define min(a, b) (a <= b ? a : b)  // Função que retorna o mínimo entre inteiros
#define N_STUDENTS     50           // Número total de alunos
#define QUEUE_SIZE      5           // Tamanho máximo da fila para o bebedouro
#define L_QUEUE_SIZE   10           // Tamanho máximo da fila para o lago
#define DANGER         90           // Chance de ataque do jacaré (100 - N)
#define MIN_D           6           // Mínimo de dano causado por um morto
#define MAX_D          12           // Máximo de dano causado por um morto
#define TIME_FILL_LAKE 1            // Tempo para encher a garrafa no lago  
#define TIME_FILL_FNT  2            // Tempo para encher a garrafa no bebedouro

// Cores
#define RED   "\e[0;31m"
#define GRN   "\e[0;32m"
#define YLW   "\e[0;33m"
#define BLU   "\e[0;34m"
#define CYN   "\e[0;36m"
#define WHT   "\e[0;37m"
#define RESET "\e[0m"

#include "student.h"
#include "dead.h"

#endif
#include "includes.h"

#ifndef STUDENT_H
#define STUDENT_H

// Bebedouro
sem_t queue;     // Posições na fila
sem_t fountain;  // Bebedouro
sem_t stop_fill; // Condição para o bebedouro ativar
sem_t bottle;    // Condição para encher a garrafa

// Lago Paranoá
sem_t l_queue;     // Posição na fila do lago
sem_t lake;        // Bebedouro público
sem_t l_stop_fill; // Condição para chegar na margem do lago
sem_t l_bottle;    // Condição para encher a garrafa

void start_student(void);
void *student(void *v);
void *water_fountain(void *v);
void *lake_fountain(void *v);
void crocodile(unsigned int id);

#endif
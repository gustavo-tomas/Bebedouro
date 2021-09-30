#include "includes.h"

#ifndef DEAD_H
#define DEAD_H

// Submundo
pthread_mutex_t lock_attack; // Ataques são feitos em turnos
pthread_mutex_t lock_damned; // Lock para o contador de alunos
pthread_mutex_t lock_turn;   // Turno entre alunos e Hades
pthread_mutex_t lock_hp;     // Lock para contador de vida

void start_dead();
void *dead(void *arg);
void *hades(void *arg);
void dead_attack(int i);
void dead_hit(int i);
void hades_attack();
void hades_hit();

int damned_count;   // Contador dos alunos que morreram
int attack_count;   // Contador de ataque dos mortos
int hades_hp;       // Pontos de vida do Hades
int dead_hit_count; // Número de mortos atingidos
int min_h;          // Mínimo de mortos atingidos por Hades
int max_h;          // Máximo de mortos atingidos por Hades

#endif
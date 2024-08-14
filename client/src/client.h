#ifndef CLIENT_H_
#define CLIENT_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

#include "utils.h"


t_log* iniciar_logger(void);
t_config* iniciar_config(void);
t_list* leer_consola(t_log*);
void chequeo_lista(t_log*, t_list*);
void paquete(int, t_log*, t_list*);
void terminar_programa(int, t_log*, t_config*);
void destroy_element(void *);

#endif /* CLIENT_H_ */

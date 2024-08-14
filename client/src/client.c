#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;
	t_list* lista_lineas;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	if (logger == NULL){
		abort();
	}
	log_info(logger,"Soy un Log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	// Loggeamos el valor de config

	if (config == NULL) {
    	// ¡No se pudo crear el config!
		log_info(logger,"¡No se pudo crear el config!");
    	// Terminemos el programa
		abort();
	}
	ip = config_get_string_value(config, "IP");
	log_info(logger,"leí la ip = %s del archivo de config",ip);
	puerto = config_get_string_value(config, "PUERTO");
	log_info(logger,"leí el puerto = %s del archivo de config",puerto);
	valor = config_get_string_value(config, "CLAVE");
	log_info(logger,"leí el valor = %s del archivo de config",valor);

	/* ---------------- LEER DE CONSOLA ---------------- */

	lista_lineas = leer_consola(logger);
	chequeo_lista(logger, lista_lineas);
	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto, logger);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor,conexion);
	log_info(logger,"Envié el mensaje %s ",valor);


	// Armamos y enviamos el paquete
	paquete(conexion, logger,lista_lineas);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("../tp0.log", "Cliente", 1, LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("../cliente.config");

	return nuevo_config;
}

t_list* leer_consola(t_log* logger)
{
	char* leido;
	t_list* lista_lineas = list_create();

	// La primera te la dejo de yapa
	leido = readline("> ");
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while(*leido != '\0'/*strcmp(leido, "") != 0*/){ //ambas son correctas
		log_info(logger,"Se ingreso por consola: %s",leido);
		char* copia_leido = strdup(leido);
		list_add(lista_lineas,copia_leido);
		free(leido);
		leido = readline("> ");
	}

	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);

	return lista_lineas;
}

void chequeo_lista(t_log* logger, t_list* lista_lineas){
	char* leido;
	int index = 0;
	while(index < list_size(lista_lineas)){
		leido = list_get(lista_lineas,index);
		if(leido != NULL){
			log_info(logger,"Lei de la lista: %s", leido);
		}else{
			log_info(logger,"Lei de la lista un elemento NULL");
		}
		index++;
	}
}

void paquete(int conexion, t_log* logger, t_list* lista_lineas)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	
	int index = 0;
	while(index < list_size(lista_lineas)){
		leido = list_get(lista_lineas,index);
		if(leido != NULL){
			log_info(logger,"Agrego a paquete: %s", leido);
			agregar_a_paquete(paquete,leido, strlen(leido) + 1);
		}else{
			log_info(logger,"Omito agregar a paquete un elemento NULL");
		}
		index++;
	}

	enviar_paquete(paquete, conexion);
	log_info(logger, "Envie paquete");
	
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);
	list_destroy_and_destroy_elements(lista_lineas, destroy_element);
}

void destroy_element(void* elemento){
	free(elemento);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);

}

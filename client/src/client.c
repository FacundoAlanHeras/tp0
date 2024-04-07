#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	//char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	FILE *archivo;

	archivo = fopen("tp0.log", "a");

	logger = malloc(sizeof(t_log));


	logger->is_active_console = true;
	logger -> detail = LOG_LEVEL_INFO;
	logger ->program_name =malloc(strlen("tp0.log") + 1);
	strcpy(logger ->program_name , "tp0.log");
	logger -> file = archivo; 

	//log_info(logger,"Hola! Soy un log");

	//log_destroy(logger);



	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

		//FILE *archivo_config;

	//archivo_config = fopen("tp0.config", "a");

	//config = config_create("/home/utnso/tp0/client/tp0.config");
	config = config_create("/home/utnso/tp0/client/cliente.config");
	
	

	 if (config == NULL) {
        printf("Error al crear el config, finaliza el programa.\n");
        return 1; 
    }
	

	//config_set_value(config, "CLAVE", "Hola! Soy un log config");

	 if (config_save(config) == -1) {
        printf("Error al guardar el archivo de configuración.\n");
    }

	//log_info(logger,config_get_string_value(config,"CLAVE"));
	log_info(logger, "Valor de la clave 'CLAVE': %s", config_get_string_value(config, "CLAVE"));


	//log_destroy(logger);

	//config_destroy(config);

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	//config_set_value(config, "IP", "10.0.2.15");
	//config_set_value(config, "PUERTO", "4444");
	ip = malloc(sizeof(config_get_string_value(config,"IP")));
	puerto = malloc(sizeof(config_get_string_value(config,"PUERTO")));
	strcpy(ip, config_get_string_value(config,"IP"));
	strcpy(puerto, config_get_string_value(config,"PUERTO"));

	// Loggeamos el valor de config
	//log_info(logger,config_get_string_value(config,"IP"));
	//log_info(logger,config_get_string_value(config,"PUERTO"));

	//log_info(logger,ip);
	//log_info(logger,puerto);

	/* ---------------- LEER DE CONSOLA ---------------- */

	//leer_consola(logger);

	//log_destroy(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(config_get_string_value(config,"CLAVE"),conexion);
	
	// Armamos y enviamos el paquete
	//leer_consola(logger);
	paquete(conexion,logger);

	terminar_programa(conexion, logger, config);
	

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	return nuevo_config;
}

void leer_consola(t_log* logger, t_paquete* paquete)//CAMBIE LOS PARAMETROS, agregue paquete
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");
	
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while(strcmp(leido,"") != 0){
		log_info(logger,leido);
		agregar_a_paquete(paquete,leido, strlen(leido) + 1);
		leido = readline("> ");
	}

	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);
}

void paquete(int conexion, t_log* logger) //CAMBIE LOS PARAMETROS
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;

	paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	leer_consola(logger,paquete);

	enviar_paquete(paquete, conexion);
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */

	  log_destroy(logger);
	  config_destroy(config);
	  liberar_conexion(conexion);

}

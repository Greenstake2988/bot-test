#include <stdio.h>
#include <tgbot/tgbot.h>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;
using namespace std;

//Un flag para poder pasar los datos
//a la funcion anyMessage
bool ORDENANDO_TACOS = false;
//json CLIENTES_JSON;

json copiaClientes();
json copiaCliente(string id_cliente);
void escribirCliente(string id_cliente, json datos);


struct Orden
{	
	//Taco Maiz Asado
	int num_tma = 0;
	//Taco Maiz Con Chicharra
	int num_tmc = 0;
	//Taco Maiz Especial
	int num_tme = 0;
	bool orden_activa = false;
	bool con_chicharra = false;
};

Orden nueva_orden;

int main() {

	//Direccion unica del bot @Green_88bot
	TgBot::Bot bot("1864266042:AAH-1fI-aLsGN78pWbBRmXYiyeOnV9Y86Rg");

	//Checamos que la base de datos existe de lo contrario la creamos.
	ifstream clientes_guardados("clientes.json");
	if(not clientes_guardados.good()){
		ofstream clientes_guardados("clientes.json");
		clientes_guardados << "{}";
	}
	clientes_guardados.close();

	//Aqui dependiendo del '/commando/ la funcion actua
	bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
		//Mensaje de bienvenida
		bot.getApi().sendMessage(message->chat->id, "Que desas hacer: \n"
													"Menu: /menu\n"
													"Ordenar : /ordenar\n"
		);
	});

	//Alta nuevos usuarios o cambiar datos
	bot.getEvents().onCommand("alta", [&bot](TgBot::Message::Ptr message) {
		
		//Sacamos el id_cliente_str de la variable
		string id_cliente_str = to_string(message->from->id);

		//Llamamos al funcion para copiar  la base de datos
		json clientes_guardados = copiasCliente();

		//Emepezamos a Guardar los datos del contacto
		bot.getApi().sendMessage(message->chat->id, "Nombre: " + message->from->firstName);
		clientes_guardados[id_cliente_str]["nombre"] =  message->from->firstName;

		bot.getApi().sendMessage(message->chat->id, "Apellido: " + message->from->lastName);
		clientes_guardados[id_cliente_str]["apellido"] =  message->from->lastName;

		bot.getApi().sendMessage(message->chat->id, "Dame tu direccion: ");

		clientes_guardados[id_cliente_str]["continuacion_alta"] =  true;

		//Guardamos la informacion en la base de datos de clientes.
		escribirClientes(clientes_guardados);

	});

	//Desplegamos Menu
	bot.getEvents().onCommand("menu", [&bot](TgBot::Message::Ptr message) {
		bot.getApi().sendMessage(message->chat->id, "*Despliega el menu...");
	});

	//Confirmar orden
	bot.getEvents().onCommand("confirmar", [&bot](TgBot::Message::Ptr message) {
		//Desplegamos el Menu
		bot.getApi().sendMessage(message->chat->id, "*Despliega el menu...");
	});

	//Pedir Taco Maiz Asado
	bot.getEvents().onCommand("tma", [&bot](TgBot::Message::Ptr message) {

		//Sacamos el id_cliente_str de la variable
		string id_cliente_str = to_string(message->from->id);

		//Llamamos al funcion para copiar  la base de datos
		json clientes_guardados = copiasCliente();	

		//Si no hay orden activa nos salimos
		if(not clientes_guardados[id_cliente_str]["orden"]["activa"].get<bool>()) {
			return;
		}

		//Creamos una variable nueva para facilitar los cambios
		json cliente_orden_tma = clientes_guardados[id_cliente_str]["orden"]["tma"];

		//Si no tiene tacos los creamos y agregamos cada vez 1
		if(cliente_orden_tma.is_null()){
			cliente_orden_tma =  1;
		} else {
			cliente_orden_tma = cliente_orden_tma.get<int>() + 1;
		}

		//Le ponemos el nuevo valor
		clientes_guardados[id_cliente_str]["orden"]["tma"] = cliente_orden_tma;

		//Guardamos la informacion en la base de datos de clientes.
		escribirClientes(clientes_guardados);
		
		//Avisamos al usuario
		bot.getApi().sendMessage(message->from->id, "Se agrego 1 taco de maiz de asado.");

	});

	//Pedir Taco Maiz Con Chicharra
	bot.getEvents().onCommand("tmc", [&bot](TgBot::Message::Ptr message) {

		//Sacamos el id_cliente_str de la variable
		string id_cliente_str = to_string(message->from->id);

		//Llamamos al funcion para copiar  la base de datos
		json clientes_guardados = copiasCliente();		

		//Si no hay orden activa nos salimos
		if(not clientes_guardados[id_cliente_str]["orden"]["activa"].get<bool>()) {
			return;
		}

		//Creamos una variable nueva para facilitar los cambios
		json cliente_orden_tmc = clientes_guardados[id_cliente_str]["orden"]["tmc"];

		//Si no tiene tacos los creamos y agregamos cada vez 1
		if(cliente_orden_tmc.is_null()){
			cliente_orden_tmc =  1;
		} else {
			cliente_orden_tmc = cliente_orden_tmc.get<int>() + 1;
		}

		//Le ponemos el nuevo valor
		clientes_guardados[id_cliente_str]["orden"]["tmc"] = cliente_orden_tmc;

		//Guardamos la informacion en la base de datos de clientes.
		escribirClientes(clientes_guardados);

		//Avisamos al usuario
		bot.getApi().sendMessage(message->from->id, "Se agrego 1 taco de maiz con chicharra.");
	});

	//Pedir Taco Maiz Especial
	bot.getEvents().onCommand("tme", [&bot](TgBot::Message::Ptr message) {

		//Sacamos el id_cliente_str de la variable
		string id_cliente_str = to_string(message->from->id);

		//Llamamos al funcion para copiar  la base de datos
		json clientes_guardados = copiasCliente();		

		//Si no hay orden activa nos salimos
		if(not clientes_guardados[id_cliente_str]["orden"]["activa"].get<bool>()) {
			return;
		}

		//Creamos una variable nueva para facilitar los cambios
		json cliente_orden_tme = clientes_guardados[id_cliente_str]["orden"]["tme"];

		//Si no tiene tacos los creamos y agregamos cada vez 1
		if(cliente_orden_tme.is_null()){
			cliente_orden_tme =  1;
		} else {
			cliente_orden_tme = cliente_orden_tme.get<int>() + 1;
		}

		//Le ponemos el nuevo valor
		clientes_guardados[id_cliente_str]["orden"]["tme"] = cliente_orden_tmc;

		//Guardamos la informacion en la base de datos de clientes.
		escribirClientes(clientes_guardados);

		//Avisamos al usuario
		bot.getApi().sendMessage(message->from->id, "Se agrego 1 taco de maiz especial.");
	});

	//Resumen del Pedido
	bot.getEvents().onCommand("resumen", [&bot](TgBot::Message::Ptr message) {

		//Sacamos el id_cliente_str de la variable
		string id_cliente_str = to_string(message->from->id);

		///Llamamos al funcion para copiar  la base de datos
		json clientes_guardados = copiasCliente();			

		//Si no hay orden activa nos salimos
		if(not clientes_guardados[id_cliente_str]["orden"]["activa"].get<bool>()) {
			return;
		}
		
		//Creamos una variable para manejar mejor
		json cliente_orden = clientes_guardados[id_cliente_str]["orden"];

		//Agregamos texto si tenemso valores en la orden.
		string resumen_mensaje = "";
		if(not cliente_guardado["orden"]["tma"].is_null()){
			resumen_mensaje += to_string(cliente_orden["tma"].get<int>()) + " tacos de maiz de asado\n";
		} 
		if(not cliente_guardado["orden"]["tmc"].is_null()){
			resumen_mensaje += to_string(cliente_orden["tmc"].get<int>()) + " tacos de maiz con chicharra\n";
		}  
		if(not cliente_guardado["orden"]["tme"].is_null()){
			resumen_mensaje += to_string(cliente_orden["tme"].get<int>())+ " tacos de maiz especiales\n";
		}

		//Imprimimos el resumen.  
		bot.getApi().sendMessage(message->chat->id, resumen_mensaje);
	});

	//Nueva orden
	bot.getEvents().onCommand("ordenar", [&bot](TgBot::Message::Ptr message) {

		//Sacamos el id_cliente_str de la variable
		string id_cliente_str = to_string(message->from->id);

		//Llamamos al funcion para copiar a los clientes de la base de datos
		json clientes_guardados = copiaClientes();

		//Copiamos al cliente
		json cliente = clientes_guardados[id_cliente_str];

		//Si el id de cliente no esta dentro de nuestros id's.
		//Le sugerimos /alta
		//De lo contrario lo saludamos
		if (not clientes_guardados.contains(id_cliente_str)){
			bot.getApi().sendMessage(message->from->id, "Eres un cliente nuevo \n"
														"presiona /alta para guardar tu contacto.");
		} else {
			//Checamos que el nombre del cliente no este vacio.
			if(not cliente["nombre"].empty()) {
				bot.getApi().sendMessage(message->from->id, "Hola " + cliente["nombre"].get<string>() + " que deseas ordenar:\n\n"+
												            "Maiz                            Precio\n"
															"Tacos de asado          $13  /tma\n"+
															"Tacos con chicharra  $14  /tmc\n"+
															"Especiales                   $15  /tme\n\n"
															"Presiona /resumen para tener el resumen de tu pedido."
															);
			}

			//Activamos la orden
			cliente["orden"]["activa"]= true;

			//Actualizamos el valor en la base de datos
			clientes_guardados[id_cliente_str] = cliente;

			//Guardamos la informacion en la base de datos de clientes.
			escribirClientes(clientes_guardados);
		}
	});


	//Por aqui pasan todos los mensajes
	bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {

		//Sacamos el id_cliente_str de la variable
		string id_cliente_str = to_string(message->from->id);

		//Llamamos al funcion para copiar a los clientes de la base de datos
		json clientes_guardados = copiaClientes();

		//Copiamos al cliente
		json cliente = clientes_guardados[id_cliente_str];

		//Continuacion del /alta
		if(not cliente["continuacion_alta"].is_null() && cliente["continuacion_alta"].get<bool>() ) {
			bot.getApi().sendMessage(message->chat->id, "Direccion: " + message->text);
			cliente["direccion"] = message->text;

			//Cerramos el Alta
			cliente["continuacion_alta"] = false;
			//Creamos la bandera orden activa
			cliente["orden"]["activa"]= false;
			
			//Actualizamos el valor en la base de datos
			clientes_guardados[id_cliente_str] = cliente;

			//Guardamos la informacion en la base de datos de clientes.
			escribirClientes(clientes_guardados);

			//Avisamos al usuario.
			bot.getApi().sendMessage(message->chat->id, "Tus datos han sido guardados:");
		}


		//Ejemplo de comparador de texto
		if (StringTools::startsWith(message->text, "/start")) {
			return;
		}
	});

	try {
		//Imprimos el nombre del bot en la consola
		printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
		TgBot::TgLongPoll longPoll(bot);

		//Este esl Bucle infinito que espera los eventos
		// y actua de acuero a ellos
		while (true) {
			printf("Long poll started\n");
		  	longPoll.start();
		}
	} catch (TgBot::TgException& e) {
		printf("error: %s\n", e.what());
	}

	return 0;

}


json copiaClientes(){
	//Abrimos el archivo clientes modo lectura
	ifstream open_file("clientes.json");
	json copiaClientes;
	open_file >> copiaClientes;
	open_file.close();
	return copiaClientes;
}

void escribirClientes( json clientes){
	//Abrimos el archivo clientes modo escritura
	ofstream clientes_guardados("clientes.json");
	clientes_guardados << clientes;
	clientes_guardados.close();
}


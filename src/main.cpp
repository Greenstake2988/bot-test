#include <stdio.h>
#include <tgbot/tgbot.h>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

//Un flag para poder pasar los datos
//a la funcion anyMessage
bool CONTINUACION_ALTA = false;
bool NUEVA_ORDEN = false;
bool ORDENANDO_TACOS = false;
json CLIENTES_JSON;


json copiaClientes();
void escribirClientes(json datos);

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
	std::ifstream clientes_guardados("clientes.json");
	if(not clientes_guardados.good()){
		std::ofstream clientes_guardados("clientes.json");
		clientes_guardados << "[]";
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

	bot.getEvents().onCommand("alta", [&bot](TgBot::Message::Ptr message) {
	
		CONTINUACION_ALTA = true;
		//Emepezamos a Guardar los datos del contacto
		bot.getApi().sendMessage(message->chat->id, "Nombre: " + message->from->firstName);
		CLIENTES_JSON[std::to_string(message->from->id)]["nombre"] =  message->from->firstName;
		bot.getApi().sendMessage(message->chat->id, "Apellido: " + message->from->lastName);
		CLIENTES_JSON[std::to_string(message->from->id)]["apellido"] =  message->from->lastName;
		bot.getApi().sendMessage(message->chat->id, "Dame tu direccion: ");

		//Guardamos la informacion en la base de datos de clientes.
		escribirClientes(CLIENTES_JSON);

	});

	bot.getEvents().onCommand("menu", [&bot](TgBot::Message::Ptr message) {
		//Desplegamos el Menu
		bot.getApi().sendMessage(message->chat->id, "*Despliega el menu...");
	});

	//Pedir Taco Maiz Asado
	bot.getEvents().onCommand("tma", [&bot](TgBot::Message::Ptr message) {

		//Sacamos el id_cliente_str de la variable
		std::string id_cliente_str = std::to_string(message->from->id);

		//Llamamos al funcion para copiar la base de datos de clientes
		json clientes_guardados = copiaClientes();

		if(clientes_guardados[id_cliente_str]["orden"]["tma"].is_null()){
			CLIENTES_JSON[id_cliente_str]["orden"]["tma"] =  1;
		} else {
			CLIENTES_JSON[id_cliente_str]["orden"]["tma"] = clientes_guardados[id_cliente_str]["orden"]["tma"].get<int>() + 1;
		}

		//Guardamos la informacion en la base de datos de clientes.
		escribirClientes(CLIENTES_JSON);

		bot.getApi().sendMessage(message->from->id, "Se agrego 1 taco de maiz de asado.");

	});

	//Pedir Taco Maiz Con Chicharra
	bot.getEvents().onCommand("tmc", [&bot](TgBot::Message::Ptr message) {
		bot.getApi().sendMessage(message->chat->id, "Se agrego 1 taco de maiz con chicharra.");
		nueva_orden.num_tmc += 1;
	});

	//Pedir Taco Maiz Especial
	bot.getEvents().onCommand("tme", [&bot](TgBot::Message::Ptr message) {
		bot.getApi().sendMessage(message->chat->id, "Se agrego 1 taco de maiz especial.");
		nueva_orden.num_tme += 1;
	});

	//Resumen del Pedido
	bot.getEvents().onCommand("resumen", [&bot](TgBot::Message::Ptr message) {
		if(nueva_orden.num_tma) bot.getApi().sendMessage(message->chat->id, std::to_string(nueva_orden.num_tma) + " Tacos de maiz de asado");
		if(nueva_orden.num_tmc) bot.getApi().sendMessage(message->chat->id, std::to_string(nueva_orden.num_tmc) + " Tacos de maiz con chicharra");
		if(nueva_orden.num_tme) bot.getApi().sendMessage(message->chat->id, std::to_string(nueva_orden.num_tme) + " Tacos de maiz especiales");
	});

	bot.getEvents().onCommand("ordenar", [&bot](TgBot::Message::Ptr message) {

		//Sacamos el id_cliente_str de la variable
		std::string id_cliente_str = std::to_string(message->from->id);

		//Llamamos al funcion para copiar la base de datos de clientes
		json clientes_guardados = copiaClientes();

		//Si el id de cliente no esta dentro de nuestros id's.
		//Le sugerimos /alta
		//De lo contrario lo saludamos
		if (not clientes_guardados.contains(id_cliente_str)){
			bot.getApi().sendMessage(message->from->id, "Eres un cliente nuevo \n"
														"presiona /alta para guardar tu contacto.");
		} else {
			//Checamos que el nombre del cliente no este vacio.
			if(not clientes_guardados[id_cliente_str]["nombre"].empty()) {
				bot.getApi().sendMessage(message->from->id, "Hola " + clientes_guardados[id_cliente_str]["nombre"].get<std::string>() + " que deseas ordenar:\n"+
												            "Maiz                              Precio\n"
															"Tacos de asado         $13 /tma\n"+
															"Tacos con chicharra $14 /tmc\n"+
															"Especiales                  $15 /tme\n"
															"Presiona /resumen para tener el resumen de tu pedido."
															);
			}

			NUEVA_ORDEN = true;
		}
	});


	//Por aqui pasan todos los mensajes
	bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {

		//Continuacion del /alta
		if(CONTINUACION_ALTA) {
			bot.getApi().sendMessage(message->chat->id, "Direccion: " + message->text);
			CLIENTES_JSON[std::to_string(message->from->id)]["direccion"] = message->text;

			//Guardamos la informacion en la base de datos de clientes.
			escribirClientes(CLIENTES_JSON);

			bot.getApi().sendMessage(message->chat->id, "Tus datos han sido guardados:");
			CONTINUACION_ALTA = false;
		}

		if(NUEVA_ORDEN) {
			if(ORDENANDO_TACOS) {
		
			}
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
	//Abrimos el archivo clientes.
	std::ifstream open_file("clientes.json");
	json copiaClientes;
	open_file >> copiaClientes;
	open_file.close();
	return copiaClientes;
}

void escribirClientes(json datos){
	std::ofstream clientes_guardados("clientes.json");
	clientes_guardados << datos;
	clientes_guardados.close();
}


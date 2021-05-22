#include <stdio.h>
#include <tgbot/tgbot.h>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

//Un flag para poder pasar los datos
//a la funcion anyMessage
bool NUEVA_ALTA = false;
bool NUEVA_ORDEN = false;
bool ORDENANDO_TACOS = false;
bool ELIGIENDO_CHICHARRA = false;
json JSON_FILE;



struct Tacos
{
	int num_tacos = 0;
	bool sin_cebolla = false;
	bool con_chicharra = false;
	bool especial = false;
};

Tacos NUEVOS_TACOS;

int main() {

	//Direccion unica del bot @Green_88bot
	TgBot::Bot bot("1864266042:AAH-1fI-aLsGN78pWbBRmXYiyeOnV9Y86Rg");

	//Checamos que la base de datos existe de lo contrario la creamos.
	std::ifstream clientes_guardados("clientes.json");
	if(not clientes_guardados.good()){
		std::ofstream clientes_guardados("clientes.json");
		clientes_guardados << "[]";
	}

	//Aqui dependiendo del '/commando/ la funcion actua
	bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
		//Mensaje de bienvenida
		bot.getApi().sendMessage(message->chat->id, "Que desas hacer: \n"
													"Menu: /menu\n"
													"Ordenar : /ordenar\n"
		);
	});

	bot.getEvents().onCommand("alta", [&bot](TgBot::Message::Ptr message) {
		
		NUEVA_ALTA = true;
		//Emepezamos a Guardar los datos del contacto
		bot.getApi().sendMessage(message->chat->id, "Nombre: " + message->from->firstName);
		JSON_FILE[std::to_string(message->from->id)]["nombre"] =  message->from->firstName;
		bot.getApi().sendMessage(message->chat->id, "Apellido: " + message->from->lastName);
		JSON_FILE[std::to_string(message->from->id)]["apellido"] =  message->from->lastName;
		bot.getApi().sendMessage(message->chat->id, "Dame tu direccion: ");

	});

	bot.getEvents().onCommand("menu", [&bot](TgBot::Message::Ptr message) {
		//Desplegamos el Menu
		bot.getApi().sendMessage(message->chat->id, "*Despliega el menu...");
	});

	bot.getEvents().onCommand("ordenar", [&bot](TgBot::Message::Ptr message) {
		//Abrimos el archivo clientes_guardados
		std::string id_cliente = std::to_string(message->from->id);
		std::ifstream open_file("clientes.json");
		json clientes_guardados;
		open_file >> clientes_guardados;

		//Si el id de cliente no esta dentro de nuestros id's.
		//Le sugerimos /alta
		//De lo contrario lo saludamos
		if (not clientes_guardados.contains(id_cliente)){
			bot.getApi().sendMessage(message->chat->id, "Eres un cliente nuevo \n"
														"presiona /alta para guardar tu contacto.");
		} else {
			//Checamos que el nombre del cliente no este vacio.
			if(not clientes_guardados[std::to_string(message->from->id)]["nombre"].empty()) {
				bot.getApi().sendMessage(message->chat->id, "Hola " + clientes_guardados[std::to_string(message->from->id)]["nombre"].get<std::string>() + " que deseas ordenar\n"
												            "/1 Tacos\n"
															"/2 Tortas\n"
															"/3 Orden\n"
															);
			}

			NUEVA_ORDEN = true;
		}
	});

	bot.getEvents().onCommand("1", [&bot](TgBot::Message::Ptr message) {
		if(NUEVA_ORDEN) {
			bot.getApi().sendMessage(message->chat->id, "Cuantos tacos quieres");
			ORDENANDO_TACOS = true;
		}
	});


	//Por aqui pasan todos los mensajes
	bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {

		if(NUEVA_ALTA) {
			bot.getApi().sendMessage(message->chat->id, "Direccion: " + message->text);
			JSON_FILE[std::to_string(message->from->id)]["direccion"] = message->text;
			std::ofstream file("clientes.json");
			file << JSON_FILE;
			bot.getApi().sendMessage(message->chat->id, "Tus datos han sido guardados:");
			NUEVA_ALTA = false;
		}

		if(NUEVA_ORDEN) {
			if(ORDENANDO_TACOS) {
				if(ELIGIENDO_CHICHARRA){
					if(message->text == "si"){
						NUEVOS_TACOS.con_chicharra = true;
						bot.getApi().sendMessage(message->chat->id, "Pediste " + std::to_string(NUEVOS_TACOS.num_tacos) + " con chicharra.");
					} else {
						bot.getApi().sendMessage(message->chat->id, "Pediste " + std::to_string(NUEVOS_TACOS.num_tacos) + " sin chicharra.");
					}
					ELIGIENDO_CHICHARRA = false;
					ORDENANDO_TACOS = false;
					NUEVA_ORDEN = false;
				}
	
				//NUEVOS_TACOS.num_tacos = std::stoi(message->text);
				bot.getApi().sendMessage(message->chat->id, "Con Chicharra? ");
				ELIGIENDO_CHICHARRA = true;
		
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





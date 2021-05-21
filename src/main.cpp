#include <stdio.h>
#include <tgbot/tgbot.h>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

bool NUEVA_ALTA = false;
bool usuario_nuevo = true;
bool orden_nueva = false;
json JSON_FILE;

int main() {
	TgBot::Bot bot("1864266042:AAH-1fI-aLsGN78pWbBRmXYiyeOnV9Y86Rg");

	//Aqui dependiendo del '/commando/ la funcion actua
	bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
		bot.getApi().sendMessage(message->chat->id, "Hola!");
	});

	bot.getEvents().onCommand("alta", [&bot](TgBot::Message::Ptr message) {

		NUEVA_ALTA = true;

		JSON_FILE["id"] =  std::to_string(message->from->id);
		bot.getApi().sendMessage(message->chat->id, "Nombre: " + message->from->firstName);
		JSON_FILE["nombre"] =  message->from->firstName;
		bot.getApi().sendMessage(message->chat->id, "Apellido: " + message->from->lastName);
		JSON_FILE["apellido"] =  message->from->lastName;
		bot.getApi().sendMessage(message->chat->id, "Dame tu direccion: ");
	});

	bot.getEvents().onCommand("menu", [&bot](TgBot::Message::Ptr message) {
		bot.getApi().sendMessage(message->chat->id, "*Despliega el menu...");
	});

	bot.getEvents().onCommand("ordenar", [&bot](TgBot::Message::Ptr message) {
		if (usuario_nuevo) {
			bot.getApi().sendMessage(message->chat->id, "Cual es tu nombre.");
			JSON_FILE["foo"] = "bar";
			std::ofstream file("key.json");
			file << JSON_FILE;
		} else {
			bot.getApi().sendMessage(message->chat->id, "Hola *su nombre*.");
		}

	});

	bot.getEvents().onCommand("comida", [&bot](TgBot::Message::Ptr message) {
		bot.getApi().sendMessage(message->chat->id, "Que Quieres Comer?");
	});


	bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {

		if(NUEVA_ALTA) {
			bot.getApi().sendMessage(message->chat->id, "Direccion: " + message->text);
			JSON_FILE["direccion"] = message->text;
			std::ofstream file("clientes.json");
			file << JSON_FILE;
			bot.getApi().sendMessage(message->chat->id, "Tus datos han sido guardados:");
			NUEVA_ALTA = false;
		}

		//printf("Id usuario: %s\n", (std::to_string(message->from->id).c_str()));
		//Si el mensaje empieza con /start no devuvle el mensaje
		//se sale de la funcion
		if (StringTools::startsWith(message->text, "/start")) {
			return;
		}
		//Esta es la funcion que devuvle el mensaje al chat
		//bot.getApi().sendMessage(message->chat->id, "Tu mensaje es: " + message->text);
		//std::to_string id = message->chat->id
		//id_char = id.c_string()
		//bot.getApi().sendMessage(message->chat->id,  "tu id es: " + std::to_string(message->chat->id));
	});

	try {
		//Esta orden se efecuta una vez
		printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
		//printf("User username: %s
		TgBot::TgLongPoll longPoll(bot);
		//Este esl Bucle infinito que espera los eventos
		// y actua de acuero a ellos
		//Mensaje de bienvenida
		bot.getApi().sendMessage(message->chat->id, "Que desas hacer: "
													"Menu: /menu"
													"Ordenar : /ordenar"
													);

		while (true) {
			printf("Long poll started\n");
			//printf();
		  	longPoll.start();
		}
	} catch (TgBot::TgException& e) {
		printf("error: %s\n", e.what());
	}

	return 0;

}





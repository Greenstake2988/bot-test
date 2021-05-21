#include <stdio.h>
#include <tgbot/tgbot.h>
#include <string>

        int main() {
	TgBot::Bot bot("1864266042:AAH-1fI-aLsGN78pWbBRmXYiyeOnV9Y86Rg");

	//Aqui dependiendo del '/commando/ la funcion actua
	bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
	bot.getApi().sendMessage(message->chat->id, "Hola!");
	});

	bot.getEvents().onCommand("menu", [&bot](TgBot::Message::Ptr message) {
	bot.getApi().sendMessage(message->chat->id, "*Despliega el menu...");
	});

	bot.getEvents().onCommand("comida", [&bot](TgBot::Message::Ptr message) {
		bot.getApi().sendMessage(message->chat->id, "Que Quieres Comer?");
	});


	bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {

		printf("Id usuario: %s\n", (std::to_string(message->from->id).c_str()));
		//Si el mensaje empieza con /start no devuvle el mensaje
		//se sale de la funcion
		if (StringTools::startsWith(message->text, "/start")) {
			return;
		}
		//Esta es la funcion que devuvle el mensaje al chat
		//bot.getApi().sendMessage(message->chat->id, "Tu mensaje es: " + message->text);
		//std::to_string id = message->chat->id
		//id_char = id.c_string()
		bot.getApi().sendMessage(message->chat->id,  "tu id es: " + std::to_string(message->chat->id));
	});

	try {
		//Esta orden se efecuta una vez
		printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
		//printf("User username: %s
		TgBot::TgLongPoll longPoll(bot);
		//Este esl Bucle infinito que espera los eventos
		// y actua de acuero a ellos
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





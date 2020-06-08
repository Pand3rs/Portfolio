#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Twitch_core.h"
#include <iostream>
#include <fstream>
#include "Stack.h"
using namespace std;

int main(int argc, char **argv)
{
	//initialize network
	Twitch::startup();

	const char *username = "pan_ders";
	//get this token from https://twitchapps.com/tmi/ and keep it safe
	//remove the oauth: part when you copy paste
	const char *token = "insert token here";  //token deleted for privacy

	//init connection data
	Twitch::Connection connection;
	Twitch::init(&connection, username, token);

	//make TCP connection to twitch and login
	Twitch::connect(&connection);

	//join a channel
	//pass channel name
	printf("connecting to channels\n");
	Twitch::join_Channel(&connection, "pokimane");
	

	//initialize message table
	//will contaain incoming message list from all connected channels
	Twitch::Message::Table incoming;
	Twitch::Message::init(&incoming);

	printf("chat log\n");

	Stack::Stack a;
	Stack::init(&a, 15);
	for (;;)
	{
		unsigned int timestamp = clock();
		incoming.n_count = 0;
		//collect all messages from all channels
		Twitch::communicate(&incoming, &connection, timestamp);
		if (connection.active == false)
		{
			printf("connection was closed!\n");
			break;
		}

		//print received messages from all channels
		for (int i = 0; i < incoming.n_count; i++)
		{
			printf("%s@%s|(%.2f)-> %s\n", incoming.username[i], incoming.channel[i], (double)timestamp / CLOCKS_PER_SEC, incoming.message[i]);
			Stack::push(&a, incoming.username[i]);
		}
	}

	getchar();
}
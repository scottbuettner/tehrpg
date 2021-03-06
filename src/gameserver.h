#pragma once

#include <SFML/System.hpp>
#include <map>

#include "gameserver_interface.h"
#include "typedefs.h"
#include "commandparser.h"

namespace teh
{
	class Application;
	class MySQL;
	
	namespace RPG
	{
		class Engine;
	}
	
	class GameClient
		: public GameConnectionInterface
	{
		public:
			enum State
			{
				WelcomeState,
				LoginState,
				LoggedInState,
				PlayingState,
				ClosingState,
			};
			
			const static unsigned short int RootPermissions = 0b100000000000000;
			const static unsigned short int UserPermissions = 0b1;
			const static unsigned short int ServerAdminPermissions = 0b10;
			const static unsigned short int ContentAdminPermissions = 0b100;
						
			GameClient(GameConnectionInterface* conn);
			
			void generate_challenge();
			
			bool has_line();
			std::string read_line();
			
			void write_line(const std::string& line);
		
			void close();
			
			GameClient::State state();
			void state(const GameClient::State& s);
			
			unsigned int userid();
			void userid(const unsigned int& id);
			
			unsigned short int permissions();
			void permissions(const unsigned short int& p);
			
			std::string challenge();
		private:
			unsigned int _userid;
			GameClient::State _state;
			unsigned short int _permissions;
			GameConnectionInterface* _conn;
			std::string _challenge;
	};

	class GameServer
		: public GameServerInterface
	{
		public:
			GameServer(Application* parent);
			~GameServer();
			
			clientid add_connection(GameConnectionInterface* conn);
		
			//void remove_client(GameClientInterface* client);
			void remove_client(const clientid& id);
			
			//void close_client(GameClientInterface* client);
			void close_client(const clientid& id);
		
			void init();
		
			void start();
			void finish();
		
			clientid find_clientid(GameClient* client);
			clientid find_from_userid(const unsigned int& id);
			GameClient* get_client(const clientid& id);
		
			MySQL* sql();
			RPG::Engine* rpg();
		
			bool kill(const clientid& id, const std::string& killer);
		
			void shutdown(const std::string username);
			
			void update_permissions(GameClient* client);
		private:
			//methods
			void process_line(const clientid& id, const std::string& line);
			std::string greeting(const clientid& id);
		
			//members
			std::map<clientid, GameClient*> _clients;
		
			clientid _next;
			bool _done;
			sf::Mutex _donemutex;
			sf::Mutex _clientsmutex;
		
			Application* _parent;
	};
}
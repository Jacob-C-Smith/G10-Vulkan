#include <G10/GXServer.h>

int create_server ( GXServer_t **pp_server )
{

	// Argument check
	{
		#ifndef NDEBUG
			if (pp_server == (void *) 0)
				goto no_server;
		#endif
	}

	// Initialized data
	GXServer_t* p_server = calloc(1, sizeof(GXServer_t));

	// Error handling
	if ( p_server == (void *) 0 )
		goto no_mem;

	// Return the memory pointer
	*pp_server = p_server;

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_server:
				#ifndef NDEBUG
					g_print_error("[G10] [Server] Null pointer provided for parameter \"pp_server\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int load_server ( GXServer_t **pp_server, char *path )
{

	// Argument check
	{
		#ifndef NDEBUG
			if (pp_server == (void *) 0)
				goto no_server;
			if (path == (void*)0)
				goto no_path;
		#endif
	}

	// Initialized data
	size_t  len        = g_load_file(path, 0, true);
	char   *text = calloc(len+1, sizeof( char *));

	// Error checking
	if (text == (void *)0)
		goto no_mem;

	// Load the file
	g_load_file(path, text, true);

	// TODO: Fix
	// Parse the JSON into a GXServer
	//load_server_as_json(pp_server, text);

	// Free the file text
	free(text);

	return 0;

	// Error handling
	{

		// Argument errors
		{
			no_server:
				#ifndef NDEBUG
					g_print_error("[G10] [Server] Null pointer provided for parameter \"pp_server\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			no_path:
				#ifndef NDEBUG
					g_print_error("[G10] [Server] Null pointer provided for parameter \"path\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int load_server_as_json ( GXServer_t **pp_server, char *text )
{

	// Argument check
	{
		#ifndef NDEBUG
			if (pp_server == (void *)0)
				goto no_server;
		#endif
	}
	/*
	// Constant data
	const size_t MAX_RETRIES = 32;

	// Initialized data
	GXServer_t  *server      = 0;
	char        *name        = 0,
		        *host        = 0,
	            *port        = 0,
		        *max_players = 0,
		        *password    = 0,
		        *tick_rate   = 0;
	dict        *json        = 0;
	size_t       retries     = 0;

	// Parse JSON
	{

		// Initialized data
		JSONToken_t* token = 0;

		// Parse the JSON text into a dictionary
		//arse_json(text, len, &json);

		// Get the name
		token       = (JSONToken_t *) dict_get(json, "name");
		name        = JSON_VALUE(token, JSON_VALUE_STRING);

		// Get the host
		token       = (JSONToken_t *) dict_get(json, "host");
		host        = JSON_VALUE(token, JSON_VALUE_STRING);

		// Get the port
		token       = (JSONToken_t *) dict_get(json, "port");
		port        = JSON_VALUE(token, JSONprimative);

		// Get the player max
		token       = (JSONToken_t *) dict_get(json, "max players");
		max_players = JSON_VALUE(token, JSONprimative);

		// Get the password
		token       = (JSONToken_t *) dict_get(json, "password");
		password    = JSON_VALUE(token, JSON_VALUE_STRING);

		// Get the tick rate
		token       = (JSONToken_t *) dict_get(json, "tick rate");
		tick_rate   = JSON_VALUE(token, JSONprimative);
	}

	// Create the server
	{

		// Allocate server struct memory
		create_server(pp_server);

		// Return
		server = *pp_server;
	}

	// Copy the name
	{

		// Initialized data
		size_t name_len = strlen(name);

		// Allocate memory for the name
		server->name = calloc(name_len + 1, sizeof(char));

		// Error check
		{
			#ifndef NDEBUG
				if ( server->name == (void *) 0 )
					goto no_mem;
			#endif
		}

		// Copy the name into the allocated memory
		strncpy(server->name, name, name_len);
	}

	// Set max players
	if (max_players)
		server->max_clients = atoi(max_players);

	retry_connection:

	// Try to resolve the host
	if (SDLNet_ResolveHost(&server->ip, NULL, atoi(port)))
	{

		if (++retries == MAX_RETRIES + 1)
			return 0;

		// Print an error
		g_print_error("[G10] [Networking] Failed to resolve \"%s\":%hu. Attempting reconnect %d/%d\r", host, port, (int)retries, MAX_RETRIES);

		// Small delay
		SDL_Delay(100);

		// Try again
		goto retry_connection;
	}
	*/

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_server:
				#ifndef NDEBUG
					g_print_error("[G10] [Server] Null pointer provided for parameter \"pp_server\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

		}
	}
};

int start_server ( void )
{

	// Initialized data
	GXInstance_t *p_instance = g_get_active_instance();
	GXServer_t   *p_server = p_instance->networking.server;

	// Open a TCP socket
	p_server->sock = SDLNet_TCP_Open(&p_server->ip);

	// Error handling
	if ( !p_server->sock )
		g_print_error("[SDLNet Error]: %s\n", SDLNet_GetError());

	// TODO: Correct return
	return 0;
}

int process_command ( GXClient_t *client, GXCommand_t *p_command )
{

	// Initialized data
	GXInstance_t* p_instance = g_get_active_instance();

	// Server processing
	if (p_instance->networking.server)
	{
		GXServer_t* server = p_instance->networking.server;

		switch (p_command->type)
		{

		case chat:
			printf("[Chat]\"%s\" says \"%s\"\n", client->name, p_command->chat.chat);
			for (size_t i = 0; i < server->client_list_size; i++)
			{
				GXClient_t *c = server->client_list[i];

				// Construct a chat command for each client
			}
			break;
		}
	}

	// Client processing
	else if (p_instance->networking.client)
	{
		switch (p_command->type)
		{
			case actor_initialize:
			{
				u16         i     = p_command->actor_initialize.index;
				vec3        l     = p_command->actor_initialize.location;
				quaternion  r     = p_command->actor_initialize.quaternion;
				vec3        s     = p_command->actor_initialize.scale;

				GXEntity_t *actor = get_entity(p_instance->context.scene, p_command->actor_initialize.name);

				// Error checking
				{
					#ifndef NDEBUG
						if (actor == (void*)0)
							;// TODO: Error handling
					#endif
				}

				// Add the actor to the client actor list
				client->actors[i] = actor;

				// Set the actors location, rotation, and scale
				{
					actor->transform->location = l;
					actor->transform->rotation = r;
					actor->transform->scale    = s;
				}
			}
			break;

			case actor_displace_rotate:
			{
				u8          i     = p_command->actor_displace_rotate.index >> 8;
				vec3        l     = p_command->actor_displace_rotate.location;
				quaternion  r     = p_command->actor_displace_rotate.quaternion;
				vec3        v     = p_command->actor_displace_rotate.velocity;

				GXEntity_t *actor = client->actors[i];

				// Error checking
				{
					#ifndef NDEBUG
						if (actor == (void*)0)
							;// TODO: Error handling
					#endif
				}


				// Set the actors location, rotation, and scale
				{
					actor->transform->location = l;
					actor->transform->rotation = r;
					actor->rigidbody->velocity = v;
				}

			}
			break;

			case actor_detach:
			{
				u16 i = p_command->actor_detach.index;

				client->actors[i] = 0x0;
			}

			case chat:
				{
					printf("[Chat]\"%s\" says \"%s\"\n", client->name, p_command->chat.chat);
				}
				break;
		}
	}

	// Deallocate the command
	destroy_command(&p_command);

	return 0;

}

int server_recv ( GXClient_t *client )
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( client == (void *) 0 ) goto no_client;
		#endif
	}
	if (client->recv_data == 0)
	{
		client->recv_data = calloc(4096, sizeof(u8));
		return 1;
	}

	// TODO: Error handling
	client->recv_len = SDLNet_TCP_Recv(client->socket, client->recv_data, 4096);

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_client:
				#ifndef NDEBUG
					g_print_error("[G10] [Server] Null pointer provided for parameter \"client\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int server_send ( GXClient_t *client )
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( client == (void *) 0 ) goto no_client;
		#endif
	}

	// TODO: Error handling
	SDLNet_TCP_Send(client->socket, client->send_data, (int) client->send_len);

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_client:
				#ifndef NDEBUG
					g_print_error("[G10] [Server] Null pointer provided for parameter \"client\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int server_parse ( GXClient_t *client )
{

	// Argument errors
	{
		#ifndef NDEBUG
			if (client == (void *) 0 )
				goto no_client;
		#endif
	}

	// Initialized data
	GXCommand_t *i_command = 0;
	size_t       i         = 0;

	// Iterate over data
	while (i <= client->recv_len && queue_empty(client->recv_queue))
	{

		// Parse data into a command
		i += command_from_data(&i_command, &client->recv_data[i]);

		// Add the parsed command into the recv queue
		queue_enqueue(client->recv_queue, i_command);

	}

	return 1;

	// Error handling
	{

		// Argument errors
		{

			// TODO:
			no_client:

				return 0;
		}
	}
}

int server_serialize ( GXClient_t *client )
{

	GXCommand_t *command = 0;
	size_t       i       = 0;

	if (queue_empty(client->send_queue))
	{
		return 1;
	}

	while (i < 4096 && !queue_empty(client->send_queue))
	{

		queue_dequeue(client->send_queue, &command);
		u8* z = client->send_data + i;
		i += data_from_command(&z, command);
	}

	client->send_len = i;

	return 1;
}

int server_process ( GXClient_t *client )
{
	GXInstance_t *p_instance = g_get_active_instance();

	if (!queue_empty(client->recv_queue))
		while (!queue_empty(client->recv_queue))
		{
			GXCommand_t* cmd = 0;
			 queue_dequeue(client->recv_queue, &cmd);

			process_command(client, cmd);
		}

	// TODO: Fix
	//queue_destroy(client->recv_queue);
	//queue_construct(&client->recv_queue, 64);

	if ( p_instance->networking.server )
		for (size_t i = 0; i < client->actor_count; i++)
		{
			GXCommand_t *adr   = calloc(1, sizeof(GXCommand_t));
			GXEntity_t  *actor = client->actors[i];

			{
				adr->type                             = actor_displace_rotate;

				adr->actor_displace_rotate.index      = (u16)i;
				adr->actor_displace_rotate.location   = actor->transform->location;
				adr->actor_displace_rotate.quaternion = actor->transform->rotation;
				adr->actor_displace_rotate.velocity   = actor->rigidbody->velocity;
			}

			queue_enqueue(client->send_queue, adr);
		}

	if (queue_empty(client->send_queue))
	{
		GXCommand_t* adr = calloc(1, sizeof(GXCommand_t));

		queue_enqueue(p_instance->networking.client->send_queue, adr);
	}

	return 0;
}

int server_wait ( GXInstance_t *p_instance )
{

	// Argument check
	{
		#ifndef NDEBUG
			if (p_instance == (void *)0)
				goto no_instance;
		#endif
	}

	while (true)
	{

		// Initialized data
		GXServer_t  *server          = p_instance->networking.server;
		TCPsocket    sock            = SDLNet_TCP_Accept(server->sock);
		GXClient_t  *client          = 0;
		GXThread_t  *server_thread   = 0;
		GXCommand_t *connect_command = 0;
		size_t       client_name_len = 0;

		// Error checking
		{
			#ifndef NDEBUG
				if (sock == NULL)
				{
					goto no_socket;
				}
			#endif
		}

		g_print_log("[SDLNet] New TCP connection accepted\n");

		//Resize client list
		if (server->client_list == NULL)
		{
			server->client_list = malloc(sizeof(GXClient_t*));
			server->client_list_size = 1;
		}
		else {
			++server->client_list_size;
			server->client_list = realloc(server->client_list, sizeof(GXClient_t*) * server->client_list_size);
		}

		// Allocate a client
		create_client(&client);

		// Set the clients socket
		client->socket = sock;

		// Read data from the socket
		SDLNet_TCP_Recv(client->socket, client->recv_data, 4096);

		// Parse the data into a connect command
		command_from_data(&connect_command, client->recv_data);

		// Make sure it's a connect command
		if ( connect_command->type == connect_CMD )
			printf("%s connected\n", connect_command->connect.name);

		client_name_len = strlen(connect_command->connect.name);
		
		extern int load_thread_as_json_value ( GXThread_t **pp_thread, json_value *p_value );

		// Load a player thread
		load_thread_as_json_value(&server_thread, "G10/client thread.json");

		sprintf(server_thread->name, "%.*s thread\0", (client_name_len > 31) ? 32 : (int)client_name_len, connect_command->connect.name);

		// Set the thread in the client
		client->thread = server_thread;

		// Add the thread to the scheduler
		dict_add(p_instance->context.schedule->threads, client->thread->name, client->thread);

		server->client_list [server->client_list_size] = client;
		server->client_list_size++;

		// TODO: Copy
		client->name = connect_command->connect.name;

		// TODO: Dynamically switch actors
		size_t       active_scene_actors_count = dict_values(p_instance->context.scene->actors, 0);
		GXEntity_t **actor_list                = calloc(active_scene_actors_count+1, sizeof(void *));

		client->actors = actor_list;

		// Get a list of actors from the active scene
		dict_values(p_instance->context.scene->actors, (void **)actor_list);

		// Iterate over each actor in the active scene
		for (size_t i = 0; i < active_scene_actors_count; i++)
		{

			// Get an entity from the actor list
			GXEntity_t *entity = actor_list[i];

			// Allocate a command
			GXCommand_t *actor_init_command = calloc(1, sizeof(GXCommand_t));

			// Construct the actor initialize packets
			{
				actor_init_command->type                        = actor_initialize;
				actor_init_command->actor_initialize.name       = entity->name;
				actor_init_command->actor_initialize.location   = entity->transform->location;
				actor_init_command->actor_initialize.quaternion = entity->transform->rotation;
				actor_init_command->actor_initialize.scale      = entity->transform->scale;
				actor_init_command->actor_initialize.index      = (u16) i;

				client->actors[i] = entity;

				// Increment the actor count
				client->actor_count++;

			}

			// Log the entities position
			{
				vec3 l = entity->transform->location;

				printf("\"%s\" @ %.2f %.2f %.2f\n", entity->name, l.x, l.y, l.z);
			}

			// Add the actor initialize packet to the send queue
			queue_enqueue(client->send_queue, actor_init_command);
		}

		// Serialize each command into a data block
		server_serialize(client);

		// Send the  data block to the client
		server_send(client);

		// Start running the client thread
		{
			client->thread->running = true;

			extern int client_work(GXThread_t * thread);

			// TODO: Fix
			// client->thread->thread = SDL_CreateThread(client_work, client->thread->name, client);
		}
	}

	// TODO:
no_instance:
	g_print_error("[G10] [Server] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);

	return 0;
no_socket:
	return 0;
}

int create_client ( GXClient_t **client)
{
	GXClient_t* c = calloc(1, sizeof(GXClient_t));
	*client = c;

	// TODO: Make a constructor
	c->socket = NULL;
	c->name   = NULL;
	c->send_data = calloc(4096, sizeof(u8));
	c->recv_data = calloc(4096, sizeof(u8));

	queue_construct(&c->send_queue);
	queue_construct(&c->recv_queue);

	return 0;
}

int connect_client ( char *name)
{

	// Argument check
	{
		#ifndef NDEBUG
			if (name == (void *)0)
				goto no_client_name;
		#endif
	}

	// Initialized data
	GXInstance_t *p_instance            = g_get_active_instance();
	GXClient_t   *i_client            = 0;
	GXThread_t   *client_thread       = 0;
	IPaddress     addr                = { 0 };
	size_t        name_len            = strlen(name),
		          connect_command_len = 0;
	GXCommand_t  *connect_command     = calloc(1, sizeof(GXCommand_t));

	// Allocate a client
	create_client(&p_instance->networking.client);

	// Return the client
	i_client = p_instance->networking.client;

	// Set the name
	{

		// Initialized data
		size_t name_len = strlen(name);

		// Allocate memory for the name
		i_client->name = calloc(name_len+1, sizeof(char));

		// Copy the name
		strncpy(i_client->name, name, name_len);
	}

	// Connect to the server
	{

		// Resolve the host, or branch to error
		if (SDLNet_ResolveHost(&addr, "localhost", 9999) == -1)
			goto failed_to_resolve_host;

		// Try and open a socket
		i_client->socket = SDLNet_TCP_Open(&addr);

		// Error checking
		{
			if (i_client->socket == NULL)
				goto failed_to_open_tcp_socket;
		}
	}

	GXEntity_t** actor_list = calloc(16 + 1, sizeof(void*));

	i_client->actors = actor_list;

	// Create a connection command
	{


		// Set up the connect command
		{
			connect_command->type         = connect_CMD;
			connect_command->connect.name = name;
		}

		// Write the command to the send buffer
		connect_command_len = (size_t) data_from_command((void **)&i_client->send_data, connect_command);
	}

	// Send the connect command to the server
	SDLNet_TCP_Send(i_client->socket, i_client->send_data, (int) connect_command_len);

	free(connect_command);

	// Set up the networking thread
	{

		// TODO: Fix
		// Load a scheduler thread for the client
		//load_thread_as_json_value(&client_thread, "G10/client thread.json");

		sprintf(client_thread->name, "Network thread");

		// Set the client thread
		{
			i_client->thread          = client_thread;

			// Start running
			i_client->thread->running = true;
		}

		// Add the scheduler to the thread list
		dict_add(p_instance->context.schedule->threads, i_client->thread->name, i_client->thread);

		// Start running the thread
		{

			// Client worker thread
			extern int client_work(GXThread_t * thread);

			// TODO: Fix
			// Create a new thread
			//i_client->thread->thread = SDL_CreateThread(client_work, i_client->thread->name, i_client);

		}
	}

	return 0;

	// Error handling
	{

		// Argument errors
		{
			no_client_name:
				#ifndef NDEBUG
					g_print_error("[G10] [Client] Null pointer provided for parameter \"name\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// Networking errors
		{
			// TODO:
			failed_to_resolve_host:
			failed_to_open_tcp_socket:
				return 0;
		}
	}
}

int command_from_data ( GXCommand_t **ret, void *data )
{
	// TODO: Argument check

	// Initialized data
	GXCommand_t  *i_ret        = calloc(1, sizeof(GXCommand_t));
	GXInstance_t *p_instance   = g_get_active_instance();

	u16           command_type = * ( (u16 *) data );
	size_t        ret_len      = 0;

	i_ret->type = command_type;

	switch (command_type)
	{
		case no_op:

			// Construct the no operation command
			{
				i_ret->no_op.i = 0;
				ret_len += 2;
			}

			break;

		case connect_CMD:

			// Construct the connection command
			{

				// Initialized data
				size_t  name_len    = strlen(((u8 *)data)+2);
				char   *name        = ((char *)data)+2;

				// Allocate memory for client name
				i_ret->connect.name = calloc(name_len + 1, sizeof(u8));

				// Copy the client name
				strncpy(i_ret->connect.name, name, name_len);
			}

			break;

		case actor_initialize:

			// Construct actor initialize command
			{

				// Initialized data
				size_t      name_len = strlen( ((const char *)data) + 0x34 );
				char       *name     = (char *)data + 0x34;
				GXEntity_t *actor    = 0;

				// Set the actor name
				{
					// Allocate memory for actor name
					i_ret->actor_initialize.name = calloc(name_len + 1, sizeof(u8));

					// Copy the client name
					strncpy(i_ret->connect.name, name, name_len);
				}

				i_ret->actor_initialize.index      = * ( (u16 *) data + 1);

				// Set actor location
				i_ret->actor_initialize.location   = * ( (vec3 *)       ( (u8 *) data ) + ( 0x4 ) );

				// Set actor rotation
				i_ret->actor_initialize.quaternion = * ( (quaternion *) ( (u8 *) data ) + ( 0x4 + sizeof(vec3) ) );

				// Set actor scale
				i_ret->actor_initialize.scale      = * ( (vec3 *)       ( (u8 *) data ) + ( 0x4 + sizeof(vec3) + sizeof(quaternion) ) );

				// Find the entity in the scene
				actor = get_entity(p_instance->context.scene, i_ret->actor_initialize.name);

				//// Set location, rotataion, scale
				//actor->transform->location = i_ret->actor_initialize.location;
				//actor->transform->rotation = i_ret->actor_initialize.quaternion;
				//actor->transform->scale    = i_ret->actor_initialize.scale;

				// Return the size of the packet in bytes
				ret_len = 4 + name_len + ( 2 * sizeof(vec3)) + sizeof(quaternion) + 1;
			}

			break;

		case actor_displace_rotate:

			// Construct actor displace rotate command
			{

				// Set the actor index
				i_ret->actor_displace_rotate.index         = * ( (u16 *)        ( (u8 *) data+1 ) );

				// Set actor location
				i_ret->actor_displace_rotate.location      = * ( (vec3 *)       ( (u8 *) data + ( 0x4 ) ) );

				// Set actor rotation
				i_ret->actor_displace_rotate.quaternion    = * ( (quaternion *) ( (u8 *) data + ( 0x4 + sizeof(vec3) ) ) );

				// Set actor scale
				i_ret->actor_displace_rotate.velocity      = * ( (vec3 *)       ( (u8 *) data + ( 0x4 + sizeof(vec3) + sizeof(quaternion) ) ) );

				// Return the size of the packet in bytes
				ret_len = ( sizeof(u16) + 2 * sizeof(vec3) + sizeof(quaternion) );
			}

			break;
		case actor_detach:
			break;
		case chat:
		{
			u16   chat_channel = *((u16*)data+1),
                  chat_len     = *((u16*)data+2);
			char* text_chat    = (char *)data+6;

			i_ret->chat.chat_channel = chat_channel;
			i_ret->chat.chat         = calloc(chat_len + 1, sizeof(char));
			strncpy(i_ret->chat.chat, text_chat, chat_len);
		}
		break;
		case disconnect:
			break;
		default:
			break;
	}

	*ret = i_ret;
	return (int)ret_len;
}

int data_from_command ( void **ret, GXCommand_t *command )
{
	size_t ret_len = 0;
	u8 *retn = *(u8**)ret;
	switch (command->type)
	{
		case no_op:
			ret_len = 2;

			break;
		case connect_CMD:
		{

			size_t name_len = strlen(command->connect.name);
			ret_len += name_len;
			ret_len += 2 + 1;

			*(u16*)retn = connect_CMD;
			strncpy(&retn[2], command->connect.name, name_len);

		}
			break;
		case actor_initialize:
		{

			size_t chat_len = strlen(command->actor_initialize.name);
			ret_len += chat_len;
			ret_len += 2 + 2 + 1 + (2 * sizeof(vec3) ) + sizeof(quaternion);

			*(u16*)retn                                                           = actor_initialize;
			*((u16*)retn+1)                                                       = command->actor_initialize.index;
			*((vec3*)       ((u8*)retn + 4))                                      = command->actor_initialize.location;
			*((quaternion*) ((u8*)retn + 4 + sizeof(vec3)                      )) = command->actor_initialize.quaternion;
			*((vec3*)       ((u8*)retn + 4 + sizeof(vec3) + sizeof(quaternion) )) = command->actor_initialize.scale;


			strncpy( (u8*) ((u8*)retn + 4 + ( 2 * sizeof(vec3) ) + sizeof(quaternion) ), command->actor_initialize.name, chat_len);

		}
			break;
		case actor_displace_rotate:
		{

			ret_len += 2 + 2 + (2 * sizeof(vec3)) + sizeof(quaternion);

			*(u16*)retn                                                   = actor_displace_rotate;
			*((u16*)retn + 1)                                             = command->actor_displace_rotate.index;
			*((vec3*)((u8*)retn + 4))                                     = command->actor_displace_rotate.location;
			*((quaternion*)((u8*)retn + 4 + sizeof(vec3)))                = command->actor_displace_rotate.quaternion;
			*((vec3*)((u8*)retn + 4 + sizeof(vec3) + sizeof(quaternion))) = command->actor_displace_rotate.velocity;

		}
		break;
		case actor_detach:
			break;
		case chat:
		{
			size_t ret_len = 0,
				   chat_len = strlen(command->chat.chat);
			ret_len += chat_len;
			ret_len += 2 + 2 + 2 + 1;

			*(u16*)retn = chat;
			*((u16*)retn+1) = command->chat.chat_channel;
			*((u16*)retn+2) = (u16)chat_len;
			strncpy(&retn[6], command->chat.chat, chat_len);

		}
		break;
		case disconnect:
			break;
		default:
			break;
	}

	return (int) ret_len;
}

int destroy_client ( GXClient_t **pp_client )
{
	//free(client->name);
	//free(client->send_data);
	//free(client->recv_data);
	//queue_destroy(client->send_queue);
	//queue_destroy(client->recv_queue);
	//
	//SDLNet_TCP_Close(client->socket);
	//
	//free(client);
	//
	return 0;
}

int destroy_command ( GXCommand_t **pp_command )
{
	//// Argument check
	//{
	//	#ifndef NDEBUG
	//		if ( pp_command == (void *) 0 )
	//			goto no_command;
	//	#endif
	//}
	//
	//// Deallocate command data
	//switch (command->type)
	//{
	//	case no_op:
	//	case actor_displace_rotate:
	//	case actor_detach:
	//	case disconnect:
	//	break;
	//
	//	case connect_CMD:
	//	{
	//		free(command->connect.name);
	//	}
	//	break;
	//
	//	case actor_initialize:
	//	{
	//		free(command->actor_initialize.name);
	//	}
	//	break;
	//
	//	case chat:
	//	{
	//		free(command->chat.chat);
	//	}
	//	break;
	//
	//	default:
	//	break;
	//}
	//
	//// Deallocate the command
	//free(command);
	//
	//no_command:
	return 0;
}

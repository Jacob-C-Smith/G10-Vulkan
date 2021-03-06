#include <G10/GXScheduler.h>

dict* scheduler_tasks = 0;

char* task_names[9] = {
	"Input",
	"UI",
	"AI",
	"Pre AI",
	"Resolve Collisions",
	"Update Forces",
	"Move objects",
	"Animation",
	"Render"
};

void* task_function_pointers[9] = {
	&process_input,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	&render_frame
};

void init_scheduler(void)
{
	GXInstance_t *instance = g_get_active_instance();

	dict_construct(&scheduler_tasks, 16);

	for (size_t i = 0; i < 9; i++)
	{
		GXTask_t *task = 0;
		
		create_task(&task, task_names[i], task_function_pointers[i]);

		dict_add(scheduler_tasks, task_names[i], task);
	}

}

int create_task (GXTask_t **task, char *name, int(*function_pointer)(GXInstance_t *))
{
	// TODO: Argument check

	GXTask_t *i_task = calloc(1, sizeof(GXTask_t));
	*task = i_task;


	i_task->name = name;

	i_task->function_pointer = function_pointer;


	return 1;
	// TODO: Error handling
}

int create_scheduler(GXScheduler_t  **scheduler)
{
	// TODO: Argument check

	GXScheduler_t *i_scheduler = calloc(1, sizeof(GXScheduler_t));

	// TODO: Memory check
	*scheduler = i_scheduler;

	// TODO: Error handling
	return 1;
}

int create_thread ( GXThread_t **thread )
{
	// TODO: Argument check

	GXThread_t *i_thread = calloc(1, sizeof(GXThread_t));

	// TODO: Memory check
	*thread = i_thread;

	// TODO: Error handling
	return 1;
}

int load_scheduler_as_json ( GXScheduler_t** scheduler, char* token_text, size_t len )
{
	// TODO: Argument check

	// Initialized data
	dict    *scheduler_json      = 0;
	char    *name                = 0;
	size_t   thread_count        = 0;
	char   **thread_json_objects = 0;

	// Parse the JSON
	{

		/*
			{
				"name"         : "Scheduler",
			    "thread count" : 1,
				"blocks"       : [
					{
						"name"        : "Game loop",
						"description" : "Single threaded solution",
						"tasks"       : [
							"Input",
							"Render"
						]
					}
				]
			}
		*/

		// Initiaized data
		JSONToken_t *token = 0;

		// Parse the JSON into a dictionary
		parse_json(token_text, len, &scheduler_json);

		// Get the name
		token               = dict_get(scheduler_json, "name");
		name                = JSON_VALUE(token, JSONstring);

		// Get the blocks
		token               = dict_get(scheduler_json, "blocks");
		thread_json_objects = JSON_VALUE(token, JSONarray);

	}

	// Construct the scheduler
	{

		// Initialized data
		GXScheduler_t *i_scheduler = 0;

		// Allocate a scheduler
		create_scheduler(scheduler);

		i_scheduler = *scheduler;

		// Set the name
		{

			// Initialized data
			size_t name_len = strlen(name);

			// Allocate for name
			i_scheduler->name = calloc(name_len + 1, sizeof(char));
			
			// Copy name
			strncpy(i_scheduler->name, name, name_len);
		}

		// Construct each thread
		{

			// Count up the blocks
			while (thread_json_objects[++thread_count]);

			dict_construct(&i_scheduler->threads, 16);

			// Iterate over each block JSON object
			for (size_t i = 0; i < thread_count; i++)
			{

				// Initialized data
				GXThread_t *thread = 0;

				// Load the thread from the JSON
				load_thread_as_json(&thread, thread_json_objects[i], strlen(thread_json_objects[i]));

				dict_add(i_scheduler->threads, thread->name, thread);

			}
		}
	}


	return 0;

	// TODO: Error handling
}

int load_thread_as_json(GXThread_t** thread, char* token_text, size_t len)
{
	// TODO: Argument check

	// Initialized data
	dict    *thread_json = 0;
	char    *name        = 0;
	size_t   task_count  = 0;
	char   **tasks       = 0;

	// Parse the JSON
	{
		/*
		{
            "name"        : "Game loop",
            "description" : "Single threaded solution",
            "tasks"       : [
                "Input",
                "Render"
            ]
        }
		*/
		// Initiaized data
		JSONToken_t *token = 0;

		// Parse the JSON into a dictionary
		parse_json(token_text, len, &thread_json);

		// Get the name
		token = dict_get(thread_json, "name");
		name  = JSON_VALUE(token, JSONstring);

		// Get the tasks
		token = dict_get(thread_json, "tasks");
		tasks = JSON_VALUE(token, JSONarray);

	}

	// Construct the thread
	{

		// Initialized data
		GXThread_t *i_thread = 0;

		// Allocate a thread
		create_thread(thread);

		i_thread = *thread;

		// Set the name
		{

			// Initialized data
			size_t name_len = strlen(name);

			// Allocate for name
			i_thread->name = calloc(name_len + 1, sizeof(char));
			
			// Copy name
			strncpy(i_thread->name, name, name_len);
		}

		// Construct each task
		{
			
			// Count up the tasks
			while (tasks[++task_count]);

			i_thread->tasks = calloc(task_count + 1, sizeof(void*));

			// Iterate over each block JSON object
			for (size_t i = 0; i < task_count; i++)
			{
				// Initialized data
				i_thread->tasks[i] = (GXThread_t*)dict_get(scheduler_tasks, tasks[i]);

			}
		}
	}


	return 0;

	// TODO: Error handling
}

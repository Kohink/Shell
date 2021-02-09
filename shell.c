#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct {
	int size;
	char **items;
} tokenlist;

char *get_input(void);
tokenlist *get_tokens(char *input);

tokenlist *new_tokenlist(void);
void add_token(tokenlist *tokens, char *item);
void free_tokens(tokenlist *tokens);
char *expand_dollar_sign(char *var);

void pathSearch(char *path, char *args[100]);

int main()
{
	// Part 3: Prompt
	const char *path = getenv("PATH");
	char *user = getenv("USER");
	char *machine = getenv("MACHINE");
	char *pwd = getenv("PWD");
	char *home = getenv("HOME");
	char *newPWD = strcat(pwd, "");

	printf("%s@%s: %s > ", user, machine, pwd);
	
	while(1) {
		/* input contains the whole command
		 * tokens contains substrings from input split by spaces
		 */
		char *input = get_input();
		
		char *temp = (char *)malloc(strlen(path) + 1);
		strcpy(temp, path);

		// printf("PATH: %s\n", temp);

		tokenlist *tokens = get_tokens(input);
		for (int i = 0; i < tokens->size; i++) 
		{
			if (strcmp(tokens->items[0], "echo") == 0 && i > 0)
			{
				// Part 2: Retrieving environmental variables
				if(strstr(tokens->items[i], "$") != NULL)
				{
					printf("%s ", expand_dollar_sign(tokens->items[i]));
					break;
				}
				// needs to check if path exists before printing 
				else if(strcmp(tokens->items[i], "~") == 0) 
				{
					printf("%s ", home);
					break;
				}
				else
				{
					printf("%s ", tokens->items[i]);
					break;
				}
			}
			else if (strcmp(tokens->items[0], "cd") == 0)
			{
				// Checks to see if cd has one argument or less

				 if(tokens->items[2]!= NULL)
				 {
				 	printf("\ncd only takes one argument");
				 	break;
				 }
				 // cd into home directory
				 else if(tokens->items[1] == NULL || strcmp(tokens->items[1], "~") == 0 || strcmp(tokens->items[1], "$HOME") == 0 )
				 {
				 	chdir(home);
					newPWD = strcat(home, "");
				 	break;
				 }
				else // actual action of cd
				 {
					char *newpath = strcat(temp,"/");

					newpath = strcat(temp, tokens->items[1]);
				 	chdir(tokens->items[1]);
					// newPWD = strcat(..., "");
				 	break;
				 }
			}
			else if (strcmp(tokens->items[0], "jobs") == 0)
			{
				// Job number + CMD PID + CMD command line
				break;
			}
			else if (strcmp(tokens->items[0], "exit") == 0)
			{
				printf("Shell has been running for: %d seconds\n");
				printf("The longest running command took %d seconds to execute\n");
				exit(0);
				break;
			}
			// Standalone tilde expansion
			else if (strcmp(tokens->items[0], "~") == 0)
			{
				printf("%s ", home);
				break;
			}
			else
			{
				pathSearch(temp, tokens->items);
				// printf("W");
				break;
			}
			
		}
		if (strcmp(tokens->items[0], "echo") == 0)
		{
			printf("\n");
		}

		printf("%s@%s: %s > ", user, machine, newPWD);

		free(input);
		free_tokens(tokens);
	};

	return 0;
}

char *expand_dollar_sign(char *var)
{
	if(var[0] == '$')
	{
		char *expand = getenv(&var[1]);
		// Returns empty environmental variable if it does not exist
		if(!expand)
		{
			char* dealloc = malloc(1);
      		dealloc[0] = '\0';
      		return dealloc;
		}
		else
			return strdup(expand);
	}
	
}


tokenlist *new_tokenlist(void)
{
	tokenlist *tokens = (tokenlist *) malloc(sizeof(tokenlist));
	tokens->size = 0;
	tokens->items = (char **) malloc(sizeof(char *));
	tokens->items[0] = NULL; /* make NULL terminated */
	return tokens;
}

void add_token(tokenlist *tokens, char *item)
{
	int i = tokens->size;

	tokens->items = (char **) realloc(tokens->items, (i + 2) * sizeof(char *));
	tokens->items[i] = (char *) malloc(strlen(item) + 1);
	tokens->items[i + 1] = NULL;
	strcpy(tokens->items[i], item);

	tokens->size += 1;
}

char *get_input(void)
{
	char *buffer = NULL;
	int bufsize = 0;

	char line[5];
	while (fgets(line, 5, stdin) != NULL) {
		int addby = 0;
		char *newln = strchr(line, '\n');
		if (newln != NULL)
			addby = newln - line;
		else
			addby = 5 - 1;

		buffer = (char *) realloc(buffer, bufsize + addby);
		memcpy(&buffer[bufsize], line, addby);
		bufsize += addby;

		if (newln != NULL)
			break;
	}

	buffer = (char *) realloc(buffer, bufsize + 1);
	buffer[bufsize] = 0;

	return buffer;
}

tokenlist *get_tokens(char *input)
{
	char *buf = (char *) malloc(strlen(input) + 1);
	strcpy(buf, input);

	tokenlist *tokens = new_tokenlist();

	char *tok = strtok(buf, " ");
	while (tok != NULL) {
		add_token(tokens, tok);
		tok = strtok(NULL, " ");
	}

	free(buf);
	return tokens;
}

void free_tokens(tokenlist *tokens)
{
	for (int i = 0; i < tokens->size; i++)
		free(tokens->items[i]);
	free(tokens);
}

void pathSearch(char *path, char *args[100])
{
	int i = 0;
    char *pathy = strtok(path, ":");
    char *array[1000];
	char arraydos[1000][1000];

    while (pathy != NULL)
    {
        array[i++] = pathy;
        pathy = strtok(NULL, ":");
    }

	for(int j = 0; j < i; j++)
	{
		for (int t = 0; t <= strlen(array[j]); t++)
		{
			if (t == strlen(array[j]))
			{
				arraydos[j][t] = '/';
				arraydos[j][t + 1] = '\0';
				arraydos[j][t + 2] = '\0';
				arraydos[j][t + 3] = '\0';
				arraydos[j][t + 4] = '\0';


				char *temp = strcat(args[0], "");

				for (int com = 0; com < strlen(temp); com++)
					arraydos[j][t + com + 1] = temp[com];
			}
			else
				arraydos[j][t] = array[j][t];
		}
		// printf("%s\n", arraydos[j]);
	}

	int status;
	char *argsdos[3];

	for(int i = 0; i < sizeof(arraydos); i++)
	{
		argsdos[0] = arraydos[i];        // first arg is the full path to the executable
		argsdos[1] = args[1];            // list of args must be NULL terminated
		argsdos[2] = NULL;

		if ( fork() == 0 )
		{
			// printf("%s\n", argsdos[0]);
			execv(argsdos[0], args); // child: call execv with the path and the args
		}
		else
		{
			wait( &status );
			break;
		}
	}
}
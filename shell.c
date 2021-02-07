#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
void findPath(char *path);

int main()
{
	while(1) {
		// Part 3: Prompt
		char *user = getenv("USER");
		char *machine = getenv("MACHINE");
		char *pwd = getenv("PWD");
		char *home = getenv("HOME");
		char *path = getenv("PATH");

		printf("%s@%s: %s > ", user, machine, pwd);

		/* input contains the whole command
		 * tokens contains substrings from input split by spaces
		 */
		char *input = get_input();

		tokenlist *tokens = get_tokens(input);
		for (int i = 0; i < tokens->size; i++) 
		{
			if (strcmp(tokens->items[0], "echo") == 0 && i > 0)
			{
				// Part 2: Retrieving environmental variables
				if(strstr(tokens->items[i], "$") != NULL)
				{
					printf("%s ", expand_dollar_sign(tokens->items[i]));
					
				}
				// needs to check if path exists before printing 
				else if(strcmp(tokens->items[i], "~") == 0) 
				{
					printf("%s ", home);
				}
				else
				{
					printf("%s ", tokens->items[i]);
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
				else if(tokens->items[1] == NULL || strcmp(tokens->items[1], "~") == 0)
				{
					chdir(home);
					printf("youre home");
					break;
				}
			}
			else if (strcmp(tokens->items[0], "jobs") == 0)
			{
					
			}
			else if (strcmp(tokens->items[0], "exit") == 0)
			{
				exit(0);
			}
			else if (strcmp(tokens->items[0], "ls") == 0)
			{
					findPath(path);
			}
			
		}
		printf("\n");

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

void findPath(char *path)
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
				arraydos[j][t + 1] = 'l';
				arraydos[j][t + 2] = 's';
			}
			else
				arraydos[j][t] = array[j][t];
		}

		// PRINTS ARRAY FOR TESTING.
		printf("%s\n", arraydos[j]);
	}
}
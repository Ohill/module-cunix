#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>	
#include <fcntl.h>	
#include "../include/hash.h"
#include "../include/linked_list.h"

char PWD[1024];
char PATH[1024];	

char  *builtin[] = {"exit", "echo", "export", "env", "ls"};

node_t *keys_of_export_lst; 
hashtable_t  *hash_table_of_export;

int shell_exit(char **args)
{
	hash_destroy(hash_table_of_export, &free_data);
	list_destroy(&keys_of_export_lst, &free_data);
	return 0;
}

int shell_env (char **args)
{
	node_t *tmp = keys_of_export_lst;
	while( tmp != NULL)
	{
		printf("%s: %s\n", tmp->data, hash_get(hash_table_of_export, tmp->data));
		tmp = tmp->next;
	}
	return 1; 
}

char *without_first_char(char *str) 
{
	for (int i = 0; i <= strlen(str); i++)
		str[i] = str[i + 1];
	return str;
}

int shell_echo(char ** args)
{
	int i = 1;
	while (1)
	{	
		if (args[i] == NULL)
			break;
		if (args[i][0] == '$')
			printf("%s ", hash_get(hash_table_of_export, without_first_char(args[i])));
		else printf("%s ", args[i]);
		i++;
	}
	printf("\n");
}

int check_export_args(char * arg)
{
	int i = 0;
	int size = strlen(arg);
	for (i; i < size; i++)
		if (arg[i] == '=' && i+1 != size)
		{
			i = -1;
			break;
		}
	return i == -1 ? 0 : 1;
}

int shell_export(char **args)
{
	int i = 1;
	while(1)
	{
		if(args[i] == NULL)
		break;
		else if (check_export_args(args[i]))
			return printf("minishell: bad assigment\n"); 
		  char *token;
		  char s[2] = "=";
		  token = strtok(args[i], s);
		  if (keys_of_export_lst == NULL && hash_table_of_export == NULL)
		  {
			  keys_of_export_lst = list_create(token);
			  hash_table_of_export = hash_create(18);
		  }
		  else if (list_check(keys_of_export_lst, token))
			  list_push(keys_of_export_lst, token);
		  char *tmp = token;
		  token = strtok(NULL, s);
		  hash_set(hash_table_of_export, tmp, token);
		i++;
	}
	return 1;
}

int shell_ls(char **args)
{
	struct dirent **name;
	int n, i = 0;
	if (args[1] == NULL)
		n = scandir(".", &name, NULL, alphasort);
	else n = scandir(args[1], &name, NULL, alphasort);
	while (i < n)
	{
		if (strcmp(name[i]->d_name, ".") && strcmp(name[i]->d_name, ".."))
		{
			write (1, name[i]->d_name, strlen(name[i]->d_name));
			if (i != n-1)
				write (1, " ", 1);
		}
		free(name[i]);
		i++;
	}
	write (1, "\n", 1);
	free(name);
	return 1;
}

int (*builtin_function[]) (char **) = {
	&shell_exit,
	&shell_echo,
	&shell_export,
	&shell_env,
  	&shell_ls
};

char **split_command_line(char *command)
{
	int position = 0;
	int no_of_tokens = 64;
	char ** tokens = malloc(sizeof(char *) * no_of_tokens);
	char delim[2] = " ";
	char * token = strtok(command, delim);
	while (token != NULL)
	{
		tokens[position] = token;
		position++;
		token = strtok(NULL, delim);
	}
	tokens[position] = NULL;
	return tokens;
}

char *read_command_line(void)
{
	int position = 0;
	int buf_size = 1024;
	char *command = (char *)malloc(sizeof(char) * 1024);
	char symbol;
	symbol = getchar();
	while (symbol != EOF && symbol != '\n')
	{
		command[position] = symbol;
		if (position >= buf_size)
		{
			buf_size += 64;
			command = realloc(command, buf_size);
		}
		position++;
		symbol = getchar();
	}
		command[position] = '\0';
	return command;
}

int shell_execute(char **args)
{
	int i = 0;
	if (args[0] == NULL)
		return 1;
	for (i; i < 5; i++) 
		if (strcmp(args[0], builtin[i]) == 0) 
			return (*builtin_function[i])(args);
  return printf("minishell: this command doesnt exist");
}

void signalHandler(int sign)
{
	if (sign == SIGINT)
	{
		puts("");
		write(1, "$_>", 3);
		signal(SIGINT, signalHandler);
	}
}

void shell_loop(void)
{
	char *command_line;
	char **arguments;
	int	status = 1;
	while (status)
	{
		printf("$_> ");
    		signal(SIGINT, signalHandler);
		command_line = read_command_line();
			if ( strcmp(command_line, "") == 0 )
				continue;
		arguments = split_command_line(command_line);
		status = shell_execute(arguments);
	}
}

int main()
{
	getcwd(PWD, sizeof(PWD));
	strcpy(PATH, PWD);
	strcat(PATH, "/cmds/");
	shell_loop();
	return 0;
}

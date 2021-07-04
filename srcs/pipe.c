#include "minishell.h"


char *process_pipe(char *str, t_env *env, t_store *token)
{
	char *tmp;
	int i;

	i = 1;
	while(*str == ' ' || *str == '\t')
        str++;
	while(str[i] == ' ' || str[i] == '\t')
		i++;
	if (!str[i])
	{
		ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
		return (NULL);
	}
	while (str[i] && str[i] != '>' && str[i] != '<' && str[i] != '|')
    {
        if (str[i] == '\'')
        {
            str = single_quotes(str, &i);
            i = i - 2;
        }
        if (str[i] == '\"')
        {
            str = double_quotes(str, &i, env);
            i = i - 2;
        }
        if (str[i] == '$')
        {
            str = process_dollar(str, &i, env);
            if (!str)
                return (NULL);
        }
        i++;
    }
	if (!token->word)
	{
		ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
		return (NULL);
	}
	else
		token = add_node_token(token, str, i);
	tmp = ft_substr(str, i, ft_strlen(str) - i);
	// free(str);
	if (!tmp)  
		return (NULL);
	return (tmp);
}
	
	
	
	
	
	
	// char *temp;
	// char *pipe;
	// int i;

	// i = 0;
	// str++;
	// temp = NULL;
	// pipe = NULL;
	// t_sh.pipe_flag = 1;
	// if (!str)
	// {
	// 	//  ожидает ввод команды
	// 	return (NULL);
	// }
	// while(str[i])
	// {
	// 	if (str[i] == '\0' || str[i] == '|' || str[i] == '>' || str[i] == '<')
	// 		break;
	// 	i++;
	// }
	// pipe = ft_substr(str, 0, i);
	// pipe = process_value(pipe, env);
	// temp = ft_substr(str, i, ft_strlen(str) - i);

	// if_pipe(token, envp, export, env);

	// printf("pipe: %s\n", pipe);
	// printf("temp: %s\n", temp);
	// return (temp);
// }
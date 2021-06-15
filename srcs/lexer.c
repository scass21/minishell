#include "minishell.h"

static int	ft_free(char *str)
{
	if (str)
		free(str);
	str = NULL;
	return (0);
}

static char *copy_str_without_quotes(char *str, int j, int i)
{
    char *tmp;
    int k;
    int l;

    tmp = NULL;
    k = -1;
    l = 0;
    tmp = (char *)malloc(sizeof(char) * (ft_strlen(str) - 2) + 1);
    if (!tmp)
        ft_error(1);
    while(str[++k] && (k != j))
        tmp[l++] = str[k];
    k = j;
    while(str[++k] && (k != i))
        tmp[l++] = str[k];
    k = i;
    while(str[++k])
        tmp[l++] = str[k];
    tmp[l] = '\0';
    return(tmp);
}

// static char *process_slash(char *str, int *i)                            //для обработки слэша
// {
//     char *tmp;
//     int j;
//     int k;

//     tmp = NULL;
//     j = 0;
//     k = 0;
//     tmp = (char *)malloc(sizeof(char) * ft_strlen(str));
//     if (!tmp)
//         ft_error(1);
//     while(str[j] && (j != *i))
//     {
//         tmp[k] = str[j];
//         k++;
//         j++;
//     }
//     j = *i + 1;
//     if (!str[j])
//     {
//         ft_free(tmp);
//         printf("minitoken: missing slash.\n");
//         return("\0");
//     }
//     while(str[j])
//     {
//         tmp[k] = str[j];
//         j++;
//         k++;
//     }
//     tmp[k] = '\0';
//     ft_free(str);
//     return (tmp);
// }


static char *get_key(char *str, int j, int i)
{
    char *tmp1;
    char *tmp2;

    tmp1 = NULL;
    tmp2 = NULL;
    tmp1 = ft_substr(str, j + 1, i - j - 1);
    if (!tmp1)
        ft_error(1);
    tmp2 = ft_strjoin(tmp1, "=");
    if (!tmp2)
        ft_error(1);
    return (tmp2);
}

static char *get_second_tmp(char *str, int j, int i, char **envp)
{
    char *tmp;
    int len;
    int flag;
    int k;

    tmp = get_key(str, j, i);
    len = ft_strlen(tmp);
    flag = 0;
    k = 0;
    while(envp[k])
    {
        if (!ft_strncmp(envp[k], tmp, len))
        {
            free(tmp);
            tmp = ft_substr(envp[k], len, ft_strlen(envp[k]) - len);
            flag = 1;
            break;
        }
        k++;
    }
    if (flag == 0)
        tmp = "\0";
    return(tmp);
}

static char *process_dollar(char *str, int *i, char **envp)
{
    int j;
    char *tmp1;
    char *tmp2;
    char *tmp3;

    j = *i;
    (*i)++;
    if (!str[*i] || str[*i] == '\'' || str[*i] == '\"')
        return (str);
    while(str[*i])
    {
        if (str[*i] != '_' && !ft_isalnum(str[*i]))
            break;
        (*i)++;
    }
    tmp1 = ft_substr(str, 0, j);
    tmp2 = get_second_tmp(str, j, *i, envp);
    tmp3 = ft_substr(str, *i, ft_strlen(str));
    tmp2 = ft_strjoin(tmp1, tmp2);
    str = ft_strjoin(tmp2, tmp3);
    return (str);
    
}

static char *double_quotes(char *str, int *i, char **envp)
{
    int j;
    char *tmp;

    j = *i;
    tmp = NULL;
    (*i)++;
    while(str[*i])
    {
        // if (str[*i] == '\\' && str[*i + 1] == '\\')                    //для обработки слэша
        //     str = process_slash(str, i);
        // if (str[*i] == '\\' && str[*i + 1] == '\"')
        // {
        //     str = process_slash(str, i);
        //     (*i)++;
        // }
        // if (str[*i] == '\\' && str[*i + 1] == '$')
        //     str = process_slash(str, i);
        if (str[*i] == '$')
            str = process_dollar(str, i, envp);
        if (str[*i] == '\"')
            break;
        (*i)++;
    }
    if (*i == ft_strlen(str))
    {
        printf("minitoken: missing second quote.\n");
        return("\0");
    }
    tmp = copy_str_without_quotes(str, j, *i);
    // ft_free(str);
    return(tmp);
}

static char *single_quotes(char *str, int *i)
{
    int j;
    char *tmp;

    j = *i;
    tmp = NULL;
    (*i)++;
    while(str[*i])
    {
        if (str[*i] == '\'')
            break;
        (*i)++;
    }
    if (*i == ft_strlen(str))
    {
        printf("minitoken: missing second quote.\n");
        return ("\0");
    }
    tmp = copy_str_without_quotes(str, j, *i);
    ft_free(str);
    return(tmp);
}

static char *str_without_space(char *str, int j, int i)
{
    char *tmp1;
    char *tmp2;

    tmp1 = ft_substr(str, 0, j + 1);
    tmp2 = ft_substr(str, i, ft_strlen(str));
    tmp1 = ft_strjoin(tmp1, tmp2);
    return(tmp1);
}

t_token *add_node(t_token *token, char *str, int i)
{
    t_token *temp;
    t_token *p;

    temp = (t_token *)malloc(sizeof(t_token));
    if (!temp)
        ft_error(1);
    p = token->next;
    token->next = temp;
    temp->word = ft_substr(str, 0, i);
    if (!temp->word)
        ft_error(1);
    temp->next = p;
    return (temp);
}

static char *process_space(char *str, int i)
{
    int j;
    char *tmp;

    j = i;
    i++;
    if (!str[i])
        return (str);
    while(str[i] == ' ' || str[i] == '\t')
        i++;
    if (j + 1 == i)
    {
        return(str);
    }
    tmp = str_without_space(str, j, i);
    return (tmp);
}

static int parser(char *str, char **envp, t_token *token)
{
    //           "" '' $ | > >> < <<

    int i;
    int flag;

    i = 0;
    flag = 0;
    while(*str == ' ' || *str == '\t')
        str++;
    while(str[i])
    {
        if (str[i] == '\'')
        {
            str = single_quotes(str, &i);
            i = i - 2;
        }
        if (str[i] == '\"')
        {
            str = double_quotes(str, &i, envp);
            i = i - 2;
        }
        if (str[i] == '$')
        {
            str = process_dollar(str, &i, envp);
            if (!str)
                return (-1);
        }
        if (str[i] == ' ' || str[i] == '\t')
        {
            if (!token->word)
            {
                token->word = ft_substr(str, 0, i);
                if (!token->word)
                    ft_error(1);
            }
            else
                token = add_node(token, str, i);
            while(str[i] == ' ' || str[i] == '\t')
                i++;
            str = ft_substr(str, i, ft_strlen(str) - i);
            i = -1;
        }
        i++;
    }
    if (!token->word)
    {
        token->word = ft_strdup(str);
        if (!token->word)
            ft_error(1);
    }
    else
        token = add_node(token, str, ft_strlen(str));
    return (0);
}

static void init_struct_token(t_token *token)
{
    
    token->word = NULL;
    token->next = NULL;
}

static int count_argument(t_token *token)
{
    t_token *p;
    int count;

    p = token;
    count = 0;
    while(p != NULL)
    {
        count++;
        p = p->next;
    }
    return (count - 1);
}

static int execute_command(char **envp, t_token *token)
{
    t_token *p;
    int n_flag;
    int count;

    n_flag = 0;
    count = count_argument(token);
    if (ft_strcmp(token->word, "echo") == 0)
    {
        p = token->next;
        if (!p)
            our_echo(NULL);
        if (ft_strcmp(p->word, "-n") == 0)
        {
            p = p->next;
            n_flag = 1;
        }
        while(p != NULL)
        {
            our_echo(p->word);
            p = p->next;
            printf(" ");
        }
        if (n_flag == 0)
            printf("\n");
        return (0);
    }
    else if (ft_strcmp(token->word, "pwd") == 0)
        our_pwd();
    else if (ft_strcmp(token->word, "cd") == 0)
    {
        p = token->next;
        our_cd(count, p->word);
    }
    // else if (ft_strcmp(token->word, "export") == 0)
    //     our_export();
    // else if (ft_strcmp(token->word, "unset") == 0)
    //     our_unset();
    else if (ft_strcmp(token->word, "env") == 0)
    {
        p = token->next;
        if (p)
        {
            printf("env: %s: No such file or directory\n", p->word);
            return (0);
        }
        our_env(envp);
    }
    // else if (ft_strcmp(token->word, "exit") == 0)
    //     our_exit();
    else
        printf("minishell: %s: command not found\n", token->word);
    return (0);
}

static void free_struct(t_token *token)
{
    t_token *p;

    while(token != NULL)
    {
        p = token;
        token = token->next;
        free(p);
    }
}

void	our_sig_proc(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\nminishell: ", 12);
		if (t_sh.fork_status == 0)
			t_sh.exit_code = 1;				// value for $?
		else
			t_sh.exit_code = 130;
	}
	else if (sig == SIGQUIT)
	{
		write(1, "\nminishell: ", 12);
		t_sh.exit_code = 131;

	}
}

int main(int argv, char **argc, char **envp)
{
    char *str;
    t_token *token;
    t_shell		t_sh;

	signal(SIGINT, our_sig_proc);
	signal(SIGQUIT, our_sig_proc);
    init_mini(&t_sh);
    while (1)
    {
        token = (t_token*)malloc(sizeof(t_token));
        if (!token)
            ft_error(1);
        str = NULL;
        init_struct_token(token);
        str = readline("minishell$ ");
        parser(str, envp, token);
        free(str);
        execute_command(envp, token);
        free_struct(token);
    }
    return (0);

}
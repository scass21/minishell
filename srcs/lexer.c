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

static char *get_tmp(char *str, int j, int i)
{
    char *tmp1;
    char *tmp2;

    tmp1 = NULL;
    tmp2 = NULL;
    tmp1 = ft_substr(str, j + 1, i - j - 1);
    if (!tmp1)
        ft_error(1);
    return (tmp1);
}

static char *get_second_tmp(char *str, int j, int i, t_env *env)
{
    char *tmp;
    int len;
    int flag;
    t_env *p;
    

    tmp = get_tmp(str, j, i);
    len = ft_strlen(tmp);
    flag = 0;
    p = env;
    while(p != NULL)
    {
        if (!ft_strcmp(p->key, tmp))
        {
            free(tmp);
            tmp = ft_strdup(p->value);
            flag = 1;
            break;
        }
        p = p->next;
    }
    if (flag == 0)
        tmp = "\0";
    return(tmp);
}

char *process_dollar(char *str, int *i, t_env *env)
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
    tmp2 = get_second_tmp(str, j, *i, env);
    tmp3 = ft_substr(str, *i, ft_strlen(str));
    tmp2 = ft_strjoin(tmp1, tmp2);
    str = ft_strjoin(tmp2, tmp3);
    return (str);
}

char *double_quotes(char *str, int *i, t_env *env)
{
    int j;
    char *tmp;

    j = *i;
    tmp = NULL;
    (*i)++;
    while(str[*i])
    {
        if (str[*i] == '$')
            str = process_dollar(str, i, env);
        if (str[*i] == '\"')
            break;
        (*i)++;
    }
    if (*i == ft_strlen(str))
    {
        printf("minishell: missing second quote.");
        return("\0");
    }
    tmp = copy_str_without_quotes(str, j, *i);
    return(tmp);
}

char *single_quotes(char *str, int *i)
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
        printf("minishell: missing second quote.");
        return ("\0");
    }
    tmp = copy_str_without_quotes(str, j, *i);
    ft_free(str);
    return(tmp);
}

t_store *add_node_token(t_store *token, char *str, int i)
{
    t_store *temp;
    t_store *p;

    temp = (t_store *)malloc(sizeof(t_store));
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

static int parser(char *str, t_env *env, t_store *token)
{
    int i;

    i = 0;
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
            str = double_quotes(str, &i, env);
            i = i - 2;
        }
        if (str[i] == '$')
        {
            str = process_dollar(str, &i, env);
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
                token = add_node_token(token, str, i);
            while(str[i] == ' ' || str[i] == '\t')
                i++;
            if (i == ft_strlen(str))
                return (0);
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
        token = add_node_token(token, str, ft_strlen(str));
    return (0);
}

void init_struct_store(t_store *token)
{
    
    token->word = NULL;
    token->next = NULL;
}

static int count_argument(t_store *token)
{
    t_store *p;
    int count;

    p = token;
    count = 0;
    while(p != NULL)
    {
        count++;
        p = p->next;
    }
    return (count);
}

static int execute_command(t_env *env, t_env * export, t_store *token)
{
    t_store *p;
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
        if (ft_strcmp(p->word, "$?") == 0)
        {
            printf("%d", t_sh.exit_code);
            return (0);
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
        if (count != 1)
            our_cd(count, p->word, env, export);
        else
            our_cd(count, NULL, env, export);
    }
    else if (ft_strcmp(token->word, "export") == 0)
        our_export(env, export, token);
    else if (ft_strcmp(token->word, "unset") == 0)
        our_unset(env, export, token);
    else if (ft_strcmp(token->word, "env") == 0)
    {
        p = token->next;
        if (p)
        {
            printf("env: %s: No such file or directory\n", p->word);
            return (0);
        }
        our_env(env);
    }
    else if (ft_strcmp(token->word, "\n") == 0)
    {
        write(1, "\n", 1);
    }
    else
        exec_bin(token);
        // printf("exec_bin on\n");
    return (0);
}

static void free_struct_store(t_store *token)
{
    t_store *p;
    while(token != NULL)
        {
            p = token;
            token = token->next;
            if (p)
                free(p);
        }
}

t_env *add_node_env(t_env *env, char *key, char *value)
{
    t_env *temp;
    t_env *p;

    temp = (t_env *)malloc(sizeof(t_env));
    if (!temp)
        ft_error(1);
    p = env->next;
    env->next = temp;
    temp->key = ft_strdup(key);
    if (!temp->key)
        ft_error(1);
    if (value)
    {
        temp->value = ft_strdup(value);
        if (!temp->value)
            ft_error(1);
    }
    else
        temp->value = "\0";
    temp->next = p;
    return (env);
}

static void fill_struct_env(char **envp, t_env *env)
{
    int i;
    char *key;
    char *value;

    i = 0;
    while(envp[i])
    {
        if (ft_strchr(envp[i], '='))
        {
            key = get_key(envp[i]);
            value = get_value(envp[i]);
        }
        else
        {
            key = ft_strdup(envp[i]);
            if (!key)
                ft_error(1);
            value = NULL;
        }
        if (!env->key)
        {
            env->key = ft_strdup(key);
            if (!env->key)
                ft_error(1);
            if (value)
            {
                env->value = ft_strdup(value);
                if (!env->value)
                    ft_error(1);
            }
        }
        else
            env = add_node_env(env, key, value);
        i++;
    }
}

void	init_mini()
{
	t_sh.exit_code = 0;
	t_sh.fork_status = 0;
	// t_sh->path = NULL;
	// t_sh->str = NULL;
	// ft_bzero(t_sh->str, ft_strlen(t_sh->str));
	// t_sh->col = 0;
	// t_sh->row = 0;
	// t_sh->win_col = 0;
	// t_sh->win_row = 0;

}

static void init_struct_env(t_env *env)
{
    env->key = NULL;
    env->value = NULL;
    env->next = NULL;
}

int main(int argv, char **argc, char **envp)
{
    char *str;
    t_store *token;
    t_env *env;
    t_env *export;
    
    init_mini();
    env = (t_env *)malloc(sizeof(t_env));
    if (!env)
        ft_error(1);
    init_struct_env(env);
    fill_struct_env(envp, env);


    export = (t_env *)malloc(sizeof(t_env));
    if (!export)
        ft_error(1);
    init_struct_env(export);
    fill_struct_env(envp, export);
    signal(SIGINT, our_sig_proc);
	signal(SIGQUIT, our_sig_proc);
    while (1)
    {
        token = (t_store *)malloc(sizeof(t_store));
        if (!token)
            ft_error(1);
        str = NULL;
        init_struct_store(token);
        str = readline("minishell$ ");
        parser(str, env, token);
        free(str);
        execute_command(env, export, token);
        free_struct_store(token);
    }
    // free_struct(env); // прописать перед всеми выходами
    return (0);
}
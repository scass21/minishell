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

static char *get_second_tmp(char *str, int j, int i, t_env *env)
{
    char *tmp;
    int len;
    int flag;
    t_env *p;
    

    tmp = ft_substr(str, j + 1, i - j - 1);
    if (!tmp)
        ft_error(1);
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
    if (!str[*i] || str[*i] == '\'' || str[*i] == '\"' || str[*i] == '?')
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


t_store *get_last(t_store *token) 
{
    if (token == NULL) {
        return NULL;
    }
    while (token->next) {
        token = token->next;
    }
    return (token);
}

t_store *add_node_token(t_store *token, char *str, int i)
{
    t_store *temp;
    t_store *p;

    p = get_last(token);
    temp = (t_store *)malloc(sizeof(t_store));
    if (!temp)
        ft_error(1);
    temp->word = ft_substr(str, 0, i);
    temp->next = NULL;
    p->next = temp;
    return (p);
}

void push_start(t_store **token, char *join)
{
    t_store *p;
    // t_store *tmp;

    p = (t_store *)malloc(sizeof(t_store));
    if (!p)
        ft_error(1);
    // tmp = (*token);
    // tmp->next = (*token)->word;

    p->word = ft_strdup(join);
    p->next = *token;
    // tmp->next = p;
    // p = tmp;
    // в next 2-го записать 1й элемент
    // в next 1-го записать tmp
    *token = p;
}

char *join_token(char *str, t_env *env, t_store **token, int flag)
{
    char *tmp;
    char *join;
    int i;

    i = 0;
    join = NULL;
    while(*str == ' ' || *str == '\t')
        str++;
    while(str[i] && str[i] != '>' && str[i] != '<' && str[i] != '|')
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
    join = ft_substr(str, 0, i);
    if (flag == 1)
        (*token)->word = ft_strjoin((*token)->word, join);
    if (flag == 0)
        push_start(token, join); 
    free(join);
    tmp = ft_substr(str, i, ft_strlen(str) - i);
    if (!tmp)
        return (NULL);
    return (tmp);
}

static int parser(char *str, t_env *env, t_store **token)
{
    int i;
    int flag;

    i = 0;
    flag = 0;
    if (!str || *str == '\4' || *str == EOF)
    {
        printf("\033[Aminishell$ exit\n");
        free_exit(*token, env, 0);
    }
    while(*str == ' ' || *str == '\t')
        str++;
    while(str[i] && str[i] != '|' && str[i] != '>' && str[i] != '<')
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

        i++;
    }
    if (i != 0)
    {
        flag = 1;
        (*token)->word = ft_substr(str, 0, i);
        if (!(*token)->word)
            ft_error(1);
        str = ft_substr(str, i, ft_strlen(str));
        if (!str)
            ft_error(1);
    }
    i = 0;
    while (str[i])
    {
        if (str[i] == '>' || str[i] == '<')
        {
            str = process_redirect(str, env, *token);
            if (!str)
                return (-1);
            i = -1;
        }
        if (str[i] == '|')
        {
            str = process_pipe(str, env, *token);
            if (!str)
                return (-1);
            i = -1;
        }
        if (str[i] != '>' && str[i] != '<' && str[i] != ' ' && str[i] != '|')
        {
            str = join_token(str, env, token, flag);
            if (!str)
                return (-1);
            i = -1;
        }
        i++;
    }

    t_store *p;

    p = (*token);
    while(p)
    {
        printf("%s\n", p->word);
        p = p->next;
    }

    return (0);
}

static void init_struct_store(t_store *token)
{
    
    token->word = NULL;
    token->next = NULL;
}

char **split_token_word(char *word)
{
    char **argv;
    int i;
    int count_word;

    i = 0;
    count_word = 0;
    while(word[i])
    {
        if (word[i] == '|')
		{
            word[i] = ' ';
            i++;
        }
        if (word[i] == ' ')
        {
            while(word[i] == ' ')
                i++;
            count_word++;
        }
        i++;
    }
    count_word++;
    argv = ft_split(word, ' ');
    if (!argv)
        ft_error(1);
    // i = 0;
    // while(argv[i])
    // {
    //     printf("%s\n", argv[i]);
    //     i++;
    // }
    return (argv);

}

int count_argument(char **argv)
{
    int i;

    i = 0;
    while(argv[i])
        i++;
    return (i);
}

int check_redirect(t_store *token, t_env *env)
{
    t_store *p;

    p = token;
    while(p)
    {
        if (!ft_strncmp(p->word, ">", 1) || !ft_strncmp(p->word, "<", 1))
        {
            if (!our_redirect(p->word, env, token))
                return (0);
        }
        p = p->next;
    }
    return (1);
}


int     command_handler(t_env *env, t_env *export, t_store *token, char **envp)
{
    int count;
    char **argv;

    if (strncmp(token->word, ">", 1) && strncmp(token->word, "<", 1))
    {
        argv = split_token_word(token->word);
        count = count_argument(argv);
    }
    else
        argv = NULL;
    if (argv)
    {
        
        if (ft_strncmp(argv[0], "echo", 4) == 0)
            our_echo(argv);
        else if (ft_strncmp(argv[0], "pwd", 3) == 0)
            our_pwd(argv);
        else if (ft_strncmp(argv[0], "cd", 2) == 0)
            our_cd(count, argv, env, export);
        else if (ft_strncmp(argv[0], "export", 6) == 0)
            our_export(count, env, export, argv);
        else if (ft_strncmp(argv[0], "unset", 5) == 0)
            our_unset(env, export, argv, count);
        else if (ft_strncmp(argv[0], "env", 3) == 0)
             our_env(env, argv, count);
        else if (ft_strcmp(argv[0],  "$?") == 0)
        {
            ft_putstr_fd("minishell: ",2);
            ft_putnbr_fd(t_sh.exit_code , 2);
            ft_putstr_fd(": command not found\n",2);
            return (0);
        }
        else if (ft_strcmp(argv[0], "exit") == 0)
            free_exit(token, env, 0);
        else
            exec_bin(argv, env, envp);
        
    }
    t_sh.exit_code = 0;
    return (0);

}

int execute_command(t_env *env, t_env *export, t_store *token, char **envp)
{
    int count;
    char **argv;

    if (!token->word)
        return (0);
    if (!check_redirect(token, env))
        return (0);

    // if (token->next)
    // {
    //     // while(token->next)
    //     // {
    //         // printf("token %s\n", token->word);
    //         pipe_proc(token->word, env, envp, export, token);
    //     //     token = token->next;
    //     // }   
    // }
    // else
        command_handler(env, export, token, envp);
    return (0);
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
    t_sh.fd = STDOUT_FILENO;
    t_sh.fd2 = STDIN_FILENO;
    t_sh.pipe_flag = 0;
}

static void init_struct_env(t_env *env)
{
    env->key = NULL;
    env->value = NULL;
    env->next = NULL;
}

static void free_struct_store(t_store *token)
{
    t_store *p;

    while (token != NULL)
    {
        p = token;
        token = token->next;
        if (p)
            free(p);
    }
}

void setup_term(void)
{
    struct termios mini;
    tcgetattr(0, &mini);
    mini.c_lflag &= ~ECHOCTL;
    tcsetattr(0, TCSANOW, &mini);
}

// void init_all_struct(t_all *all)
// {
//     t_env *env;

// }


int main(int argv, char **argc, char **envp)
{
    // t_all *all;

    // init_struct_all(all);
    
    char *str;
    t_store *token;
    t_env *env;
    t_env *export;

    int fd_in_old;
    int fd_out_old;
    
    init_mini();
    setup_term();
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
    //rl_catch_signals = 0;            //forbiden?
    signal(SIGINT, (void *) our_sig_proc);
	signal(SIGQUIT, (void *)our_sig_proc);
    while (1)
    {
        t_sh.pipe_flag = 0;
        fd_in_old = dup(STDIN_FILENO);
        fd_out_old = dup(STDOUT_FILENO);
        token = (t_store *)malloc(sizeof(t_store));
        if (!token)
            ft_error(1);
        str = NULL;
        init_struct_store(token);
       
        str = readline("minishell$ ");
        // if (!str || *str == '\4' || *str == EOF)
        // {
        //     printf("exit\n");
        //     free_exit(token, env, 0);
        // }
        add_history(str);
        if (parser(str, env, &token) != -1)
            execute_command(env, export, token, envp);
        free(str);
        free_struct_store(token);
        dup2(fd_out_old, 1);
        dup2(fd_in_old, 0);
    }
    return (0);
}
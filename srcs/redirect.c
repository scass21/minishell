#include "minishell.h"

static int	ft_free(char *str)
{
	if (str)
		free(str);
	str = NULL;
	return (0);
}

static void init_struct_store(t_store *token)
{
    
    token->word = NULL;
    token->next = NULL;
}

static int fill_struct_node(t_store *node, char *str, t_env *env)
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
            if (str[i++] == '?')
                printf("%d: command not found", t_sh.exit_code);
            str = process_dollar(str, &i, env);
            if (!str)
                return (-1);
        }
        if (str[i] == ' ' || str[i] == '\t')
        {
            if (!node->word)
            {
                node->word = ft_substr(str, 0, i);
                if (!node->word)
                    ft_error(1);
            }
            else
                node = add_node_token(node, str, i);
            while(str[i] == ' ' || str[i] == '\t')
                i++;
            if (i == ft_strlen(str))
                return (0);
            str = ft_substr(str, i, ft_strlen(str) - i);
            i = -1;
        }
        i++;
    }
    if (!node->word)
    {
        node->word = ft_strdup(str);
        if (!node->word)
            ft_error(1);
    }
    else
        node = add_node_token(node, str, ft_strlen(str));
    return(0);
}

static char *cat_str(t_store *node)
{
    t_store *p;
    char *str;
    char *tmp;

    p = node;
    p = p->next;
    str = NULL;
    tmp = NULL;
    if (!p)
        return ("\0");
    else
    {
        while(p)
        {
            if (!str)
                tmp = ft_strdup(p->word);
            else
                tmp = ft_strjoin(str, p->word);
            str = ft_strjoin(tmp, " ");
            p = p->next;
        }
    }
    return(str);
}

char *process_redirect(char *str, t_env *env)
{
    char *tmp;
    char *filename;
    t_store *node;

    int r = 0;
    char *buf;


    tmp = NULL;
    filename = NULL;
    buf = (char *)malloc(sizeof(char));
    node = (t_store *)malloc(sizeof(t_store));
    if (!node)
        ft_error(1);
    init_struct_store(node);
    if (*str == '>')
    {
        str++;
        if (*str != '>')
        {
            fill_struct_node(node, str, env);
            filename = ft_strdup(node->word);
            if (!filename)
                ft_error(1);
            filename = process_value(filename, env);
            t_sh.fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (t_sh.fd == -1)
                printf("%s\n", strerror(errno));
                printf("fd1: %i\n", t_sh.fd);
            tmp = cat_str(node);
            free(filename);
            // free_struct_store(node);
        }
        if (*str == '>')
        {
            str++;
            fill_struct_node(node, str, env);
            filename = ft_strdup(node->word);
            if (!filename)
                ft_error(1);
            t_sh.fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (t_sh.fd == -1)
                printf("%s\n", strerror(errno));
            tmp = cat_str(node);
            free(filename);
            // free_struct_store(node);
        }
    }
    if (*str == '<')
    {
        str++;
        if (*str != '<')
        {
            fill_struct_node(node, str, env);
            filename = ft_strdup(node->word);
            if (!filename)
                ft_error(1);
            t_sh.fd2 = open(filename, O_RDONLY, 0644);
            if (t_sh.fd2 == -1)
                printf("%s\n", strerror(errno));
            tmp = cat_str(node);
            free(filename);
            // free_struct_store(node);
        }
        if (*str == '<')
        {
            str++;
            fill_struct_node(node, str, env);
            filename = ft_strdup(node->word);
            filename = ft_strjoin(filename, "\n");
            if (!filename)
                ft_error(1);
            tmp = cat_str(node);
            r = read(t_sh.fd2, buf, BUFFER_SIZE);
            if (r == -1)
                ft_error(2);
            while(r > 0)
            {
                if (ft_strcmp(buf, filename) == 0)
                    break;
                ft_free(buf);
                buf = (char *)malloc(sizeof(char));
                r = read(t_sh.fd2, buf, BUFFER_SIZE);
            }
            ft_free(buf);
            // // free_struct_store(node);
        }
    }

    return (tmp);
}

void our_redirect(t_env *env, t_env *export, t_store *token, char **envp)
{
    pid_t	pid;
	int		status;

    pid = fork();
	signal(SIGINT, our_sig_proc);
	signal(SIGQUIT, our_sig_proc);
	if (pid == 0)
	{
        // printf("fd2: %i\n", t_sh.fd);
        t_sh.fork_status = 1;
        dup2(t_sh.fd, STDOUT_FILENO);
        dup2(t_sh.fd2, STDIN_FILENO);
        execute_command(env, export, token, envp);
        close(t_sh.fd);
        close(t_sh.fd2);
        t_sh.fork_status = 0;
		exit(0);
    }
    else if (pid < 0)
	{
		printf("%s\n", strerror(errno));
		exit(1);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			t_sh.exit_code = WEXITSTATUS(status);
	}
}
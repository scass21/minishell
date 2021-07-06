#include "minishell.h"


char *cut_redirect(char *str, t_env *env, t_store *token, int i)
{
    char *tmp;

    while(str[i] == ' ' || str[i] == '\t')
        i++;
    if (!str[i])
    {
        ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
        return (NULL);
    }
    while (str[i] && str[i] != ' ' && str[i] != '>' && str[i] != '<' && str[i] != '|')
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
        token->word = ft_substr(str, 0, i);
        if (!token->word)
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

char *process_redirect(char *str, t_env *env, t_store *token)
{
    char *tmp;
    int i;

    tmp = NULL;
    i = 0;
    while(*str == ' ' || *str == '\t')
        str++;
    if (str[i] == '>')
    {
        i++;
        if (!str[i])
        {
            ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
            return (NULL);
        }
        if (str[i] != '>')
        {
            tmp = cut_redirect(str, env, token, 1);
            if (!tmp)
                return (NULL);
            return (tmp);
        }
        if (str[i] == '>')
        {
            tmp = cut_redirect(str, env, token, 2);
            if (!tmp)
                return (NULL);
            return (tmp);
        }
    }
    if (str[i] == '<')
    {
        i++;
        if (!str[i])
        {
            ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
            return (NULL);
        }
        if (str[i] != '<')
        {
            tmp = cut_redirect(str, env, token, 1);
            if (!tmp)
                return (NULL);
            return (tmp);
        }
        if (*str == '<')
        {
            tmp = cut_redirect(str, env, token, 2);
            if (!tmp)
                return (NULL);
            return (tmp);
        }
    }
    // ft_free(str);
    return (tmp);
}
    
int our_redirect(char *word, t_env *env, t_store *token)
{
    int i;
    char *filename;
    int fd_in;
    int fd_out;

    // char *buf;
    // int r;

    // r = 0;
    // buf = (char *)malloc(sizeof(char));
    // if (!buf)
    //     ft_error(1);


    i = 0;
    fd_in = 0;
    fd_out = 1;
    filename = NULL;
    if (word[i] == '>' && word[i + 1] != '>')
    {
        word++;
        while(*word == ' ' || *word == '\t')
            word++;
        filename = ft_substr(word, 0, ft_strlen(word));
        if (!filename)
            ft_error(1);
        filename = process_value(filename, env);
        fd_out = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd_out == -1)
        {
            printf("%s\n", strerror(errno));
            return (0);
        }
        // printf("filename: %s\n", filename);
    }
    else if (word[i] == '>' && word[i + 1] == '>')
    {
        word++;
        word++;
        while(*word == ' ' || *word == '\t')
            word++;
        filename = ft_substr(word, 0, ft_strlen(word));
        if (!filename)
            ft_error(1);
        filename = process_value(filename, env);
        fd_out = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd_in == -1)
        {
            printf("%s\n", strerror(errno));
            return (0);
        }
    }
    else if (word[i] == '<' && word[i + 1] != '<')
    {
        word++;
        while(*word == ' ' || *word == '\t')
            word++;
        filename = ft_substr(word, 0, ft_strlen(word));
        if (!filename)
            ft_error(1);
        filename = process_value(filename, env);
        fd_in = open(filename, O_RDONLY, 0644);
        if (fd_in == -1)
        {
            printf("%s\n", strerror(errno));
            return (0);
        }
    }
    else if (word[i] == '<' && word[i + 1] == '<')
    {
        word++;
        word++;
        while(*word == ' ' || *word == '\t')
            word++;
        filename = ft_substr(word, 0, ft_strlen(word));
        if (!filename)
            ft_error(1);
        fd_out = open(filename, O_RDWR | O_CREAT | O_EXCL, 0644);
        if (fd_out == -1)
        {
            printf("%s\n", strerror(errno));
            return (0);
        }
    }
    dup2(fd_out, 1);
    dup2(fd_in, 0);
    return (1);
}
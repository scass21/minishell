#include "minishell.h"

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
    // free(str);
    return(tmp);
}

static char *process_slash(char *str, int *i)
{
    char *tmp;
    int j;
    int k;

    tmp = NULL;
    j = 0;
    k = 0;
    tmp = (char *)malloc(sizeof(char) * ft_strlen(str));
    if (!tmp)
        ft_error(1);
    while(str[j] && (j != *i))
    {
        tmp[k] = str[j];
        k++;
        j++;
    }
    j = *i + 1;
    if (!str[j])
    {
        free(tmp);
        return("minishell: missing slash.");
    }
    while(str[j])
    {
        tmp[k] = str[j];
        j++;
        k++;
    }
    tmp[k] = '\0';
    // free(str);
    return (tmp);
}

static char *double_quotes(char *str, int *i)
{
    int j;
    char *tmp;

    j = *i;
    tmp = NULL;
    (*i)++;
    while(str[*i])
    {
        if (str[*i] == '\\' && str[*i + 1] == '\\')
            str = process_slash(str, i);
        if (str[*i] == '\\' && str[*i + 1] == '\"')
        {
            str = process_slash(str, i);
            (*i)++;
        }
        if (str[*i] == '\\' && str[*i + 1] == '$')
            str = process_slash(str, i);
        // if (str[*i] == '$')
        // {
        //     str = process_dollar(str, i);
        // }
        if (str[*i] == '\"')
            break;
        (*i)++;
    }
    if (*i == ft_strlen(str))
        return("minishell: missing second quote.");
    tmp = copy_str_without_quotes(str, j, *i);
    // free(str);
    return(tmp);
}

static char *single_quotes(char *str, int *i)
{
    int j;
    int flag;
    char *tmp;

    j = *i;
    flag = 0;
    tmp = NULL;
    (*i)++;
    while(str[*i])
    {
        if (str[*i] == '\'')
            break;
        (*i)++;
    }
    if (*i == ft_strlen(str))
        return("minishell: missing second quote.");
    tmp = copy_str_without_quotes(str, j, *i);
    // free(str);
    return(tmp);
}

static int parser(char *str)
{
    // "" '' \ $ ; | > >> <

    int i;

    i = 0;
    while(str[i])
    {
        if (str[i] == '\'')
        {
            str = single_quotes(str, &i);
            if (!str)
                return(-1);
            i = i - 2;
        }
        if (str[i] == '\"')
        {
            str = double_quotes(str, &i);
            if (!str)
                return(-1);
            i = i - 2;
        }
        if (str[i] == '\\')
        {
            str = process_slash(str, &i);
            if (!str)
                return (-1);
        }
        i++;
    }
    printf("%s\n", str);
    return (0);
}

int main(int argv, char **argc, char **env)
{
    char *str;

    str = NULL;
    while (1)
    {
        str = readline("minishell$ ");
        // printf("str: %s\n", str);
        if (parser(str) == -1)
        {
            //ТУТ НАДО ВСЕ ПОЧИСТИТЬ!!!
            return(-1);
        }
        free(str);
    }
    return (0);

}
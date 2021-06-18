#include "minishell.h"

static void add_new_env(t_store *env, t_store *export, char *key, char *value)
{
    char *str;

    str = ft_strjoin(key, value);
    env = add_node_env(env, str);
    export = add_node_env(export, str);
}

static int check_export_argument(char *arg)
{
    int i;
    int k;
    char *str;

    i = 1;
    k = 0;
    str = NULL;
    if (!ft_isalpha(arg[0]) && arg[0] != '_')
        return(-1);
    if (ft_strchr(arg, '='))
    {
        while(arg[k])
        {
            if (arg[k] == '=')
                break;
            k++;
        }
        str = ft_substr(arg, 0, i);
    }
    else
        str = arg;
    while(str[i])
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (-1);
        i++;
    }
    return(0);
}

static char *process_value(char *str, int i, t_store *env)
{
    char *arg;
    int k;

    arg = ft_substr(str, i + 1, ft_strlen(str) - i);
    if (!arg)
        ft_error(1);
    k = 0;
    while(arg[k])
    {
        if (arg[k] == '\'')
        {
            arg = single_quotes(arg, &k);
            k = k - 2;
        }
        if (arg[k] == '\"')
        {
            arg = double_quotes(arg, &k, env);
            k = k - 2;
        }
        if (arg[k] == '$')
            arg = process_dollar(arg, &k, env);
        k++;
    }
    return (arg);
}

static void sort_struct(t_store *env)
{
    t_store *tmp;
    t_store *min;
    t_store *r;
    char *str;

    tmp = env;
    while(tmp)
    {
        min = tmp;
        r = tmp->next;
        while(r)
        {
            if (strncmp(min->word, r->word, 1) > 0)
                min = r;
            r = r->next;
        }
        str = tmp->word;
        tmp->word = min->word;
        min->word = str;
        tmp = tmp->next;
    }
}

static void print_declare_export(t_store *export)
{
    t_store *tmp;

    sort_struct(export);
    tmp = export;
    while(tmp != NULL)
    {
        printf("declare -x %s\n", tmp->word);
        tmp = tmp->next;
    }
}

// static void fill_struct_export(t_store *export, t_store *env)
// {
//     t_store *temp_env;

//     temp_env = env;
//     while(temp_env)
//     {
//         if (!export->word)
//         {
//             export->word = ft_strdup(temp_env->word);
//             if (!export->word)
//                 ft_error(1);
//         }
//         else
//             export = add_node_env(export, temp_env->word);
//         temp_env = temp_env->next;
//     }

//     // t_store *p;
//     // p = export;
//     // while(p != NULL)
//     // {
//     //     printf("%s\n", p->word);
//     //     p = p->next;
//     // }
    
// }

static void add_env_and_export(char *str, t_store *env, t_store *export)
{
    int i;
    char *key;
    char *value;

    i = 0;
    key = NULL;
    while(str[i])
    {
        if (str[i] == '=')
            break;
        i++;
    }
    key = ft_substr(str, 0, i + 1);
    if (!key)
        ft_error(1);
    value = process_value(str, i, env);
    add_new_env(env, export, key, value);
}

static int process_export(t_store *next_token, t_store *env, t_store *export)
{
    while(next_token)
    {
        if (check_export_argument(next_token->word) == -1)
        {
            printf("minishell: export: `%s': not a valid identifier\n", next_token->word);
            return (0);
        }
        if (ft_strchr(next_token->word, '='))
            add_env_and_export(next_token->word, env, export);
        else
            export = add_node_env(export, next_token->word);
        next_token = next_token->next;
    }


    return (0);
}

int	our_export(t_store *env, t_store * export, t_store *token)
{
    t_store *next_token;

    next_token = token->next;
    if (!next_token)
    {
        print_declare_export(export);
        return (0);
    }
    if (process_export(next_token, env, export) == 0)
        return (0);
    return (0);
}
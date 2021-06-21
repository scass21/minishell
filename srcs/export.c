#include "minishell.h"

int check_export_argument(char *arg)
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

static int check_repeat(char *key, char *value, t_env *env)
{
    t_env *p;
    int flag;

    p = env;
    flag = 0;
    while(p)
    {
        if (ft_strcmp(key, p->key) == 0)
        {
            free(p->key);
            p->key = ft_strdup(key);
            if (value)
            {
                free(p->value);
                p->value = ft_strdup(value);
            }
            flag = 1;
        }
        p = p->next;
    }
    return(flag);
}

static char *process_value(char *val, t_env *env)
{
    int i;

    i = 0;
    while(val[i])
    {
        if (val[i] == '\'')
        {
            val = single_quotes(val, &i);
            i = i - 2;
        }
        if (val[i] == '\"')
        {
            val = double_quotes(val, &i, env);
            i = i - 2;
        }
        if (val[i] == '$')
            val = process_dollar(val, &i, env);
        i++;
    }
    return (val);
}

static void add_env_and_export(char *key, char *value, t_env *env, t_env *export)
{
    char *val;

    val = NULL;
    if (value)
        val = process_value(value, env);
    if (check_repeat(key, val, env) == 0)
        add_node_env(env, key, val);

    if (check_repeat(key, val, export) == 0)
        add_node_env(export, key, val);
}

char *get_key(char *token)
{
    char *key;
    int i;

    key = NULL;
    i = 0;
    while(token[i])
    {
        if (token[i] == '=')
            break;
        i++;
    }
    key = ft_substr(token, 0, i);
    if (!key)
        ft_error(1);
    return(key);
}

char *get_value(char *token)
{
    char *value;
    int i;
    int len;

    value = NULL;
    i = 0;
    len = ft_strlen(token);
    while(token[i])
    {
        if (token[i] == '=')
            break;
        i++;
    }
    if (i != len - 1)
    {
        value = ft_substr(token, i + 1, len - i);
        if (!value)
            ft_error(1);
    }
    return(value);
}

static int process_export(t_store *next_token, t_env *env, t_env *export)
{
    char *key;
    char *value;

    key = NULL;
    value = NULL;
    while(next_token)
    {
        if (check_export_argument(next_token->word) == -1)
        {
            printf("minishell: export: `%s': not a valid identifier\n", next_token->word);
            return (0);
        }
        if (ft_strchr(next_token->word, '='))
        {
            key = get_key(next_token->word);
            value = get_value(next_token->word);
            add_env_and_export(key, value, env, export);
        }
        else
        {
            key = ft_strdup(next_token->word);
            if (check_repeat(key, value, export) == 0)
                export = add_node_env(export, key, value);
        }
        next_token = next_token->next;
    }


    return (0);
}

static void sort_struct(t_env *export)
{
    t_env *tmp;
    t_env *min;
    t_env *r;
    char *key;
    char *value;
    int i;

    tmp = export;
    while(tmp)
    {
        min = tmp;
        r = tmp->next;
        while(r)
        {
            i = 1;
            while(i <= ft_strlen(min->key))
            {
                if (strncmp(min->key, r->key, i) > 0)
                {
                    min = r;
                    break;
                }
                i++;
            }
            r = r->next;
        }
        key = tmp->key;
        value = tmp->value;
        tmp->key = min->key;
        tmp->value = min->value;
        min->key = key;
        min->value = value;
        tmp = tmp->next;
    }
}

static void print_declare_export(t_env *export)
{
    t_env *tmp;

    sort_struct(export);
    tmp = export;
    while(tmp != NULL)
    {
        printf("declare -x %s", tmp->key);
        if (tmp->value)
            printf("=\"%s\"\n", tmp->value);
        else
            printf("\n");
        tmp = tmp->next;
    }
}

int	our_export(t_env *env, t_env * export, t_store *token)
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
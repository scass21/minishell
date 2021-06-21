#include "minishell.h"

t_env *delete_node_env(t_env *lst, t_env *root)
{
    t_env *temp;

    temp = root;
    while(temp->next != lst)
        temp = temp->next;
    temp->next = lst->next;
    return (temp);
}

static int search_key(char *key, t_env *env)
{
    t_env *p;
    int flag;

    p = env;
    flag = 0;
    while(p)
    {
        if (ft_strcmp(key, p->key) == 0)
        {
            p = delete_node_env(p, env);
            flag = 1;
        }
        p = p->next;
    }
    return (flag);
}

int our_unset(t_env *env, t_env *export, t_store *token)
{
    t_store *token2;

    token2 = token->next;

    while(token2)
    {
        if (check_export_argument(token2->word) == -1)
        {
            printf("minishell: unset: `%s': not a valid identifier\n", token2->word);
            return (0);
        }
        search_key(token2->word, env);
        search_key(token2->word, export);
        token2 = token2->next;
    }
    return (0);
}
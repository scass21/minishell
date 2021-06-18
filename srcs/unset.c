#include "minishell.h"

t_store *delete_node(t_store *lst, t_store *root)
{
    t_store *temp;

    temp = root;
    while(temp->next != lst)
        temp = temp->next;
    temp->next = lst->next;
    free(lst);
    return (temp);
}

int our_unset(t_store *env, t_store *export, t_store *token)
{
    t_store *env2;
    t_store *export2;
    t_store *next;

    env2 = env;
    export2 = export;
    next = token->next;

    while (next)
    {
        while(env2)
        {
            if (ft_strcmp(env2->word, next->word) == 0)
                env = delete_node(env2, env);
            env2 = env2->next;
        }
        next = next->next;
    }
    return (0);
}
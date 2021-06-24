#include "minishell.h"

void	our_sig_proc(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
		if (t_sh.fork_status == 1)
			t_sh.exit_code = 1;				// value for $?
		else
			t_sh.exit_code = 130;
	}
	else if (sig == SIGQUIT)
	{
		t_sh.exit_code = 131;
	}
}

#include "minishell.h"

void	our_sig_proc(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\nminishell$ ", 12);
		if (t_sh.fork_status == 1)
			t_sh.exit_code = 1;				// value for $?
		else
			t_sh.exit_code = 130;
	}
	else if (sig == SIGQUIT)
	{
		write(1, "\nminishell$ ", 12);
		t_sh.exit_code = 131;

	}
}

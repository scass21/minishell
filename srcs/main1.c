#include "minishell.h"

void	init_mini(t_shell *t_sh)
{
	t_sh->exit_code = 0;
	t_sh->fork_status = 0;
	t_sh->path = NULL;
	// t_sh->str = NULL;
	ft_bzero(t_sh->str, ft_strlen(t_sh->str));
	t_sh->col = 0;
	t_sh->row = 0;
	t_sh->win_col = 0;
	t_sh->win_row = 0;

}

void	term_cap(t_shell *t_sh)
{
	char	*termname;

	// setupterm(0, STDOUT_FILENO, 0); 
	tcgetattr(0, &t_sh->term);			//запрашивает настройки терминала из стрк термиос
	// t_sh->term.c_lflag &= ~(ECHO);		
	t_sh->term.c_lflag &= ~(ICANON);
	t_sh->term.c_cc[VMIN] = 1;	// посимвольное чтение (от 1 знака)
	t_sh->term.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &t_sh->term); // устанавливает настройки, немедленное вступление изменений в силу
	termname = getenv("TERM");		// узнать какой терминал
	tgetent(0, termname);			//подгружает базу терминала 
	t_sh->cm = tgetstr("cm", NULL);  // курсор перемещается на полосу %1 и колонку %2 (на экране)
	t_sh->ce = tgetstr("ce", NULL);		// очистка до конца линии
	t_sh->dc = tgetstr("dc", NULL);  //удаление 1 символа
}


void	our_sig_proc(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "^C\nminishell: ", 14);
		if (t_sh.fork_status == 0)
			t_sh.exit_code = 1;
		else
			t_sh.exit_code = 130;
	}
	else if (sig == SIGQUIT)
	{
		write(1, "^C\nminishell: ", 12);
		t_sh.exit_code = 131;

	}
}

void	get_cursor()
{
	int		i;
	int		next_val;
	int		line_read;
	char	line[255];

	struct winsize	win;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &win); // terminal-size
	t_sh.win_col = win.ws_col;
	t_sh.win_row = win.ws_row;

	i = 0;
	next_val = 0;

	write(0, "\033[6n", 4);			// cursor-position
	if(line_read = read(0, line, 254) < 0)
		line_read = 0;
	line[line_read] = '\0';

	while (line[++i])
	{
		if (ft_isdigit(line[i]))
		{
			if (next_val == 0)
				t_sh.row = ft_atoi(&line[i]) - 1;
			else
			{
				t_sh.col = ft_atoi(&line[i]) - 1;
				break ;
			}
			next_val++;
			i += ft_len(t_sh.row) - 1;
		}
	}
}

int	ft_buttons(t_shell *t_sh)
{
	char str[2048];

	get_cursor();
	while(read(0, str, sizeof(str)) > 0)
	{
		if (!strcmp(str,"\n"))
		{
			write(1, "\n", 1);
				return (0);
		}
		if (!strcmp(str, "\e[b"))
		{
			printf("bs\n");
		}
		if (!strcmp(str,"\4"))		// ctrl-D ASCII
		{
			printf("exit\n");
			exit(0);
		}
		if (KEY_UP)  // key up (!strcmp(str, "\e[A"))
		{
			tputs(restore_cursor, 1, putchar);
			// tputs(tgoto("ed", 0, 0), 1, putchar);
			printf("previous\n");
		}
		if (!strcmp(str, "\e[B"))  // key down
		{
			tputs(restore_cursor, 1, putchar);
			tputs(tigetstr("ed"), 1, putchar);
			printf("next\n");
		}
		if (KEY_RIGHT)	//key right (!strcmp(str, "\e[C"))
		{ 
			tputs(cursor_right, 1, putchar);
			printf("right\n");
		}
		if (!strcmp(str, "\e[D"))	//key left
		{
			// write(1, "left", 4);
			// tputs(tgoto(tgetstr("le", 0), 5, 5), 0, 0);
			// tputs(tgoto(cursor_left, 0, 0), 1, putchar);
			
		}
		else
			write(1, str, 1);
		ft_bzero(str, ft_strlen(str));
	}
	return 0;
		
}

int main(int argc, char **argv, char **env)
{
	t_shell		t_sh;

	signal(SIGINT, our_sig_proc);
	signal(SIGQUIT, our_sig_proc);
	init_mini(&t_sh);
	// tcgetattr(0, &t_sh.term);
	while (1)
	{
		write(1, "minishell: ", 11);
		term_cap(&t_sh);
		while (1)
		{
			write(1, "> ", 2);
			if(ft_buttons(&t_sh) == 0)
			{
				printf("ok\n");
				break;
			}
			else
				continue;
			
		
		}
		// tcsetattr(0, TCSANOW, &t_sh.ret_term);

	}
	return (0);
}

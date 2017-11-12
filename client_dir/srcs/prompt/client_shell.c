/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_shell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wescande <wescande@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/26 18:16:09 by wescande          #+#    #+#             */
/*   Updated: 2017/11/07 20:04:11 by wescande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client.h>

void    client_histo_del(t_client *cli)
{
    (void)cli;
//TODO
}

void	show_histo(t_prompt *cmd, char c, t_ld **histo, int *first)
{
	if (!histo || !*histo)
		return ;
	delete_cmd(cmd);
	if (c == 'A')
	{
		if (*first)
		*first = 0;
		else if ((*histo)->next)
		*histo = (*histo)->next;
	}
	else if (c == 'B')
	{
		if ((*histo)->prev)
			*histo = (*histo)->prev;
		else if ((*first = 1))
			ft_bzero(cmd->line, BUFF_SIZE);
	}
	if (!*first)
		ft_strcpy(cmd->line, (*histo)->content);
	ft_putstr(cmd->line);
	cmd->len = ft_strlen(cmd->line);
	cmd->pos = cmd->len;
}

void    client_histo_add(t_client *cli, char *str)
{
	if (!ft_strlen(str))
		return ;
	if (!cli->prompt.histo)
		ft_ld_pushfront(&cli->prompt.histo, ft_strdup(str));
	else if (ft_strcmp(cli->prompt.histo->content, str))
		ft_ld_pushfront(&cli->prompt.histo, ft_strdup(str));
    //TODO LX PUSH
}

int     ft_quit(t_client *cli, char **command)
{
    (void)command;
    (void)cli;
    return (0);
}
int     ft_exit(t_client *cli, char **command)
{
    (void)command;
    (void)cli;
    return (1);
}

static const t_stof g_cmd_tab[]=
{
	{"exit", &ft_exit},
	{"quit", &ft_quit},
	{"ls", &client_ls},
	{NULL, NULL},
};

int     client_analyze_command(t_client *cli, char **command_tab)
{
	int      i;

	i = -1;
	while (g_cmd_tab[++i].str)
		if (!ft_strcmp(command_tab[0], g_cmd_tab[i].str))
		{
			cli->prompt.err = 0;
			return (g_cmd_tab[i].f(cli, command_tab));
		}
	cli->prompt.err = 1;
	verbose(cli->flag, MSG_ERROR, "%s: Unknow command", *command_tab);
	return (0);
}

static void		termcap_fleche(t_prompt *cmd, int *read_ret,
	char **buffer, int *first)
{
	if (*read_ret <= 2)
		return ;
	--*read_ret;
	++*buffer;
	if (**buffer != 91)
		return ;
	--*read_ret;
	++*buffer;
	if (**buffer == 'A' || **buffer == 'B')
		show_histo(cmd, **buffer, &cmd->curhisto, first);
	else if (**buffer == 'D' || **buffer == 'C')
		move_one(cmd, **buffer);
	else if (**buffer == 'H' || **buffer == 'F')
		move_lim(cmd, **buffer);
	else if (**buffer == 49 || **buffer == 52)
		move_lim(cmd, *(*buffer++) + 0 * --*read_ret);
	else if (**buffer == '3' && *(*buffer + 1) == '~')
	{
		if (cmd->pos < cmd->len)
		{
			move_one(cmd, 'C');
			delete_one(cmd);
		}
		--*read_ret;
		++*buffer;
	}
}

void			termcap_complet(t_prompt *cmd)
{
	(void) cmd;
	//TODO
}

static int		analyse_buffer(t_prompt *cmd, int read_ret, char *buffer,
	int *first)
{
	while (read_ret)
	{

		if (ft_isprint(*buffer))
			add_one(cmd, *buffer);
		else if (*buffer == 9)
			termcap_complet(cmd);
		else if (*buffer == 27)
			termcap_fleche(cmd, &read_ret, &buffer, first);
		else if (*buffer == 127)
			delete_one(cmd);
		else if (*buffer == 4 && cmd->len == 0)
			return (0);
		else if (*buffer == 10)
			return (1);
		++buffer;
		--read_ret;
	}
	return (-1);
}
int		client_read(t_client *cli, char *command)
{
	char	buffer[1024];
	int		first;
	int		read_ret;
	int		ret;

	ft_bzero(command, BUFF_SIZE);
	first = 1;
	cli->prompt.pos = 0;
	cli->prompt.len = 0;
	cli->prompt.curhisto = ft_ld_front(cli->prompt.histo);
	ret = -1;
	while (ret == -1 && (read_ret = read(0, buffer, 1024)))
		ret = analyse_buffer(&cli->prompt, read_ret, buffer, &first);
	cli->prompt.prompt = 0;
	ft_putchar('\n');
	return (ret);
}

int		client_shell(t_client *cli, struct termios *term)
{
	char	command[BUFF_SIZE];
	char    **cmd_split;
	int     ret;

	cli->prompt.line = command;
	while (client_read(cli, command))
	{
		if (!ft_strlen(command))
		{
			display_prompt(cli);
			continue;
		}
		verbose(cli->flag, MSG_INFO, "command: [%s]", command);
		client_histo_add(cli, command);
		cmd_split = ft_strsplitspa(command);
		if (reinit_term(term))
			return (1);
		ret = client_analyze_command(cli, cmd_split);
		if (catch_term(term))
			return (1);
		ft_tabdel(&cmd_split);
		if (ret)
			break ;
		display_prompt(cli);
	}
	client_histo_del(cli);
	return (0);
}

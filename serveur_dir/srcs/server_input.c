/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wescande <wescande@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 18:31:10 by wescande          #+#    #+#             */
/*   Updated: 2017/11/12 16:02:25 by wescande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server.h>

static int				server_quit(t_server *ser)
{
	verbose(ser->flag, MSG_DEBUG, "Asking to quit", NULL);
	SET(ser->flag, STOP_SERV);
	return (0);
}

static const t_stof		g_func_tab[] =
{
	{"quit", server_quit},
	{NULL, NULL},
};

static int		ft_commandcmp(char *command, char *origin)
{
	int		len;

	len = ft_strlen(origin);
	if (ft_strncmp(command, origin, len))
		return (1);
	return (ft_isspa(command[len]));
}

static int		stdin_input(t_server *ser)
{
	char	buf[BUFF_SIZE];
	const char	*p;
	int		len;
	int		i;

	if (0 >= (len = read(STDIN, buf, BUFF_SIZE - 1)))
		return (server_quit(ser) + error_set(E_CUSTOM_ERR, "read", "shutdown"));
	buf[len] = 0;
	p = buf;
	while (ft_isspa(*p))
		++p;
	if (!*buf)
		return (0);
	verbose(ser->flag, MSG_DEBUG, "Command is [%s]", buf);
	i = -1;
	while (g_func_tab[++i].str)
		if (!ft_commandcmp(buf, g_func_tab[i].str))
			return (g_func_tab[i].f(ser));
	verbose(ser->flag, MSG_WARNING, "%s: Command not recognize.", buf);
	return (0);
}


void		server_input(t_server *ser)
{
	if (FD_ISSET(ser->master_sock, &ser->readfds))
		if (client_new(ser))
			verbose(ser->flag, MSG_ERROR,
					"Failed to create client: %s", g_errmsg);
	if (FD_ISSET(STDIN, &ser->readfds))
		if (stdin_input(ser))
			verbose(ser->flag, MSG_ERROR,
					"Failed to execute command server: %s", g_errmsg);
}

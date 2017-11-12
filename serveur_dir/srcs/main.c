/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wescande <wescande@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/26 12:10:11 by wescande          #+#    #+#             */
/*   Updated: 2017/11/12 17:37:42 by wescande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server.h>

static int		usage(void)
{
	ft_printf("{yel}Usage:\n{pur}"
			"\tserver [-vqD] port\n"
			"\tserver [-vqD] [-p port]\n"
			"{eoc}");
	return (-1);
}

static int		parse_port(char **opt_arg, t_server *s, int n_args)
{
	char	*port;
	int		iport;

	if (!opt_arg || !s)
		return (1);
	if (ft_tablen(opt_arg) < n_args)
		return (verbose(0, MSG_ERROR, "option needs more arguments"));
	port = *opt_arg;
	if (!ft_strisnumeral(port))
		return (verbose(0, MSG_ERROR, "%s: Port must be numeral", port));
	iport = ft_atoi(port);
	if (iport < 0)
		return (verbose(0, MSG_ERROR, "%s: Port must be positive", port));
	if (iport > 65535)
		return (verbose(0, MSG_ERROR, "%s: Port must < 65535", port));
	s->port = (uint16_t)iport;
	verbose(s->flag, MSG_DEBUG, "%s: port is correct: %hu", port, s->port);
	return (0);
}

static const t_cliopts		g_opts[] =
{
	{'D', "debug", DEBUG, 0, NULL, 0},
	{'q', "quiet", QUIET, VERBOSE, NULL, 0},
	{'v', "verbose", VERBOSE, QUIET, NULL, 0},
	{'p', "port", 0, 0, parse_port, 1},
	{0, 0, 0, 0, 0, 0},
};

static int		quit_server(t_server *ser)
{
	t_client	*cli;
	t_client	*tmp;

	LIST_FOR_EACH_ENTRY_SAFE_0(cli, tmp, &ser->client, list);
	while (LIST_FOR_EACH_ENTRY_SAFE_1(cli, tmp, &ser->client, list))
		client_delete(ser, cli);
	close(ser->master_sock);
	//TODO check if complete free ?
	return (0);
}

int				main(int argc, char **av)
{
	t_server			ser;
	int					ret;

	(void)argc;
	ft_bzero(&ser, sizeof(t_server));
	if (cliopts_get(av, g_opts, &ser))
		return (ft_perror(NULL) && usage());
	if (!ser.port && !*ser.av_data)
		return (usage());
	if (!ser.port && parse_port(ser.av_data++, &ser, 1))
		return (usage());
	if (*ser.av_data)
		return (verbose(0, MSG_ERROR, "%s: Too many arguments", *ser.av_data));
	if (initialize_server(&ser))
		return (verbose(0, MSG_ERROR, "Unable to set-up server"));
	verbose(ser.flag, MSG_DEBUG, "Start of loop");
	ret = server_loop(&ser);
	return (quit_server(&ser) || ret);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wescande <wescande@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/26 12:10:11 by wescande          #+#    #+#             */
/*   Updated: 2017/11/07 20:08:03 by wescande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client.h>

static int		usage(void)
{
	printf("{yel}Usage:\n{pur}"
			"\tclient [-vqD] addr port\n"
			"\tclient [-vqD] [-a addr] [-p port]\n"
			"{eoc}");
	return (-1);
}

t_client	*get_cli(uint8_t reinit)
{
	static t_client			data;
	t_prompt				*p;

	if (reinit)
	{
		p = &data.prompt;
		ft_bzero(p->line, BUFF_SIZE);
		p->pos = 0;
		p->len = 0;
		p->err = 0;
		p->prompt = 0;
	}
	return (&data);
}

static int		parse_port(char **opt_arg, t_client *c, int n_args)
{
	char	*port;
	int		iport;

	if (!opt_arg || !c)
		return (1);
	if (ft_tablen(opt_arg) < n_args)
		return (verbose(c->flag, MSG_ERROR, "option needs more arguments", NULL));
	port = *opt_arg;
	if (!ft_strisnumeral(port))
		return (verbose(c->flag, MSG_ERROR, "%s: Port must be numeral", port));
	iport = ft_atoi(port);
	if (iport < 0)
		return (verbose(c->flag, MSG_ERROR, "%s: Port must be positive", port));
	if (iport > 65535)
		return (verbose(c->flag, MSG_ERROR, "%s: Port must < 65535", port));
	c->port = (uint16_t)iport;
	verbose(c->flag, MSG_DEBUG, "%s: port is correct: %hu", port, c->port);
	return (0);
}

static int		parse_addr(char **opt_arg, t_client *cli, int n_args)
{
	char	*addr;

	if (!opt_arg || !cli)
		return (1);
	if (ft_tablen(opt_arg) < n_args)
		return (verbose(cli->flag, MSG_ERROR, "option needs more arguments", NULL));
	addr = *opt_arg;
	if (!ft_strcmp(addr, "localhost"))
		cli->addr = inet_addr("127.0.0.1");
	else
		cli->addr = inet_addr(addr);
	if (cli->addr == INADDR_NONE)
		return (verbose(cli->flag, MSG_ERROR, "%s: Wrong address", addr));
	verbose(cli->flag, MSG_DEBUG, "%s: addr is correct: 0x%x", addr, cli->addr);
	cli->straddr = addr;
	return (0);
}

static const t_cliopts		g_opts[] =
{
	{'D', "debug", DEBUG, 0, NULL, 0},
	{'q', "quiet", QUIET, VERBOSE, NULL, 0},
	{'v', "verbose", VERBOSE, QUIET, NULL, 0},
	{'p', "port", 0, 0, parse_port, 1},
	{'a', "address", 0, 0, parse_addr, 1},
	{0, 0, 0, 0, 0, 0},
};

int		main(int ac, char **av, char **env)
{
	int					ret;
	t_client			*cli;

	(void)ac;
	cli = get_cli(0);
	if (cliopts_get(av, g_opts, cli))
		return (ft_perror(NULL) && usage());
	if (!cli->straddr && parse_addr(cli->av_data++, cli, 1))
		return (usage());
	if (!cli->port && parse_port(cli->av_data++, cli, 1))
		return (usage());
	if (*cli->av_data)
		return (verbose(cli->flag, MSG_ERROR,
			"%s: Too many arguments...", *cli->av_data));
	if (connection(cli))
		return (verbose(cli->flag, MSG_ERROR,
			"%s:%hu: Failed to connect", cli->straddr, cli->port));
	cli->prompt.env = env;
	ret = client_prompt(cli);
	close(cli->socket);
	return (ret);
}
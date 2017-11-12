/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wescande <wescande@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/28 10:44:59 by wescande          #+#    #+#             */
/*   Updated: 2017/11/12 18:31:56 by wescande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server.h>

static int		lx_client_fd_stock(t_lx *head, fd_set *fds)
{
	int			fd_max;
	t_client	*cli;

	fd_max = 0;
	LIST_FOR_EACH_ENTRY_0(cli, head, list);
	while (LIST_FOR_EACH_ENTRY_1(cli, head, list))
	{
		fd_max = ft_max(cli->sock, fd_max);
		FD_SET(cli->sock, fds);
		if (IS_SET(cli->flag, TALK) && cli->chan)
		{
			SET(cli->chan->flag, HAS_MESSAGE);
			cli->chan->speaker = cli;
		}
	}
	return (fd_max);
}

static int		channel_check(t_lx *head, fd_set *fds)
{
	int			fd_max;
	t_channel	*channel;

	fd_max = 0;
	LIST_FOR_EACH_ENTRY_0(channel, head, list);
	while (LIST_FOR_EACH_ENTRY_1(channel, head, list))
		if (IS_SET(channel->flag, HAS_MESSAGE))
			fd_max = lx_client_fd_stock(&channel->client, fds);
	return (fd_max);
}

static void		reset_fdset(t_server *ser)
{
	int			fd_max;

	FD_ZERO(&ser->readfds);
	FD_ZERO(&ser->writefds);
	FD_SET(STDIN, &ser->readfds);
	FD_SET(ser->master_sock, &ser->readfds);
	fd_max = ser->master_sock;
	fd_max = ft_max(fd_max, lx_client_fd_stock(&ser->client, &ser->readfds));
	fd_max = ft_max(fd_max, channel_check(&ser->channel, &ser->writefds));
	fd_max = ft_max(fd_max, channel_check(&ser->priv_channel, &ser->writefds));
	ser->nfds = fd_max + 1;
}

static void		channel_iter(t_server *ser, t_lx *head)
{
	t_channel	*chan;
	t_client	*cli;
	char		buf[CIRC_BUFF_SIZE];

	LIST_FOR_EACH_ENTRY_0(chan, head, list);
	while (LIST_FOR_EACH_ENTRY_1(chan, head, list))
		if (IS_SET(chan->flag, HAS_MESSAGE))
		{
			if (chan->speaker)
				circular_buf_get(&chan->speaker->buf, buf);
			else
				circular_buf_get(&chan->buf, buf);
				LIST_FOR_EACH_ENTRY_0(cli, &chan->client, list);
				while (LIST_FOR_EACH_ENTRY_1(cli, &chan->client, list))
					if (!FD_ISSET(cli->sock, &ser->writefds))
						client_delete(ser, cli);
					else if (cli != chan->speaker)
						ft_putstr_fd(buf, cli->sock);
			chan->speaker = NULL;
			UNSET(chan->flag, HAS_MESSAGE);
		}
}

int				server_loop(t_server *ser)
{
	t_client	*cli;

	while (IS_UNSET(ser->flag, STOP_SERV))
	{
		DG("Return on select");
		reset_fdset(ser);
		if (-1 == select(ser->nfds, &ser->readfds, &ser->writefds, NULL, NULL))
			return (verbose(0, MSG_ERROR, "Select failed.")); //TODO EINTR CHECK
		server_input(ser);
		channel_iter(ser, &ser->channel);
		channel_iter(ser, &ser->priv_channel);
		LIST_FOR_EACH_ENTRY_0(cli, &ser->client, list);
		while (LIST_FOR_EACH_ENTRY_1(cli, &ser->client, list))
			client_entry(ser, cli);
	}
	return (0);
}
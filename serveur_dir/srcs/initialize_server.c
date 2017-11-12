/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_server.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wescande <wescande@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/28 10:40:03 by wescande          #+#    #+#             */
/*   Updated: 2017/11/12 18:08:19 by wescande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server.h>

static int		create_socket(t_server *ser)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in6	sin;
	int					opt;

	if (!(proto = getprotobyname("tcp")))
		return (verbose(0, MSG_ERROR, "getprotobyname failed", NULL));
	if ((sock = socket(PF_INET6, SOCK_STREAM, proto->p_proto)) == -1)
		return (verbose(0, MSG_ERROR, "can't create socket", NULL));
	opt = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt))
			< 0)
		return (verbose(0, MSG_ERROR, "setsockopt failed", NULL));
	ft_bzero(&sin, sizeof(struct sockaddr_in6));
	sin.sin6_family = AF_INET6;
	sin.sin6_port = htons(ser->port);
	sin.sin6_addr = in6addr_any;
	if (bind(sock, (const struct sockaddr *)&sin, sizeof(struct sockaddr_in6)))
		return (verbose(0, MSG_ERROR, "bind failed", NULL));
	if (listen(sock, 42) == -1)
		return (verbose(0, MSG_ERROR, "listen failed", NULL));
	ser->master_sock = sock;
	return (0);
}


int				initialize_server(t_server *ser)
{
	if (create_socket(ser))
		return (1);
	INIT_LIST_HEAD(&ser->priv_channel);
	INIT_LIST_HEAD(&ser->channel);
	INIT_LIST_HEAD(&ser->client);
	verbose(ser->flag, MSG_INFO,
		"Server created on port %hu. Now waiting for connection ...",
		ser->port);
	return (0);
}


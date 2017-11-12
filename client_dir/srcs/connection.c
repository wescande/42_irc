/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wescande <wescande@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/26 17:31:33 by wescande          #+#    #+#             */
/*   Updated: 2017/10/29 15:16:05 by william          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client.h>

int     connection(t_client *cli)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	if (!(proto = getprotobyname("tcp")))
		return (verbose(cli->flag, MSG_ERROR, "getprotobyname as failed", NULL));
	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(cli->port);
	sin.sin_addr.s_addr = cli->addr;
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(struct sockaddr_in)))
		return (verbose(cli->flag, MSG_ERROR, "connect as failed", NULL));
	cli->socket = sock;
	return (0);
}
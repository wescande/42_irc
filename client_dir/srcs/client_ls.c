/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_ls.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wescande <wescande@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/27 14:17:00 by wescande          #+#    #+#             */
/*   Updated: 2017/10/27 15:45:50 by wescande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client.h>

int		client_ls(t_client *cli, char **av)
{
	char	*cmd;
	int		ret;
	char	buf[BUFF_SIZE];

	if (!(cmd = set_rfc_959_cmd("LIST", av)))
		return (verbose(0, MSG_ERROR, "Failed to create command RFC", NULL));
	ret = write(cli->socket, cmd, ft_strlen(cmd));
	free(cmd);
	if (ret == -1)
	{
		return (verbose(0, MSG_ERROR,
			"Failed to send following command to the server:\n\t%s ", cmd));
	}
	if ((cmd = rfc_959_return(cli->flag, cli->socket)))
		return (verbose(0, MSG_ERROR, "LIST: Command as failed:\n\t%s", cmd));
	while ((ret = read(cli->socket, &buf, BUFF_SIZE - 1)))
	{
		if (write(0, buf, ret) == -1)
			return (verbose(0, MSG_ERROR, "Write call as failed", NULL));
		if (ret < BUFF_SIZE - 1)
			break ;
	}
	return (0);
}
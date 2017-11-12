/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wescande <wescande@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/27 14:33:57 by wescande          #+#    #+#             */
/*   Updated: 2017/10/27 16:10:55 by wescande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client.h>

// static char		*rfc_error[] =
// {
// 	"Ressource are busy",
// 	"Request is not valid",
// };

char	*set_rfc_959_cmd(char *cmd, char **av)
{
	char	*rfc_cmd;

	if (av[1])
	{
		cmd = ft_strjoin(cmd, " ");
		rfc_cmd = ft_strjoinf(cmd, ft_strsepjoin_crlf(av + 1, ' '), 2);
		free(cmd);
	}
	else
		rfc_cmd = ft_strjoin(cmd, "\r\n");
	return (rfc_cmd);
}

char	*rfc_959_return(uint64_t flag, int socket)
{
	char	buf[5];
	int		ret;

	if ((ret = read(socket, buf, 4)) != 4)
		return ("Failed to read code return");
	buf[ret] = 0;
	verbose(flag, MSG_INFO, "Serveur return: [%s]", buf);
	if (*buf == '4')
		return ("Ressource are busy");
	if (*buf == '5')
		return ("Request is not valid");
	DG("%c (%d) vs %c (%d)", *buf, *buf, '2', '2');
	if (*buf != '1' && *buf != '2' && *buf != '3')
		return ("Code return is not valid");
	return (NULL);
}
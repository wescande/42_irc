/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wescande <wescande@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/12 18:50:47 by wescande          #+#    #+#             */
/*   Updated: 2017/11/12 19:08:28 by wescande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server.h>

int				nick(t_server *ser, t_client *cli, char **av)
{
	(void)ser;
	if (ft_tablen(av) <= 1)
		return (ERR_SET(-1, E_CUSTOM_ERR, *av, "Missing argument"));
	if (ft_tablen(av) > 2)
		return (ERR_SET(-1, E_CUSTOM_ERR, *av, "Too many arguments"));
	ft_strdel(&cli->nick);
	if (!(cli->nick = ft_strdup(av[1])))
		return (ERR_SET(-1, E_MALLOC));
	return (0);
}

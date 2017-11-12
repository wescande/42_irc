/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   circular_buf_get.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wescande <wescande@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/12 16:40:24 by wescande          #+#    #+#             */
/*   Updated: 2017/11/12 19:13:01 by wescande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server.h>

void				circular_buf_get(t_buf *cir, char *buf)
{
	int		len;

	if (cir->read < cir->write)
	{
		len = cir->write - cir->read;
		ft_memcpy(buf, cir->buf + cir->read, len);
	}
	else
	{
		len = CIRC_BUFF_SIZE - cir->read;
		ft_memcpy(buf, cir->buf + cir->read, len);
		buf += len;
		len = cir->write;
		ft_memcpy(buf, cir, len);
	}
	buf[len] = 0;
	cir->read = cir->write;
}

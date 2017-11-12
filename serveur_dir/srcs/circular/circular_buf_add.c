/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   circular_buf_add.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wescande <wescande@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/12 16:16:21 by wescande          #+#    #+#             */
/*   Updated: 2017/11/12 16:35:06 by wescande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server.h>

static int		buffer_overflow(t_buf *cir, uint16_t len)
{
	if (cir->write < cir->read)
		return (cir->write + len >= cir->read);
	else
		return (cir->write + len >= CIRC_BUFF_SIZE + cir->read);
}

int				circular_buf_add(t_buf *cir, char *buf, uint16_t len)
{
	int		first;

	if (buffer_overflow(cir, len))
		return (error_set(E_CUSTOM_ERR, "Buffer circular", "overflow"));
	if (cir->write + len > CIRC_BUFF_SIZE)
	{
		first = CIRC_BUFF_SIZE - cir->write;
		ft_memcpy(cir + cir->write, buf, first);
		ft_memcpy(cir, buf + first, len - first);
	}
	else
		ft_memcpy(cir + cir->write, buf, len);
	cir->write = (cir->write + len) % CIRC_BUFF_SIZE;
	return (0);
}

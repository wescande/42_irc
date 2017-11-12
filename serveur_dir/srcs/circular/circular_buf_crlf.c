/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   circular_buf_crlf.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wescande <wescande@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/12 16:43:28 by wescande          #+#    #+#             */
/*   Updated: 2017/11/12 16:47:15 by wescande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <server.h>

int				circular_buf_crlf(t_buf *cir)
{
	return ((cir->buf[(CIRC_BUFF_SIZE + cir->write - 1) % CIRC_BUFF_SIZE]
				== '\n')
			&& (cir->buf[(CIRC_BUFF_SIZE + cir->write - 2) % CIRC_BUFF_SIZE]
				== '\r'));
}

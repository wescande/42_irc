/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wescande <wescande@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/26 17:54:22 by wescande          #+#    #+#             */
/*   Updated: 2017/10/26 19:09:31 by wescande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client.h>

int			reinit_term(struct termios *term)
{
	if (!term)
		return (1);
	term->c_lflag |= (ICANON);
	term->c_lflag |= (ECHO);
	if (tcsetattr(0, 0, term) == -1)
        return (verbose(0, MSG_ERROR, "Failed to reset tty.", NULL));
    return (0);
}

int			catch_term(struct termios *term)
{
	if (!term)
		return (1);
	term->c_lflag &= ~(ICANON);
	term->c_lflag &= ~(ECHO);
	if (tcsetattr(0, 0, term) == -1)
	{
        verbose(0, MSG_ERROR, "Failed to catch tty.", NULL);
        reinit_term(term);
        return (1);
    }
    return (0);
}
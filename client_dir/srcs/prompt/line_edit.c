/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_edit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wescande <wescande@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/27 10:56:27 by wescande          #+#    #+#             */
/*   Updated: 2017/11/07 20:03:38 by wescande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client.h>

void	move_one(t_prompt *cmd, char c)
{
	if (c == 'C')
	{
		if (cmd->pos < cmd->len)
		{
			++cmd->pos;
			PUT("nd");
		}
	}
	else if (c == 'D')
		if (cmd->pos)
		{
			--cmd->pos;
			PUT("le");
		}
}

void	move_lim(t_prompt *cmd, char c)
{
	if (c == 'F' || c == 52)
		while (cmd->pos < cmd->len)
		{
			++cmd->pos;
			PUT("nd");
		}
	else if (c == 'H' || c == 49)
		while (cmd->pos)
		{
			--cmd->pos;
			PUT("le");
		}
}

void	add_one(t_prompt *cmd, char c)
{
	int	i;

	if (cmd->len >= BUFF_SIZE - 1)
	{
		ft_putchar('\n');
		verbose(0, MSG_ERROR, "Command is too long", NULL);
		cmd->prompt = 0;
		display_prompt(get_cli(0));
		ft_putstr(cmd->line);
		return ;
	}
	ft_memmove(cmd->line + cmd->pos + 1, cmd->line + cmd->pos,
		cmd->len - cmd->pos);
	cmd->line[cmd->pos] = c;
	ft_putstr(cmd->line + cmd->pos);
	i = cmd->len - cmd->pos;
	while (i--)
		PUT("le");
	++cmd->pos;
	++cmd->len;
}

void	delete_one(t_prompt *cmd)
{
	if (!cmd->len || !cmd->pos)
		return ;
	ft_memcpy(cmd->line + cmd->pos - 1, cmd->line + cmd->pos,
		cmd->len - cmd->pos);
	cmd->line[cmd->len - 1] = 0;
	--cmd->pos;
	--cmd->len;
	PUT("le");
	PUT("dc");
}

void	delete_cmd(t_prompt *cmd)
{
	move_lim(cmd, 'F');
	while (cmd->len)
	{
		--cmd->pos;
		--cmd->len;
		PUT("le");
		PUT("dc");
	}
	ft_bzero(cmd->line, BUFF_SIZE);
}

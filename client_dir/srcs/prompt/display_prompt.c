/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_prompt.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wescande <wescande@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/05 23:20:08 by wescande          #+#    #+#             */
/*   Updated: 2017/11/07 20:03:08 by wescande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client.h>

void	error_prompt(void)
{
	ft_printf("%s%s✗ %s%s {eoc}%s {eoc}", ER_FG, ER_BG, ER_FGN, PR_BG, PR_FGN);
}

void	display_prompt(t_client *cli)
{
	t_prompt		*p;
	// char			*pwd;
	// char			*home;

	p = &cli->prompt;
	if (p->prompt)
		return ;
	if (p->err)
		error_prompt();
	else
		ft_printf("%s%s✓ %s%s {eoc}%s {eoc}", ER_FG, OK_BG, OK_FG, PR_BG, PR_FGN);
	// pwd = env_get_by_name(p->env, "PWD");
	// home = env_get_by_name(p->env, "HOME");
	// if (!pwd)
	// 	ft_printf("%s%s$ {eoc}%s {eoc}", PR_FG, PR_BG, PR_FGN);
	// else if (home && !ft_strncmp(home, pwd, ft_strlen(home)))
	// 	ft_printf("%s%s~%s {eoc}%s {eoc}", PR_FG, PR_BG, pwd + ft_strlen(home), PR_FGN);
	// else
	// 	ft_printf("%s%s%s {eoc}%s {eoc}", PR_FG, PR_BG, pwd, PR_FGN);
	p->prompt = 1;
}
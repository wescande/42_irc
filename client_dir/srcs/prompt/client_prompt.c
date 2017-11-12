/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_prompt.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wescande <wescande@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/26 16:32:34 by wescande          #+#    #+#             */
/*   Updated: 2017/10/27 13:36:35 by wescande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client.h>

static void		signal_capture(int sig)
{
	t_client		*cli;
	t_prompt		*p;


	cli = get_cli(1);
	p = &(cli->prompt);
	if (sig == SIGQUIT && p->father)
	{
		ft_printf("{red}[1]\t%d quit\t%s\n{eoc}", p->father, p->name);
		p->father = 0;
		p->name = NULL;
		p->err = 1;
		display_prompt(cli);
	}
	else if (sig == SIGINT)
	{
		ft_printf("\n");
		p->err = 1;
		display_prompt(cli);
	}
}

// static char		*env_get_by_name(char **env, char *str)
// {
// 	//TODO
// 	(void)env;
// 	(void)str;
// 	return (NULL);
// }
// char	*ft_getenv(char **env, char *key)
// {
// 	if (!env || !key || !*env)
// 		return (NULL);
// 	while (*env)
// 	{
// 		if (ft_strcmp(*env, key) == '='
// 				&& ft_strlen(key) == ft_strlenchr(*env, '='))
// 			return (*env + ft_strlen(key) + 1);
// 		env++;
// 	}
// 	return (NULL);
// }

static int		prompt_init(t_client *c, struct termios *term)
{
	char	*name_term;

	if (!isatty(STDOUT_FILENO))
		return (verbose(c->flag, MSG_ERROR, "Output is not a tty", NULL));
	if (!(name_term = ft_getenv(c->prompt.env, "TERM")))
		// name_term = "xterm-256color";
		{name_term = "screen";
		DG("BAD TERM INIT");}
	if (tgetent(NULL, name_term) <= 0)
		return (verbose(c->flag, MSG_ERROR, "Tty name does not match", NULL));
	if (tcgetattr(0, term) == -1)
		return (verbose(c->flag, MSG_ERROR, "Failed to get termios struct", NULL));
	return (catch_term(term));
}

int				client_prompt(t_client *client)
{
	struct termios		term;

	if (prompt_init(client, &term))
		return (1);
	signal(SIGINT, signal_capture);
	signal(SIGQUIT, signal_capture);
	display_prompt(client);
	return (client_shell(client, &term));
}

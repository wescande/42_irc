/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wescande <wescande@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/26 16:06:03 by wescande          #+#    #+#             */
/*   Updated: 2017/11/07 20:02:42 by wescande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include <libft.h>
# include <signal.h>
# include <stdio.h>
# include <sys/socket.h>
# include <stdlib.h>
# include <unistd.h>
# include <netdb.h>
# include <arpa/inet.h>

typedef struct	s_prompt
{
	char		**env;
	char		*line;
	int			pos;
	int			len;
	int			prompt;
	t_ld		*histo;
	t_ld		*curhisto;
	int			err;
	// int			exit;
	char		*name;
	pid_t		father;
}				t_prompt;

typedef struct	s_client
{
	uint64_t	flag;
	char		**av_data;
	t_prompt	prompt;
	char		*straddr;
	in_addr_t	addr;
	uint16_t	port;
	int			socket;
}				t_client;

int				connection(t_client *cli);
t_client		*get_cli(uint8_t reinit);
char			*set_rfc_959_cmd(char *cmd, char **av);
char			*rfc_959_return(uint64_t flag, int socket);

/*
** RFC Command:
*/
int				client_ls(t_client *cli, char **av);

/*
** PROMPT
*/

# include <term.h>

# define OK_FG			"\033[38;5;49m"
# define OK_BG			"\033[48;5;49m"
# define PR_BG			"\033[48;5;68m"
# define PR_FG			"\033[38;5;232m"
# define PR_FGN			"\033[38;5;68m"
# define ER_BG			"\033[48;5;202m"
# define ER_FG			"\033[38;5;232m"
# define ER_FGN			"\033[38;5;202m"
# define PUT(x) tputs(tgetstr(x, NULL), 0, &ft_putchar)
# define BUFF_SIZE		1024

int				client_prompt(t_client *client);
int				client_shell(t_client *cli, struct termios *term);
void			display_prompt(t_client *cli);
int				reinit_term(struct termios *term);
int				catch_term(struct termios *term);


void			move_one(t_prompt *cmd, char c);
void			move_lim(t_prompt *cmd, char c);
void			add_one(t_prompt *cmd, char c);
void			delete_one(t_prompt *cmd);
void			delete_cmd(t_prompt *cmd);
#endif

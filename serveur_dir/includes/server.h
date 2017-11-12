/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wescande <wescande@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/26 16:06:03 by wescande          #+#    #+#             */
/*   Updated: 2017/11/12 19:07:43 by wescande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# ifndef FULL_LIBFT
#  define FULL_LIBFT
# endif

# include <libft.h>
# include <netdb.h>
# include <errno.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <arpa/inet.h>


# define BUFF_SIZE				1024
# define CIRC_BUFF_SIZE			(2 * BUFF_SIZE)

/*
** MAIN FLAG
*/

# define STOP_SERV				(1 << 0)

/*
** Channel flag
*/
# define HAS_MESSAGE			(1 << 0)
/* # define PUB_CHANEL				(1 << 0) */

/*
** Client flag
*/
# define TALK					(1 << 0)

typedef struct s_client		t_client;
typedef struct	s_buf
{
	char		buf[CIRC_BUFF_SIZE];
	uint16_t	read;
	uint16_t	write;
}				t_buf;

typedef struct	s_channel
{
	uint8_t		flag;
	char		*name;
	t_buf		buf;
	/* char		*msg; */
	t_client	*speaker;
	t_lx		client;
	t_lx		list;
}				t_channel;

struct			s_client
{
	uint8_t			flag;
	int			sock;
	t_buf		buf;
	char		*nick;
	t_channel	*chan;
	t_lx		list;
};

typedef struct	s_server
{
	uint64_t	flag;
	char		**av_data;
	uint16_t	port;
	int			master_sock;
	fd_set		readfds;
	fd_set		writefds;
	int			nfds;
	t_lx		client;
	t_lx		channel;
	t_lx		priv_channel;
}				t_server;


int				server_loop(t_server *ser);
int				initialize_server(t_server *ser);
void			server_input(t_server *ser);

/*
** Client manager:
*/
int				client_new(t_server *ser);
void			client_entry(t_server *ser, t_client *cli);
int				client_input(t_server *ser, t_client *cli);
int				client_delete(t_server *ser, t_client *cli);
int				client_message(t_server *ser, t_client *cli);
int				client_command(t_server *ser, t_client *cli);

/*
** Circular buffer
*/
int				circular_buf_add(t_buf *cir, char *buf, uint16_t len);
int				circular_buf_crlf(t_buf *cir);
void			circular_buf_get(t_buf *cir, char *buf);

/*
** Preformat reponse:
*/

/*
** RFC Command:
*/
int				nick(t_server *ser, t_client *cli, char **av);

#endif

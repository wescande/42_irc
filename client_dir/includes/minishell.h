/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wescande <wescande@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/05 23:19:09 by wescande          #+#    #+#             */
/*   Updated: 2017/10/26 16:31:56 by wescande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <term.h>
# include <signal.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <libft.h>
# include <client.h>

# define OK_FG			"\033[38;5;49m"
# define PR_BG			"\033[48;5;68m"
# define PR_FG			"\033[38;5;232m"
# define PR_FGN			"\033[38;5;68m"
# define ER_BG			"\033[48;5;202m"
# define ER_FG			"\033[38;5;232m"
# define ER_FGN			"\033[38;5;202m"
# define PUT(x) tputs(tgetstr(x, NULL), 0, &ft_putchar)
# define MINI_BUFSIZE 1024


/*
** Core
*/
int					minishell(struct termios *term);
char				**get_path(t_ld *env);
char				*env_get_by_name(t_ld *env, char *str);
int					minishell_execute(char **fullcmd, t_ld **env);
int					minishell_cmd(t_ld **ld_env, char **cmd_arg, t_ld *histo);
void				reinit_term(struct termios *term);
void				catch_term(struct termios *term);
t_read_termcap		*get_singleton(int reinit);
int					is_directory(const char *path);

/*
** Termcap
*/
void				termcap_complet(t_read_termcap *cmd);
void				move_one(t_read_termcap *cmd, char c);
void				move_lim(t_read_termcap *cmd, char c);
void				add_one(t_read_termcap *cmd, char c);
void				delete_one(t_read_termcap *cmd);
void				delete_cmd(t_read_termcap *cmd);

/*
** Gestion setenv
*/
int					minishell_setenv(t_ld **env, char **cmd_arg);
int					minishell_unsetenv(t_ld **env, char **cmd_arg);
void				remove_env(t_ld **env, char *arg);
void				fun_setenv(t_ld **env, char *name, char *content);

/*
** Gestion histo
*/
t_ld				*minishell_get_histo(t_read_termcap *cmd);
int					minishell_save_histo(t_ld *histo);
void				show_histo(t_read_termcap *cmd, char c, t_ld **histo,
								int *first);

/*
** Built env
*/
int					minishell_env(t_ld **env, char **cmd_arg);

/*
** Built exit
*/
int					minishell_prepare_exit(char **cmd_arg);
int					minishell_exit(t_ld **env, t_ld **histo,
									struct termios *term);
int					minishell_exit_error(char *str, int ret,
									struct termios *term);

/*
** Built echo
*/
int					minishell_echo(char **cmd_arg);

/*
** Built cd
*/
int					minishell_cd(t_ld **env, char **cmd_arg);
int					prepare_cd(t_ld **env, char **cmd_input);

/*
** Output
*/
void				prompt(void);
void				error_prompt(void);
int					env_prompt(t_ld *env);
int					output_padding(t_ld *lst);

#endif

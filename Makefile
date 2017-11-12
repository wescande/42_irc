# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wescande <wescande@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/08/29 21:32:58 by wescande          #+#    #+#              #
#    Updated: 2017/11/12 12:09:02 by wescande         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME1		=	serveur
NAME2		=	client

DIR1		=	$(NAME1)_dir
DIR2		=	$(NAME2)_dir

LIBFT_DIR	=	libft/
LIBFT_LIB	=	$(LIBFT_DIR)libft.a

all :
	@make -j -C $(LIBFT_DIR) libft.a
	@make -j -C $(DIR1) $(NAME1)
	@make -j -C $(DIR2) $(NAME2)
	@make -j -C $(DIR3) $(NAME3)
	@make $(NAME1)
	@make $(NAME2)
	@make $(NAME3)

$(NAME1):		$(DIR1)/$(NAME1) Makefile
	@cp $(DIR1)/$@ $@
	@printf "\r\033[38;5;117m✓ COPY $@\033[0m\033[K\n"

$(NAME2):		$(DIR2)/$(NAME2) Makefile
	@cp $(DIR2)/$@ $@
	@printf "\r\033[38;5;117m✓ COPY $@\033[0m\033[K\n"

clean:
	@make -C $(DIR1) clean
	@make -C $(DIR2) clean

fclean:			clean
	@make -C $(DIR1) fclean
	@make -C $(DIR2) fclean
	@if [ -e $(NAME1) ]; \
	then \
		rm -rf $(NAME1); \
		printf "\r\033[38;5;196m✗ fclean copy $(NAME1).\033[0m\033[K\n"; \
	fi;
	@if [ -e $(NAME2) ]; \
	then \
		rm -rf $(NAME2); \
		printf "\r\033[38;5;196m✗ fclean copy $(NAME2).\033[0m\033[K\n"; \
	fi;

re:				fclean
	@make

.PHONY :		clean fclean re

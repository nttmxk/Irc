# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: heson <heson@student.42seoul.kr>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/24 01:39:25 by jinoh             #+#    #+#              #
#    Updated: 2023/12/16 21:18:00 by heson            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ---------------------------------- NAME ------------------------------------ #

NAME = ircserv

# ---------------------------------- COLOR ----------------------------------- #

C_BLUE=\033[0;34m
C_GREEN=\033[0;32m
C_YELLOW=\033[0;33m
C_RED=\033[0;31m
C_END=\033[0m

# ------------------------------- DIRECTORIES -------------------------------- #

HDRS_DIR	= includes/
SRCS_DIR	= sources/
OBJS_DIR	= objects/

# ---------------------------------- FILES ----------------------------------- #

INCLUDE = -I $(HDRS_DIR) 
HDRS = $(wildcard $(HDRS_DIR)*.hpp)
SRCS = $(wildcard $(SRCS_DIR)*.cpp) $(wildcard $(SRCS_DIR)commands/*.cpp)
OBJS += $(addprefix $(OBJS_DIR), $(SRCS:.cpp=.o))

# -------------------------------- COMPILATE --------------------------------- #

CC		= c++
CFLAGS	= -Wall -Werror -Wextra -std=c++98
RM		= rm -rf
MD		= mkdir -p

# --------------------------------- RULES ------------------------------------ #

$(OBJS_DIR)%.o : %.cpp $(HDRS)
	@$(MD) $(dir $@)
	$(CC) $(CFLAGS) -g -c $< -o $@

all:
	@echo "$(C_YELLOW)======== make $(NAME) =======$(C_END)"
	@make $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -g -o $(NAME)
	@echo "$(C_GREEN)+-------------------------+"
	@echo "$(C_GREEN)|        COMPLETE         |"
	@echo "$(C_GREEN)+-------------------------+$(C_END)"

clean:
	@echo "$(C_YELLOW)========== cleaning =========$(C_END)"
	$(RM) $(OBJS_DIR)

fclean: clean
	@echo "$(C_YELLOW)====== cleaning $(NAME) =====$(C_END)"
	$(RM) $(NAME)

re:
	@echo "$(C_YELLOW)======== recompiling ========$(C_END)"
	@make fclean
	@make all

.PHONY: all clean fclean re
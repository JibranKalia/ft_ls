# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jkalia <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/03/23 14:12:11 by jkalia            #+#    #+#              #
#*   Updated: 2017/06/29 09:07:44 by jkalia           ###   ########.fr       *#
#                                                                              #
# **************************************************************************** #

CC			:= gcc
TARGET		:= ft_ls
OBJDIR		:= obj
SRCDIR		:= src
INCDIR		:= includes
OBJEXT		:= o
SRCEXT		:= c
CFLAGS		+= -I includes/ -I libft/includes/
LDFLAGS		:= -L libft/ -lft
INC         := -I $(INCDIR)
MAKEFLAGS	="-j $(grep -c ^processor /proc/cpuinfo)"

LIBFT		:= libft/libft.a

FILES		+= ls_main ls_print_dir ls_util ls_sort ls_long ls_args
SRC			:= $(addprefix $(SRCDIR)/, $(addsuffix .$(SRCEXT), $(FILES)))
OBJ			:= $(patsubst $(SRCDIR)/%, $(OBJDIR)/%, $(SRC:.$(SRCEXT)=.$(OBJEXT)))

MAX			:=	$(words $(OBJ))
n			:=	x
increment	=	$1 x
COUNTER		=	$(words $n)$(eval n := $(call increment,$n))

.PHONY = all clean fclean clean re

all: $(TARGET)

$(LIBFT):
	@make -C libft

$(OBJDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INC) -o $@ -c $<
	@printf "\r\e[32mCompiling...(%d/%d)\e[0m" $(COUNTER) $(MAX)

$(TARGET): $(LIBFT) $(OBJ)
	@printf "\r\e[32mCompiling...(%d/%d)[DONE]\n\e[0m" $(MAX) $(MAX)
	@$(CC) $(LDFLAGS) -o $@ $^
	@echo "\033[32mCreated FT_LS\033[0m"

clean:
	@if [ -e $(OBJDIR) ]; \
		then \
		make -C libft clean; \
		rm -rf $(OBJDIR); \
		printf "\r\033[32mRemoving libft-$(OBJDIR)\033[0m\033[K\n"; \
	fi;

fclean:
	@$(MAKE) clean
	@if [ -e $(TARGET) ]; \
		then \
		make -C libft fclean; \
		rm -f $(TARGET); \
		printf "\r\033[32mRemoving $(TARGET)\033[0m\033[K\n"; \
	fi;
	
re:
	@$(MAKE) fclean
	@$(MAKE) all

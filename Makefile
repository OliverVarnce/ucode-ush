APP_NAME = ush

LIBMXF = libmx

LIB_NAME = libmx.a

INC = inc/ \
	  libmx/inc/

SRC_DIR = src

OBJ_DIR = obj

SRC = $(addprefix $(SRC_DIR)/,\
      builtins/mx_add_proc.c \
      builtins/mx_del_proc.c \
      builtins/mx_detect_builds.c \
      builtins/mx_env_in_list.c \
      builtins/mx_env_print.c \
      builtins/mx_proc_chars_find.c \
      builtins/mx_proc_cut_str.c \
      builtins/mx_proc_num_find.c \
      builtins/mx_create_proc.c \
      builtins/mx_cd.c \
      builtins/mx_echo.c \
      builtins/mx_env.c \
      builtins/mx_exit.c \
      builtins/mx_export.c \
      builtins/mx_fg.c \
      builtins/mx_ush_processes.c \
      builtins/mx_pwd.c \
      builtins/mx_unset.c \
      builtins/mx_which.c \
      builtins/mx_print_history.c \
      errhandlers/mx_printerror.c \
      history/mx_history.c \
      memory/mx_free_processes.c \
      memory/mx_free_history.c \
      memory/mx_free_list.c \
      memory/mx_free_node.c \
      parsers/mx_parse.c \
      parsers/mx_parse_backquote.c \
      parsers/mx_parse_dblqte_openparen_closeparen_opbrce_clbrce.c \
      parsers/mx_parse_dollar.c \
      parsers/mx_parse_get_format_str.c \
      parsers/mx_parse_get_proc_output.c \
      parsers/mx_parse_get_subst_replace_str.c \
      parsers/mx_parse_mark_chars.c \
      parsers/mx_parse_mx_check_parse.c \
      parsers/mx_parse_mx_create_outer_subst_n_dblq_list.c \
      parsers/mx_parse_pop_push_free_format_list.c \
      parsers/mx_parse_replace_sub_str.c \
      parsers/mx_parse_single_quote.c \
      parsers/mx_parse_substitutions.c \
      parsers/mx_parse_tilda.c \
      sorting/mx_pop_frontf.c \
      sorting/mx_push_backdup.c \
      stream/mx_read_stream.c \
      stream/mx_stream.c \
      utils/mx_copy_dub_arr.c \
      utils/mx_detect_exp.c \
      utils/main.c \
      utils/mx_enable_disable_canon.c \
      utils/mx_join.c \
      utils/mx_delit_fre.c \
      utils/mx_getopt.c \
      utils/mx_setup_term.c \
      utils/mx_strpart.c \
      utils/mx_strcmp_null.c \
      utils/mx_name_search.c \
      utils/mx_opencheck.c \
      utils/mx_empty_proc.c \
      utils/mx_pop_front_proc.c)


OBJ = mx_add_proc.o \
      mx_del_proc.o \
      mx_detect_builds.o \
      mx_env_in_list.o \
      mx_env_print.o \
      mx_proc_chars_find.o \
      mx_proc_cut_str.o \
      mx_proc_num_find.o \
      mx_create_proc.o \
      mx_cd.o \
      mx_echo.o \
      mx_env.o \
      mx_exit.o \
      mx_export.o \
      mx_fg.o \
      mx_ush_processes.o \
      mx_pwd.o \
      mx_unset.o \
      mx_which.o \
      mx_printerror.o \
      mx_history.o \
      mx_print_history.o \
      mx_free_processes.o \
      mx_free_history.o \
      mx_free_list.o \
      mx_free_node.o \
      mx_parse.o \
      mx_parse_backquote.o \
      mx_parse_dblqte_openparen_closeparen_opbrce_clbrce.o \
      mx_parse_dollar.o \
      mx_parse_get_format_str.o \
      mx_parse_get_proc_output.o \
      mx_parse_get_subst_replace_str.o \
      mx_parse_mark_chars.o \
      mx_parse_mx_check_parse.o \
      mx_parse_mx_create_outer_subst_n_dblq_list.o \
      mx_parse_pop_push_free_format_list.o \
      mx_parse_replace_sub_str.o \
      mx_parse_single_quote.o \
      mx_parse_substitutions.o \
      mx_parse_tilda.o \
      mx_pop_frontf.o \
      mx_push_backdup.o \
      mx_read_stream.o \
      mx_stream.o \
      mx_copy_dub_arr.o \
      mx_detect_exp.o \
      main.o \
      mx_enable_disable_canon.o \
      mx_join.o \
      mx_delit_fre.o \
      mx_getopt.o \
      mx_setup_term.o \
      mx_strpart.o \
      mx_strcmp_null.o \
      mx_name_search.o \
      mx_opencheck.o \
      mx_empty_proc.o \
      mx_pop_front_proc.o

CC = clang

CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic #-g3 -fsanitize=address

all : install

install : libmx/libmx.a ush

libmx/libmx.a:
	@make -C $(LIBMXF)

ush : $(SRC) inc/ush.h libmx/libmx.a
	@$(CC) $(CFLAGS) -c $(SRC) $(foreach d, $(INC), -I $d)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBMXF)/$(LIB_NAME) -o $(APP_NAME)
	@printf "\r\33[2K$@ \033[32;1mcreated\033[0m\n"
	@mkdir -p $(OBJ_DIR)
	@mv $(OBJ) $(OBJ_DIR)
	@printf "$(NAME) \033[31;1minstalled\033[0m\n"

uninstall : clean
	@make uninstall -C $(LIBMXF)
	@rm -rf $(APP_NAME)
	@printf "$(NAME) \033[31;1muninstalled\033[0m\n"

clean :
	@make clean -C $(LIBMXF)
	@rm -rf $(OBJ_DIR)
	@printf "$(OBJD)\t   \033[31;1mdeleted\033[0m\n"

reinstall : uninstall install
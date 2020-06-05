APP_NAME = ush

LIBMXF = libmx

LIB_NAME = libmx.a

INC = \
inc/ \
libmx/inc/

SRC_DIR = src

OBJ_DIR = obj

SRC = $(addprefix $(SRC_DIR)/,\
      builtins/add_job.c \
      builtins/del_job.c \
      builtins/detect_builds.c \
      builtins/env_in_list.c \
      builtins/env_print.c \
      builtins/job_chars_find.c \
      builtins/job_cut_str.c \
      builtins/job_num_find.c \
      builtins/mx_create_job.c \
      builtins/ush_cd.c \
      builtins/ush_echo.c \
      builtins/ush_env.c \
      builtins/ush_exit.c \
      builtins/ush_export.c \
      builtins/ush_fg.c \
      builtins/ush_jobs.c \
      builtins/ush_pwd.c \
      builtins/ush_unset.c \
      builtins/ush_which.c \
      errhandlers/mx_printerror.c \
      history/history.c \
      memory/free_jobs.c \
      memory/free_list.c \
      memory/free_list2.c \
      memory/free_node.c \
      parsers/parse.c \
      parsers/parse_backquote.c \
      parsers/parse_dblqte_openparen_closeparen_opbrce_clbrce.c \
      parsers/parse_dollar.c \
      parsers/parse_get_format_str.c \
      parsers/parse_get_process_output.c \
      parsers/parse_get_subst_replace_str.c \
      parsers/parse_mark_chars.c \
      parsers/parse_mx_check_parse.c \
      parsers/parse_mx_create_outer_subst_n_dblq_list.c \
      parsers/parse_pop_push_free_format_list.c \
      parsers/parse_replace_sub_str.c \
      parsers/parse_single_quote.c \
      parsers/parse_substitutions.c \
      parsers/parse_tilda.c \
      sorting/mx_pop_frontf.c \
      sorting/mx_push_backdup.c \
      stream/read_stream.c \
      stream/stream.c \
      utils/copy_dub_arr.c \
      utils/detect_exp.c \
      utils/main.c \
      utils/mx_enable_disable_canon.c \
      utils/mx_join.c \
      utils/mx_delit_fre.c \
      utils/mx_getopt.c \
      utils/mx_setup_term.c \
      utils/mx_strpart.c \
      utils/mx_strcmp_null.c \
      utils/name_search.c \
      utils/opencheck.c \
      utils/straus_proc.c)


OBJ = add_job.o \
      del_job.o \
      detect_builds.o \
      env_in_list.o \
      env_print.o \
      job_chars_find.o \
      job_cut_str.o \
      job_num_find.o \
      mx_create_job.o \
      ush_cd.o \
      ush_echo.o \
      ush_env.o \
      ush_exit.o \
      ush_export.o \
      ush_fg.o \
      ush_jobs.o \
      ush_pwd.o \
      ush_unset.o \
      ush_which.o \
      mx_printerror.o \
      history.o \
      free_jobs.o \
      free_list.o \
      free_list2.o \
      free_node.o \
      parse.o \
      parse_backquote.o \
      parse_dblqte_openparen_closeparen_opbrce_clbrce.o \
      parse_dollar.o \
      parse_get_format_str.o \
      parse_get_process_output.o \
      parse_get_subst_replace_str.o \
      parse_mark_chars.o \
      parse_mx_check_parse.o \
      parse_mx_create_outer_subst_n_dblq_list.o \
      parse_pop_push_free_format_list.o \
      parse_replace_sub_str.o \
      parse_single_quote.o \
      parse_substitutions.o \
      parse_tilda.o \
      mx_pop_frontf.o \
      mx_push_backdup.o \
      read_stream.o \
      stream.o \
      copy_dub_arr.o \
      detect_exp.o \
      main.o \
      mx_enable_disable_canon.o \
      mx_join.o \
      mx_delit_fre.o \
      mx_getopt.o \
      mx_setup_term.o \
      mx_strpart.o \
      mx_strcmp_null.o \
      name_search.o \
      opencheck.o \
      straus_proc.o

CC = clang

CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic

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
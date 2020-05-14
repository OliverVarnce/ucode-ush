APP_NAME = ush

LIBMXF = libmx

LIB_NAME = libmx.a

INC = \
inc/ \
libmx/inc/

SRC_DIR = src

OBJ_DIR = obj

SRC = $(addprefix $(SRC_DIR)/,\
        	cd/mx_cd_with_flags.c \
            cd/mx_cd_without_flags.c \
            env/mx_env_flags.c \
            env/mx_env_func.c \
            env/mx_env_main.c \
            env/mx_env_run.c \
            env/mx_envp_i.c \
            env/mx_envp_manage.c \
            env/mx_envp_pack.c \
            err/mx_errors_cd.c \
            err/mx_errors_for_comands.c \
            parsers/mx_main_parse.c \
            parsers/mx_parcer_iline.c \
            parsers/mx_parse_flags_cd.c \
            parsers/mx_parser.c \
            parsers/mx_parser_array.c \
            parsers/mx_parser_tilda.c \
            parsers/mx_parser_var.c \
            printing/mx_e_no_dirorfile_cd.c \
            printing/mx_e_nodir_after_split_pwd_cd.c \
            printing/mx_e_notpwd_cd.c \
            printing/mx_e_too_many_arg_cd.c \
            printing/mx_print_e_for_echo.c \
            printing/mx_printerror.c \
            pwd/mx_pwd_replace.c \
            pwd/mx_pwd_run.c \
            runners/mx_run_cd.c \
            runners/mx_run_cd_command.c \
            runners/mx_run_echo_command.c \
            runners/mx_run_exec.c \
            runners/mx_run_exit.c \
            which/mx_which_helper.c \
            which/mx_which_run.c \
            main.c \
            mx_appropos_command.c \
            mx_arch_command.c \
            mx_aud_for_ignore.c \
            mx_aud_for_vars.c \
            mx_buffer_for_ignored_symb.c \
            mx_change_pwd_in_cd.c \
            mx_change_var.c \
            mx_chdir_command.c \
            mx_count_numbers.c \
            mx_dirorfile.c \
            mx_export_run.c \
            mx_get_command.c \
            mx_handling_signals.c \
            mx_help_command.c \
            mx_ignore_symb.c \
            mx_join_for_cd.c \
            mx_my_count_words.c \
            mx_my_repl_substr.c \
            mx_my_repl_substr_cd.c \
            mx_mystrsplit.c \
            mx_printlist.c \
            mx_return_command.c \
            mx_save_story.c \
            mx_second_parse.c \
            mx_set_correct_path.c \
            mx_true_false.c \
            mx_uniq_prompt.c \
            mx_unset_run.c \
            mx_ush_init.c \
            mx_ush_rescleaner.c \
            mx_ush_strsplit.c)


OBJ = mx_cd_with_flags.o \
      mx_cd_without_flags.o \
      mx_env_flags.o \
      mx_env_func.o \
      mx_env_main.o \
      mx_env_run.o \
      mx_envp_i.o \
      mx_envp_manage.o \
      mx_envp_pack.o \
      mx_errors_cd.o \
      mx_errors_for_comands.o \
      mx_main_parse.o \
      mx_parcer_iline.o \
      mx_parse_flags_cd.o \
      mx_parser.o \
      mx_parser_array.o \
      mx_parser_tilda.o \
      mx_parser_var.o \
      mx_e_no_dirorfile_cd.o \
      mx_e_nodir_after_split_pwd_cd.o \
      mx_e_notpwd_cd.o \
      mx_e_too_many_arg_cd.o \
      mx_print_e_for_echo.o \
      mx_printerror.o \
      mx_pwd_replace.o \
      mx_pwd_run.o \
      mx_run_cd.o \
      mx_run_cd_command.o \
      mx_run_echo_command.o \
      mx_run_exec.o \
      mx_run_exit.o \
      mx_which_helper.o \
      mx_which_run.o \
      main.o \
      mx_appropos_command.o \
      mx_arch_command.o \
      mx_aud_for_ignore.o \
      mx_aud_for_vars.o \
      mx_buffer_for_ignored_symb.o \
      mx_change_pwd_in_cd.o \
      mx_change_var.o \
      mx_chdir_command.o \
      mx_count_numbers.o \
      mx_dirorfile.o \
      mx_export_run.o \
      mx_get_command.o \
      mx_handling_signals.o \
      mx_help_command.o \
      mx_ignore_symb.o \
      mx_join_for_cd.o \
      mx_my_count_words.o \
      mx_my_repl_substr.o \
      mx_my_repl_substr_cd.o \
      mx_mystrsplit.o \
      mx_printlist.o \
      mx_return_command.o \
      mx_save_story.o \
      mx_second_parse.o \
      mx_set_correct_path.o \
      mx_true_false.o \
      mx_uniq_prompt.o \
      mx_unset_run.o \
      mx_ush_init.o \
      mx_ush_rescleaner.o \
      mx_ush_strsplit.o

CC = clang

CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic

all : install

install : libmx/libmx.a uls

libmx/libmx.a:
	@make -C $(LIBMXF)

uls : $(SRC) inc/ush.h libmx/libmx.a
	@$(CC) $(CFLAGS) -c $(SRC) $(foreach d, $(INC), -I $d)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBMXF)/$(LIB_NAME) -o $(APP_NAME)
	@printf "\r\33[2K$@ \033[32;1mcreated\033[0m\n"
	@mkdir -p $(OBJ_DIR)
	@mv $(OBJ) $(OBJ_DIR)
	@printf "\r\33[2K$(NAME) \033[33;1mcompile \033[0m$(<:$(SRC)/%.c=%) "

uninstall : clean
	@make uninstall -C $(LIBMXF)
	@rm -rf $(APP_NAME)
	@printf "$(NAME) \033[31;1muninstalled\033[0m\n"

clean :
	@make clean -C $(LIBMXF)
	@rm -rf $(OBJ_DIR)
	@printf "$(OBJD)\t   \033[31;1mdeleted\033[0m\n"

reinstall : uninstall install
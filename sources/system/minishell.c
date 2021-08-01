/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phemsi-a <phemsi-a@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 17:28:42 by lcouto            #+#    #+#             */
/*   Updated: 2021/07/31 15:16:37 by lfrasson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	save_history(char *input)
{
	if (input && *input)
		add_history(input);
}

static void	read_input(char **input, bool *emoji)
{
	char	*prompt;

	prompt = create_prompt(emoji);
	*input = readline(prompt);
	free(prompt);
}

static void	repl(void)
{
	bool	emoji;
	char	*input;
	t_token	*token_lst;

	emoji = false;
	while (true)
	{
		token_lst = NULL;
		define_input_signals();
		read_input(&input, &emoji);
		save_history(input);
		tokenizer(&input, &token_lst);
		parse_and_execute(token_lst);
		token_list_clear(&token_lst);
		if (input)
			free(input);
	}
}

int	main(int argc, char **argv)
{
	if (argc > 1 && argv)
		error_message("👿", TOO_MANY_ARGS);
	g_minishell.env = env_to_hashmap(__environ);
	g_minishell.local_vars = hashmap_create_table(50);
	g_minishell.error_status = 0;
	repl();
	return (0);
}

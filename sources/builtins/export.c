/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phemsi-a <phemsi-a@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/16 20:23:19 by phemsi-a          #+#    #+#             */
/*   Updated: 2021/07/24 17:14:45 by phemsi-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_ordered_env(void)
{
	char	**ordered_env;

	ordered_env = hashmap_to_env_with_quotes(g_minishell.env);
	quick_sort_2d_array(ordered_env, 0, get_2d_array_len(ordered_env));
	join_2d_array("declare -x ", ordered_env);
	print_2d_array_fd(ordered_env, STDOUT_FILENO);
	free_2d_array(ordered_env);
	return (0);
}

static void	define_variable(char **cmd, int index)
{
	char	**key_value;

	key_value = ft_split(cmd[index], '=');
	hashmap_insert(key_value[0], key_value[1], g_minishell.env);
	free_2d_array(key_value);
}

static void	update_variable(char **cmd, int index)
{
	char	*value;

	value = hashmap_search(g_minishell.local_vars, cmd[index]);
	if (!value)
		value = ft_strdup("");
	hashmap_insert(cmd[index], value, g_minishell.env);
	hashmap_delete(g_minishell.local_vars, cmd[index]);
	free(value);
}

static int	export_variable(char **cmd, int index)
{
	if (!cmd[index])
		return (0);
	if (ft_strchr(cmd[index], '='))
		define_variable(cmd, index);
	else
		update_variable(cmd, index);
	return (export_variable(cmd, index + 1));
}

int	export(char **cmd)
{
	if (cmd[1])
		return (export_variable(cmd, 1));
	else
		return (print_ordered_env());
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_part2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhtab <aakhtab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 03:47:33 by aakhtab           #+#    #+#             */
/*   Updated: 2023/11/11 04:59:38 by aakhtab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	*out_file(t_redir *file,	int *fd)
{
	if (file->type == GREAT	)
	{
		fd[1] = open(file->redirect, O_RDWR | O_CREAT | O_TRUNC, 0644);
		if (fd[1] < 0)
		{
			ft_error("minishell: No such file or directory\n", 1);
			return (NULL);
		}
	}
	if (file->type == APPEND)
	{
		fd[1] = open(file->redirect, O_RDWR | O_CREAT | O_APPEND, 0644);
		if (fd[1] < 0)
		{
			ft_error("minishell: No such file or directory\n", 1);
			return (NULL);
		}
	}
	return (fd);
}

int	*openfile_ut(t_redir *file,	int *fd)
{
	fd = out_file(file, fd);
	if (file->type == LESS)
	{
		fd[0] = open(file->redirect, O_RDONLY, 0644);
		if (fd[0] < 0)
		{
			ft_error("minishell: No such file or directory\n", 1);
			return (NULL);
		}
	}
	if (file->type == HEREDOC && g_data.exitheredoc != 1)
		fd[0] = heredoc(file->redirect, file->in_quote);
	return (fd);
}

int	*openfile(t_cmd_tab *list)
{
	int		*fd;
	t_cmd_tab	*tmp;
	t_redir	*fl;

	tmp = list;
	fl = tmp->redirs;
	fd = malloc(sizeof(int) * 2);
	fd[0] = 0;
	fd[1] = 0;
	while (fl)
	{
		fd = openfile_ut(fl, fd);
		if (!fd)
			return (NULL);
		fl = fl->next;
	}
	return (fd);
}

void	ft_execve(t_cmd_tab *list, t_vr *vr, char *cmderr)
{
	if (!in_builtin(list))
	{	
		if (execve(list->cmd[0], list->cmd, vr->env) < 0)
		{
			ft_error(ft_strjoin1(ft_strjoin1("minishell : ", cmderr),
					" :command not found\n"), 127);
			free (cmderr);
			exit (127);
		}
	}
}

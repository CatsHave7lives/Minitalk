/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessaber <aessaber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 17:05:38 by aessaber          #+#    #+#             */
/*   Updated: 2025/04/13 18:35:26 by aessaber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static int	arg_is_invalid(int ac, char **av)
{
	if (ac != 3)
		return (ft_putstr_fd("Do: ./client [server PID] [message]\n", 1), FAIL);
	if (!av[1][0])
		return (ft_putstr_fd("Empty PID\n", 1), FAIL);
	if (!av[2][0])
		return (ft_putstr_fd("Empty message\n", 1), FAIL);
	return (0);
}

static int	pid_is_invalid(int server_pid, char *str)
{
	int	col;

	if (server_pid == 0 || server_pid == -1)
		return (FAIL);
	col = 0;
	while (str[col])
	{
		if (!ft_isdigit(str[col]))
			return (FAIL);
		col++;
	}
	return (0);
}

static void	message_encrypt(int server_pid, char digit)
{
	int	bits;
	int	fail_check;

	bits = 0;
	while (bits < 8)
	{
		if ((digit & (1 << bits)))
			fail_check = kill(server_pid, SIGUSR1);
		else
			fail_check = kill(server_pid, SIGUSR2);
		if (fail_check == -1)
			(ft_putstr_fd("Failed to send signal\n", 1), exit(FAIL));
		usleep(50);
		bits++;
	}
}

int	main(int ac, char **av)
{
	int	col;
	int	server_pid;

	if (arg_is_invalid(ac, av))
		return (FAIL);
	col = 0;
	server_pid = ft_atoi(av[1]);
	if (pid_is_invalid(server_pid, av[1]))
		return (ft_putstr_fd("Invalid PID\n", 1), FAIL);
	while (av[2][col])
	{
		message_encrypt(server_pid, av[2][col]);
		col++;
	}
	message_encrypt(server_pid, '\0');
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessaber <aessaber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 12:04:45 by aessaber          #+#    #+#             */
/*   Updated: 2025/04/13 21:48:45 by aessaber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static t_server	g_server;	

static void	message_decrypt(int sigusr, siginfo_t *client_info, void *n)
{
	(void)n;
	if (g_server.client_pid && g_server.client_pid != client_info->si_pid)
	{
		g_server.digit = 0;
		g_server.bits = 0;
	}
	g_server.client_pid = client_info->si_pid;
	if (sigusr == SIGUSR1)
		g_server.digit |= (1 << g_server.bits);
	g_server.bits++;
	if (g_server.bits == 8)
	{
		if (g_server.digit == '\0')
			ft_putchar_fd('\n', 1);
		else
			ft_putchar_fd(g_server.digit, 1);
		g_server.digit = 0;
		g_server.bits = 0;
	}
}

static void	print_server_pid(void)
{
	int	server_pid;

	server_pid = getpid();
	ft_putstr_fd("The Server is on!\nServer PID: [ ", 1);
	ft_putnbr_fd(server_pid, 1);
	ft_putstr_fd(" ]\n", 1);
}

int	main(int ac, char **av)
{
	struct sigaction	signal_action;

	(void)av;
	if (ac != 1)
		return (ft_putstr_fd("Do: ./server\n", 1), FAIL);
	signal_action.sa_flags = SA_SIGINFO;
	signal_action.sa_sigaction = message_decrypt;
	sigemptyset(&signal_action.sa_mask);
	sigaddset(&signal_action.sa_mask, SIGUSR1);
	sigaddset(&signal_action.sa_mask, SIGUSR2);
	if (sigaction(SIGUSR1, &signal_action, NULL) == -1
		|| sigaction(SIGUSR2, &signal_action, NULL) == -1)
		return (ft_putstr_fd("Failed to register signal handler\n", 1), FAIL);
	print_server_pid();
	while (1)
		pause();
	return (0);
}

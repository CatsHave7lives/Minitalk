/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_main_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessaber <aessaber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 12:04:45 by aessaber          #+#    #+#             */
/*   Updated: 2025/04/13 21:44:10 by aessaber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk_bonus.h"

static t_server	g_server;	

static void	print_bytes(unsigned char *bytes, int count)
{
	int	col;

	col = 0;
	while (col < count)
	{
		ft_putchar_fd(bytes[col], 1);
		col++;
	}
	g_server.digit = 0;
	g_server.bits = 0;
	g_server.count = 0;
	ft_bzero(g_server.bytes, 4);
}

static void	utf_handler(int client_pid)
{
	int	bytes_cap;

	if (g_server.digit <= 0b01111111)
	{
		if (g_server.digit == 0b00000000)
			(ft_putchar_fd('\n', 1), kill(client_pid, SIGUSR1));
		else
			ft_putchar_fd(g_server.digit, 1);
		g_server.digit = 0;
		g_server.bits = 0;
		ft_bzero(g_server.bytes, 4);
		return ;
	}
	g_server.bytes[g_server.count++] = g_server.digit;
	bytes_cap = 1;
	if ((g_server.bytes[0] & 0b11100000) == 0b11000000)
		bytes_cap = 2;
	else if ((g_server.bytes[0] & 0b11110000) == 0b11100000)
		bytes_cap = 3;
	else if ((g_server.bytes[0] & 0b11111000) == 0b11110000)
		bytes_cap = 4;
	if (g_server.count >= bytes_cap)
		print_bytes(g_server.bytes, g_server.count);
	g_server.digit = 0;
	g_server.bits = 0;
}

static void	message_decrypt(int sigusr, siginfo_t *client_info, void *n)
{
	(void)n;
	if (g_server.client_pid && g_server.client_pid != client_info->si_pid)
	{
		g_server.digit = 0;
		g_server.bits = 0;
		g_server.count = 0;
		ft_bzero(g_server.bytes, 4);
	}
	g_server.client_pid = client_info->si_pid;
	if (sigusr == SIGUSR1)
		g_server.digit |= (1 << g_server.bits);
	g_server.bits++;
	if (g_server.bits == 8)
		utf_handler(client_info->si_pid);
}

int	main(int ac, char **av)
{
	struct sigaction	signal_action;
	int					server_pid;

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
	server_pid = getpid();
	ft_putstr_fd("The Server is on!\nServer PID: [ ", 1);
	ft_putnbr_fd(server_pid, 1);
	ft_putstr_fd(" ]\n", 1);
	while (1)
		pause();
	return (0);
}

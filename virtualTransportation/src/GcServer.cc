#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <cstdio>
#include <string>

#include "GcNetworkUtil.hh"

/*
 * A simple server that listen to simulation clients. It collects position information
 * from different clients and re-distribute them.
 */
int main(int argc, char *argv[]) {

	struct addrinfo hints, *res;
	int sockfd, *clientFds, numClients;
	gcPacket *clientPkts;

	if (argc != 2) {
		fprintf(stderr, "usage: GcServer numClients");
		return 1;
	}

	if ((numClients = std::stoi(argv[1])) < 1) {
		fprintf(stderr, "need at least one client (numClients=%d)", numClients);
		return 1;
	}

	clientFds = (int *) calloc(numClients, sizeof(int));
	clientPkts = (gcPacket *) calloc(sizeof(gcPacket) * numClients, sizeof(char));

	sockfd = gc::ListenGcClients(numClients);

	// Initialize connections with clients. For each client, store its corresponding
	// file descriptor, and then send a packet containing its network-id and the
	// total number of clients.
	for (int i = 0; i < numClients; i++) {
		struct sockaddr_storage client_addr;
		socklen_t addr_size = sizeof client_addr;
		clientFds[i] = accept(sockfd, (struct sockaddr *) &client_addr, &addr_size);
		printf("client %d connected\n", i);
		struct initPkg pkg = { i, numClients };
		send(clientFds[i], &pkg, sizeof(pkg), 0); // notify the client of its id and number of clients
	}

	// Synchronize the update loop between clients.
	while (true) {
		// get position packets from the clients
		for (int i = 0; i < numClients; i++) {
			gc::Recv(clientFds[i], (char *) &clientPkts[i], sizeof(gcPacket));
		}
		// send the position packet of each client to all other clients
		for (int i = 0; i < numClients; i++) {
			for (int j = 0; j < numClients; j++) {
				// a client won't receive the package it just sent to the server
				if (i == j)
					continue;
				gc::Send(clientFds[i], (char *) &clientPkts[j], sizeof(gcPacket));
			}
		}
	}

	return 1;
}
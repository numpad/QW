#ifndef QW_UTIL_H
#define QW_UTIL_H

/* QW auxiliary */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

typedef enum {
	SOCKET_NEW,
	SOCKET_CONNECTED,
	SOCKET_ERROR_OPENING,
	SOCKET_ERROR_BINDING,
	SOCKET_ERROR_ACCEPTING
} qa_socket_status;

/* qa_server functions */
typedef struct {
	/* everything ok? */
	qa_socket_status status;

	int port;
	int sockfd, newsockfd;
	
	socklen_t clilen;
	struct sockaddr_in cli_addr;
} qa_server;

/* create a server with error status */
qa_server qa_server_error(int port, qa_socket_status status) {
	return (qa_server) {
		.port = port,
		.status = status,
		.sockfd = -1,
		.newsockfd = -1
	};
}

/* create a server running on port `portno` */
qa_server qa_server_create(int portno) {
	int sockfd, newsockfd;
	struct sockaddr_in serv_addr, cli_addr;
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	/* did opening the socket work? */
	if (sockfd < 0)
		return qa_server_error(portno, SOCKET_ERROR_OPENING);
	
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		return qa_server_error(portno, SOCKET_ERROR_BINDING);
	
	listen(sockfd, 5);
	socklen_t clilen = sizeof(cli_addr);
	
	//newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	//if (newsockfd < 0)
		//return qa_server_error(portno, SOCKET_ERROR_ACCEPTING);
	
	return (qa_server) {
		.status = SOCKET_NEW,
		.port = portno,
		.sockfd = sockfd,
		.newsockfd = newsockfd,
		.clilen = clilen,
		.cli_addr = cli_addr
	};
}

void qa_server_accept(qa_server *s) {
	s->newsockfd = accept(s->sockfd, (struct sockaddr *) &(s->cli_addr), &(s->clilen));
}

void qa_server_close(qa_server s) {
	close(s.newsockfd);
}

/* close server */
void qa_server_destroy(qa_server s) {
	close(s.sockfd);
}

/* read string from client*/
int qa_server_read(qa_server s, char *buffer, int buf_sz) {
	int n = read(s.newsockfd, buffer, buf_sz - 1);
	
	return n;
}

/* write `str_len` bytes to client */
int qa_server_writen(qa_server s, char *string, int str_len) {
	int n = write(s.newsockfd, string, str_len);
	
	return n;
}

/* write `string` to client */
void qa_server_write(qa_server s, char *string) {
	qa_server_writen(s, string, strlen(string));
}

enum {
	/* general socket status */
	TCP_CLIENT_ERROR, TCP_CLIENT_OK,
	/* specific socket status */
	TCP_CLIENT_ERROR_OPENING_SOCKET, TCP_CLIENT_ERROR_NO_SUCH_HOST
} TCP_CLIENT_STATUS;

void qa_run_client(const char *hostname, int portno) {
	int sockfd, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	
	const int BUF_SZ = 1024 * 4; // 4MB of data
	char buffer[BUF_SZ];
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		perror("ERROR opening socket");
	server = gethostbyname(hostname);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	memcpy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
		perror("ERROR connecting");
	printf("Please enter the message: ");
	memset(buffer, 0, BUF_SZ);
	fgets(buffer, BUF_SZ, stdin);

	char msg[] = "GET /html HTTP/1.0\r\n\r\n";
	n = write(sockfd, msg, strlen(msg));
	if (n < 0) 
		perror("ERROR writing to socket");
	bzero(buffer, BUF_SZ);
	n = read(sockfd,buffer, BUF_SZ - 1);
	if (n < 0) 
		perror("ERROR reading from socket");
	printf("%s\n", buffer);
	close(sockfd);
}

#endif

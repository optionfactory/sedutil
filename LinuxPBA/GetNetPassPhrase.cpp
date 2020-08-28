#include "GetNetPassPhrase.h"

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define PORT 7253
#define MAXLINE 1024

std::shared_ptr<SecureString> GetNetPassPhrase()
{
    int sockfd;
    char buffer[MAXLINE];
    char const *hello = "GIMMEPWD";
    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    int broadcast_enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) < 0)
    {
        perror("Error");
        std::shared_ptr<SecureString> password = std::allocate_shared<SecureString>(SecureAllocator<SecureString>(), "");
        return password;
    }

    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
    {
        perror("Error");
        std::shared_ptr<SecureString> password = std::allocate_shared<SecureString>(SecureAllocator<SecureString>(), "");
        return password;
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_BROADCAST;

    socklen_t n, len;

    sendto(sockfd, (const char *)hello, strlen(hello),
           MSG_CONFIRM, (const struct sockaddr *)&servaddr,
           sizeof(servaddr));
    printf("Request sent.\n");

    n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                 MSG_WAITALL, (struct sockaddr *)&servaddr,
                 &len);
    buffer[n] = '\0';
    printf("Received : %s\n", buffer);

    close(sockfd);

    std::shared_ptr<SecureString> password = std::allocate_shared<SecureString>(SecureAllocator<SecureString>(), buffer);
    return password;
}

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define FAIL    -1

int OpenConnection(const char *hostname, int port)
{   int sd;
struct hostent *host;
struct sockaddr_in addr;

if ( (host = gethostbyname(hostname)) == NULL )
{
    printf('Eroor: %s\n',hostname);
    perror(hostname);
    abort();
}
sd = socket(PF_INET, SOCK_STREAM, 0);
bzero(&addr, sizeof(addr));
addr.sin_family = AF_INET;
addr.sin_port = htons(port);
addr.sin_addr.s_addr = *(long*)(host->h_addr);
if ( connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
{
    close(sd);
    perror(hostname);
    abort();
}
return sd;
}

SSL_CTX* InitCTX(void)
{   SSL_METHOD *method;
SSL_CTX *ctx;

OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
SSL_load_error_strings();   /* Bring in and register error messages */
method = SSLv23_client_method();  /* Create new client-method instance */
ctx = SSL_CTX_new(method);   /* Create new context */
if ( ctx == NULL )
{
    ERR_print_errors_fp(stderr);
    printf('Eroor: %s\n',stderr);
    abort();
}
return ctx;
}

void ShowCerts(SSL* ssl)
{   X509 *cert;
    char *line;

    cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
    if ( cert != NULL )
    {
    printf("Server certificates:\n");
    line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
    printf("Subject: %s\n", line);
    free(line);       /* free the malloc'ed string */
    line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
    printf("Issuer: %s\n", line);
    free(line);       /* free the malloc'ed string */
    X509_free(cert);     /* free the malloc'ed certificate copy */
}
else
    printf("No certificates.\n");
}

int main(int count, char *strings[])
{   SSL_CTX *ctx;
int server;
SSL *ssl;
char buffer[1024];
char *hello = "Hello from client";
int bytes;
char *hostname, *portnum;

if ( count != 3 )
{
    printf("usage: %s <hostname> <portnum>\n", strings[0]);
    exit(0);
}


SSL_library_init();
hostname=strings[1];
portnum=strings[2];

ctx = InitCTX();
server = OpenConnection(hostname, atoi(portnum));
ssl = SSL_new(ctx);      /* create new SSL connection state */
int c=SSL_set_fd(ssl, server);    /* attach the socket descriptor */
printf("Gia tri c:%d",c);
if ( SSL_connect(ssl) == (-1))   /* perform the connection */
{ 
    printf("Loi conection\n");
    printf('Eroor: %s\n',stderr);
    ERR_print_errors_fp(stderr);
}
else
{   	char mess_from_client[225];
        printf("Nhap noi dung tin nhan gui den server\n");
        gets(mess_from_client);
        fflush(stdin);
        hello = &mess_from_client;
	     memset(buffer,0,1024);//Clear buffer
    printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
    ShowCerts(ssl);        /* get any certs */
    SSL_write(ssl, mess_from_client, strlen(mess_from_client));   /* encrypt & send message */
    bytes = SSL_read(ssl, buffer, sizeof(buffer)); /* get reply & decrypt */
    buffer[bytes] = 0;
    printf("Received: \"%s\"\n", buffer);
    SSL_free(ssl);        /* release connection state */
}
close(server);         /* close socket */
SSL_CTX_free(ctx);        /* release context */
return 0;
}

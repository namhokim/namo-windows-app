/* OpenSSL headers */
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")

int main(int argc, char *argv[])
{
	/* Initializing OpenSSL */
	printf("Initializing OpenSSL\n");
	SSL_load_error_strings();
	ERR_load_BIO_strings();
	OpenSSL_add_all_algorithms();

	BIO * bio;
	bio = BIO_new_connect("10.15.1.100:8080");
	if(bio == NULL)
	{
		/* Handle the failure */
		printf("Can't Connect\n");
	}

	if(BIO_do_connect(bio) <= 0)	// 에러시 0 또는 -1
	{
		/* Handle failed connection */
	}

	const int len = 256;
	char buf[len];
	memset(&buf, 0, len);
	int x = BIO_read(bio, buf, len);
	if(x == 0)	// 연결이 닫혔다
	{
		/* Handle closed connection */
		printf("Closed connection\n", buf);
	}
	else if(x < 0)	// -1: 에러
	{
	   if(! BIO_should_retry(bio))
		{
			/* Handle failed read here */
			printf("Cannnot Retry\n", buf);
		}
		/* Do something to handle the retry */
		printf("Retry\n", buf);
	}
	else {
		printf("res >%s\n", buf);
	}

	/* To reuse the connection, use this line */
	BIO_reset(bio);
	
	return 0;
}

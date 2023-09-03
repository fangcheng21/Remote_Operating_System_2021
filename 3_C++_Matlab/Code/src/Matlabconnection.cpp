#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "Matlabconnection.h"

MatConnection::MatConnection()
{

}

//******************************************************************************

MatConnection::~MatConnection()
{
	close();
}

//******************************************************************************
bool MatConnection::open()
{

   //struct sockaddr_in servAddr;
   // socket creation
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0)
   {
      printf("opening socket failed!\n");
      return false;
   }


   memset(&servaddr, 0, sizeof(servaddr));
   // use local server port
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //本地回环
   servaddr.sin_port = htons(6668);
   
    //主动连接服务器
    if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        printf("connecting to controller with address 6668 failed!\n");
        return false;
    }

}

//******************************************************************************
void MatConnection::close()
{

   if (isOpen())
   {
      ::close(sockfd);
	sockfd = -1;
   }
   else
	printf("已经断开\n");


   
}

//******************************************************************************
bool MatConnection::isOpen() const
{
   return (sockfd >= 0);
}

//******************************************************************************
int MatConnection::receive(char *buffer, int maxSize)
{
   if (isOpen())
   {
      return recv(sockfd, buffer, maxSize, 0);
   }
   else
	return -1;
}
  
//******************************************************************************
bool MatConnection::sendto(const char* buffer, int size)
{
   if (isOpen())
   {
      int num = send(sockfd, buffer, size, 0);
      if (num == size)
      {
         return true;
      }
      else
	return false;
   }
   else
	printf("连接断开\n");

} 



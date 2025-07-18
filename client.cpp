#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

std::string get_Message(){
  std::string message;
  std::cout << "Enter message: ";
  std::cin >> message;
  return message;
}

int main(){
  //Create socket
  //AF_INET = USE IPv4
  //SOCK_STREAM = USE TCP
  //0 USE DEFAULT PROTOCOL FOR TCP
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  //Check if socket could connect
  if (sock == -1){
    std::cerr << "Could not create socket\n";
    return 1;
  };


  sockaddr_in server_addr;
  memset(&server_addr,0,sizeof(server_addr)); //Zero out the struct
  server_addr.sin_family = AF_INET; //IPv4
  server_addr.sin_port = htons(54000); //Port 54000 converted to network byte order
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP Adress



  int connection_result = connect(sock, (sockaddr*)&server_addr, sizeof(server_addr));
  if (connection_result == -1) {
    std::cerr << "Connection failed\n";
    return -1;
  }

  std::cout << "Connected to server\n";

  while(true){
    std::string msg;
    std::cout << "Enter message: ";
    std::getline(std::cin, msg);
    int bytes_sent = send(sock, msg.c_str(), msg.size(), 0);
    if(bytes_sent == -1){
      std::cerr << "Failed to send message";
    } 
  };

  //receive message from server
  char buffer[4096];
  //recv reads data from sock, into buffer, max number of bytes to read, flag 0 for default behaviour
  int bytes_received = recv(sock, buffer, sizeof(buffer), 0);
  if (bytes_received > 0){
    std::cout << "Sever returns: " << std::string(buffer, bytes_received) << "\n";
  } else {
    std::cerr << "Failed to receive message\n";
  }


  close(sock);
  return 0;
};


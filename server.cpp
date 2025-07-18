#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <math.h>
#include <thread>
#include <algorithm>

std::vector<int> client_sockets;
std::mutex client_mutex;

void handle_client(int client_sock){
  char buffer[4096];
  while(true){
    memset(buffer, 0 ,sizeof(buffer));
    int bytes_received = recv(client_sock, buffer, sizeof(buffer), 0);
    if(bytes_received == -1){
      std::cerr << "Error receiving data\n";
      break;
    }
    if(bytes_received == 0){
      std::cout << "Client Disconnected\n";
      break;
    }


    std::string msg(buffer, bytes_received);
    std::cout << "Received from client " << client_sock << ": " << msg << "\n";

    {
    std::lock_guard<std::mutex> lock(client_mutex);

    for (int sock : client_sockets){
    if (sock != client_sock){
      send(sock, msg.c_str(), msg.size(), 0);
      }
    }
  
  }
}

close(client_sock);

  {
    std::lock_guard<std::mutex> lock(client_mutex);

    client_sockets.erase(
      std::remove(client_sockets.begin(), client_sockets.end(), client_sock),
      client_sockets.end()
    );

  }

};

void accept_client (int serverSocket){
  //Accept client connection
  sockaddr_in client_addr;
  socklen_t client_size = sizeof(client_addr);
  int client_sock = accept(serverSocket, (sockaddr*)&client_addr, &client_size);
  // socket descriptior client_sock
  // accept(socket, sockaddr, len of struct)
  if(client_sock == -1){
    std::cerr << "Failed to accept cleint";
    return;
  }
  
  std::lock_guard<std::mutex> lock(client_mutex);
  client_sockets.push_back(client_sock);

  std::thread(handle_client, client_sock).detach();

  std::cout << "New client connected\n";
};


int main(){
  int server_sock = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in server_addr;
  memset(&server_addr,0,sizeof(server_addr)); //Zero out the struct
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(54000);
  server_addr.sin_addr.s_addr = INADDR_ANY; //Listen on all interfaces(localhost, LAN etc)


  //Assign IP and port to socket
  bind(server_sock, (sockaddr*)&server_addr, sizeof(server_addr));
  //Listen for incoming connections
  listen(server_sock, SOMAXCONN);


  while(true){
    accept_client(server_sock);
  };


  close(server_sock);

  return 0;
};

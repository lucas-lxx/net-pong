#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#define READ_BUFFER_SIZE 1024

bool interrupt_game(char buffer[READ_BUFFER_SIZE]) {
  std::cout << "into interrupt game\n";
  if (buffer[0] == '#') return true;
  return false;
}

int main() {
  const int PORT = 8888;
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == 0) {
    std::cerr << "Socket failed!\n" ;
    return -1;
  }

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);
  int addrlen = sizeof(address);
  if (bind(serverSocket, (struct sockaddr*)&address, addrlen) < 0) {
    std::cerr << "Bind failed!\n";
    close(serverSocket);
  }

  if (listen(serverSocket, 3) < 0) {
    std::cerr << "Listen failed!\n";
    close(serverSocket);
    return -1;
  }

  while (true) {
    int new_socket = accept(serverSocket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
      std::cerr << "Accept failed!\n";
      close(serverSocket);
      return -1;
    }

    while (true) {
      char buffer[READ_BUFFER_SIZE] = {0};
      int valread = read(new_socket, buffer, READ_BUFFER_SIZE);
      if (valread < 0) {
        std::cerr << "Error reading from buffer!\n";
        close(new_socket);
        continue;
      }
      if (interrupt_game(buffer)) break;
      
      std::cout << "Message from client: " << buffer << '\n';
      std::string response = "Hello from server!\n";
      send(new_socket, response.c_str(), response.size(), 0);
    }
    close(new_socket);
  }
  close(serverSocket);
}
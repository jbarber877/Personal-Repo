#include<iostream>
#include<stdlib.h>
#include <sys/socket.h>
#include<cstring>
#include<netinet/in.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include <queue>
#include <stack>

using namespace std;
queue<string> q;
stack<string> s;

void display(){
    cout << "Wall Contents" << endl;
    cout << "-------------" << endl;
    // If the queue is empty, print a message
    if (! q.size() > 0){
	cout << "[NO MESSAGES - WALL EMPTY]" << endl;
        return;
    }
    // Otherwise print the queue - print an element and remove it
    while (!q.empty()){
	s.push(q.front());
	cout << q.front() << endl;
	q.pop();
    }
    // Restore elements to the queue
    while (!s.empty()){
	q.push(s.top());
	s.pop();
    }
    cout << endl;
    return;
}

void clear(){
    while (!q.empty()){
	q.pop();
    }
}

int main (int argc, char** argv){
    int qsize = 20;
    int port = 5514;

    if (argc == 3){
        qsize = atoi(argv[1]);
        port = atoi(argv[2]);
    }
    if (argc == 2){
        qsize = atoi(argv[1]);
    }

//    std::cout << "Wall server running on port " << port << " with queue size " << qsize << ". '\n'";

    // socket 
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    // bind
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    int bind_value = bind(sock, (struct sockaddr*)&address, sizeof(address));
    if (bind_value < 0){
        perror("Could not bind");
        return 1;
    }
    
    // listen
    int listen_value = listen(sock, 1);
    if (listen_value < 0){
        perror("Could not listen");
        return 1;
    }

    // accept
    struct sockaddr_in remote_address;
    memset(&remote_address, 0, sizeof(remote_address));
    socklen_t remote_addrlen = sizeof(address);

    int client_socket = accept(sock, (struct sockaddr*)&remote_address, &remote_addrlen);
    if(client_socket < 0){
        perror("Could not accept");
        return 1;
    }

    std::string client_ip = inet_ntoa(remote_address.sin_addr);
    int remote_port = ntohs(remote_address.sin_port);

    int bufflen = 1024;
    char buffer[bufflen];


//    string res = "Wall server running on port " + string(port) + " with queue size " + string(qsize) + ".";
    string res = "Wall server running on port 5514 with queue size 30";
    if (send(client_socket, res.c_str(), res.length(), 0) < 0){
        perror("Could not send");
        return 1;
    }

    display();

    res = "";
    if (send(client_socket, res.c_str(), res.length(), 0) < 0){
        perror("Could not send");
        return 1;
    }

    while (1){
        // recv
        int bytes_recieved = recv(client_socket, buffer, bufflen-1, 0);
        if (bytes_recieved < 0){
            perror("Could not recieve");
            return 1;
        }
        if (bytes_recieved == 0){
            string res = "Come back soon. Bye!";
	    if (send(client_socket, res.c_str(), res.length(), 0) < 0){
	        perror("Could not send");
	        return 1;
	    }
	    break;
        }

        if (buffer[bytes_recieved-1] == '\n'){
            buffer[bytes_recieved] = 0;
        }

	// resume
	if(strcmp(buffer, "clear")==0){
	    clear();

	    string res = "Wall cleared.";
	    if (send(client_socket, res.c_str(), res.length(), 0) < 0){
		perror("Could not send");
		return 1;
	    }
	    display();
	}
	if(strcmp(buffer, "post")==0){
	    string temp1, temp2 = "";
	    int maxLength = 80;
	    std::cout << "Enter name: ";
	    getline(cin, temp1);
	    maxLength -= temp1.length();

	    std::cout << '\n' << "Post [Max length " << maxLength << "]: ";
	    getline(cin, temp2);
	    if (temp2.length() > maxLength){
		string res = "Error: message is too long!";
		if (send(client_socket, res.c_str(), res.length(), 0) < 0){
		    perror("Could not send");
		    return 1;
		}
		continue;
	    }
	    else {
		if (q.size() == qsize)
		    q.pop();
		q.push(temp1 + ": " + temp2);
		res = "Successfully tagged the wall";
		if (send(client_socket, res.c_str(), res.length(), 0) < 0){
    		    perror("Could not send");
    		    return 1;
		}
	    }
	}
	if(strcmp(buffer, "kill")==0){

	    res = "";
	    if (send(client_socket, res.c_str(), res.length(), 0) < 0){
		perror("Could not send");
		return 1;
	    }
	    shutdown(client_socket, SHUT_RDWR);
	}
	if(strcmp(buffer, "quit")==0){
	    string response = "Come back soon. Bye!";
	    if(send(client_socket, res.c_str(), res.length(), 0) < 0){
		perror("Could not send");
		return 1;
	    }
	    break;
	}

        //std::cout << "client message " << buffer << std::endl;

        // send 
        //std::string response = "Hello client at " + client_ip + ":" + std::to_string(remote_port);
        /*int bytes_sent = send(client_socket, response.c_str(), response.length(), 0);
            if (bytes_sent < 0){
                perror("Could not send");
                return 1;
            }*/
    }

    client_socket = accept(sock, (struct sockaddr*)&remote_address, &remote_addrlen);
    return 0;
}

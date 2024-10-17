
#define _GNU_SOURCE
#define WHITESPACE " \t\n"
#define MAX_COMMAND_SIZE 255    // The maximum command-line size

#define MAX_NUM_ARGUMENTS 32

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

void print_Error()
{
  char error_message[30] = "An error has occurred\n";
  write(STDERR_FILENO, error_message, strlen(error_message)); 
}

int main( int argc, char * argv[] )
{

  char * command_string = (char*) malloc( MAX_COMMAND_SIZE );
  FILE *fp = NULL;

  if( argc > 2) 
  {
    print_Error();
    exit(1);
  }

  if( argc == 2) 
  {
    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
      print_Error(); 
      exit(1);
    }
  }

  while( 1 )
    {
      // Print out the msh prompt
      //printf ("msh> ");
      if( fp != NULL) 
      {
        if( !fgets(command_string, MAX_COMMAND_SIZE, fp))
        {
          if( feof(fp)) 
          {
            fclose(fp);
            break;
          }
        continue;
        }
      } 
      else
      {
        printf("msh> "); // basically, if we are not inside of a file, we are 
                         //just doing with the msh-example does 
          if(!fgets(command_string, MAX_COMMAND_SIZE, stdin)) 
          {
            if( feof(stdin)) // I feel like this saved some of my code 
              {
                break;
              }
            continue;
          }
      }
        // Read the command from the commandi line.  The
        // maximum command that will be read is MAX_COMMAND_SIZE
        // This while command will wait here until the user
        // inputs something.
      //while( !fgets (command_string, MAX_COMMAND_SIZE, stdin) );

        /* Parse input */
      char *token[MAX_NUM_ARGUMENTS];

      int token_count = 0;                                 
                                                              
        // Pointer to point to the token
        // parsed by strsep
      char *argument_pointer;                                         
                                                              
      char *working_string  = strdup( command_string );                

        // we are going to move the working_string pointer so
        // keep track of its original value so we can deallocate
        // the correct amount at the end
        
      char *head_ptr = working_string;
        
        // Tokenize the input with whitespace used as the delimiter
      while ( ( (argument_pointer = strsep(&working_string, WHITESPACE ) ) != NULL) &&
              (token_count<MAX_NUM_ARGUMENTS))
      {
        token[token_count] = strndup( argument_pointer, MAX_COMMAND_SIZE );
        if( strlen( token[token_count] ) == 0 )
        {
          continue;
          //token[token_count] = NULL;
        }
          token[token_count] = strndup(argument_pointer, MAX_COMMAND_SIZE);
          token_count++;
      }
      token[token_count] = NULL;
        // Now print the tokenized input as a debug check
      
      //since the execvp() seems to not detect the 'quit' and 'exit' command, we must check the user input
      // this wil detcted if the user didnt just press enter "whitespace

      if( token[0] == NULL)
      {
        //free( head_ptr);
        continue;
      }

      if( strcmp(token[0], "quit") == 0) // if the user will type in 'quit', the program will exit(0);
      {
        free( head_ptr); // i think this finally helps with the problem of having to type 'quit' or 'exit' multiple times
        exit(0);
      }
      
      if( strcmp(token[0], "exit") == 0 && token[1] != NULL) // if the user will type in 'exit', the program will exit(0);
      {
        print_Error();
        exit(0);
      }


      if( strcmp(token[0], "exit") == 0) // if the user will type in 'exit', the program will exit(0);
      {
        free( head_ptr);
        exit(0);
      }

      if( strcmp(token[0], "cd") == 0)
      {
        if( token[1] == NULL)
        {
          free(head_ptr);
          print_Error();
          continue;
        }
        else if( token[2] != NULL)
        {
          free(head_ptr);
          print_Error();
          continue;
        }
        else if( chdir(token[1]) == -1)
        {
          print_Error();
          free(head_ptr);
          continue;  
        }
        continue;
      }

      int num_of_redirect = 0;
      for( int i = 0; i < token_count; i++)
      {
        if( strcmp(token[i], ">") == 0)
        {
          num_of_redirect++;
          if( num_of_redirect > 1)
            {
              print_Error();
              exit(0);
            }
        }
      }

      if( strcmp(token[0], ">") == 0){
        print_Error();
        exit(0);
      }

      pid_t child_pid = fork(); // taken from in class example, this creates a child and we will use that child

      if( child_pid == -1) // this will check if the child is created, and if not, we will throw the print_Error function created
      {
        print_Error();
        exit(1); // from further reaserach, it seems that if you use exit(1), it indicates an error
                // if you use exit(0), it means you exit the system without error
      }
      else if( child_pid == 0) // we have successfully made it into the child and can execute from the command line
      {

         if( strstr(token[0], ".sh") != NULL)
         {
            if( strstr(token[0], "/") == NULL)
            {
              print_Error();
              exit(0); 
            }
          }
        int fd;
        // pretty cool that I am using some library defined variables 
        for( int i = 0; i < token_count; i++) 
        {
            if( token[i] != NULL && strcmp(token[i], ">") == 0) 
            {
                if( token[i + 1] == NULL) 
                {
                    print_Error();
                    exit(1);
                }
                if( token[i +2] != NULL && strcmp(token[i+2], ">") != 0)
                {
                  print_Error();
                  exit(1);
                }
                fd = open(token[i + 1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
                if( fd == -1) 
                {
                    print_Error(); // Error opening file
                    exit(1);
                }
                // Redirect stdout to the file
                if( dup2(fd, STDOUT_FILENO) == -1) 
                {
                    print_Error();
                    exit(1);
                }
                close(fd);
                token[i] = NULL;
                token[i + 1] = NULL; // Remove file name from arguments
                break;
            }
        }
        //printf("you are inside the child\n");
        if( execvp(token[0], token) == -1)
        {
          print_Error();
          exit(0);
        }
        // i pass in the tokenized info from the command line
      }
      else
      {
        int status;
        // if you do not use waitpid(), the code will completely skip the else if(child_pid ==0)
        waitpid(child_pid, &status, 0); // will wait until we are in the child? adding this fixed my code
                                        // ah so this waitpid is for waiting for the child process to finish
        // this line was testing if the fork() was working and where we were ending up when forking
        //printf("You are in the Parent and the PPID is %u\n", getppid());
      }

      free( head_ptr);

    }

  return 0;
}

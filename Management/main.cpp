#include <iostream>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

int spawn (char* program, char** arg_list)
{
  pid_t child_pid;
  /* Duplicate this process. */
  child_pid = fork ();
  if (child_pid != 0)
       /* This is the parent process. */
      return child_pid;
  else {
       /* Now execute PROGRAM, searching for it in the path. */
       execvp (program, arg_list);
       /* The execvp function returns only if an error occurs. */
       fprintf (stderr, "an error occurred in execvp\n");
       abort ();
   }
}

int main()
{
    char* arg_list[] = {
    NULL
    };
    spawn ("/home/thiago/Milton shared Memory 26-02-2014/Decisao/bin/Debug/Decisao", arg_list);
    spawn ("/home/thiago/planning/bin/Debug/planning",arg_list);
    return 0;
}

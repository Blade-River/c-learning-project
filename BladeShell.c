#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <dirent.h>

void tokenization(char *command, char *argsv[]){
  int i = 0;
  int argc = 0;
  for (int k = 0; k < 100; k++) argsv[k] = NULL;

  while (command[i] != '\0'){
    while (command[i] == ' ') i++;
    if (command[i] == '\0') break;

    argsv[argc] = &command[i];
    int write = i;
    int singleq = 0;
    int doubleq = 0;

    while (command[i] != '\0'){
      char c = command[i];

      if (c == ' ' && !singleq && !doubleq){
        break;
      }
      if (c == '\'' && !doubleq){
        singleq = !singleq;
        i++;
        continue;
      }
      if (c == '"' && !singleq){
        doubleq = !doubleq;
        i++;
        continue;
      }
      if (c == '\\'){
        if (singleq){
          command[write++] = c;
          i++;
          continue;
        }
        if (doubleq){
          char next = command[i+1];
          if (next == '"' || next == '\\' || next == '$' || next == '`'){
            command[write++] = next;
            i += 2;
          } else {
            command[write++] = c;
            i++;
          }
          continue;
        }
        char next = command[i+1];
        if (next != '\0'){
          command[write++] = next;
          i += 2;
        } else {
          i++;
        }
        continue;
      }

      command[write++] = c;
      i++;
    }

    int endedOnSpace = (command[i] == ' ');   
    command[write] = '\0';
    argc++;
    if (endedOnSpace) i++;                
  }

  argsv[argc] = NULL;
}
void normlization(char *command){
  int read = 0;
  int write = 0;
  int singleq = 0;
  int doubleq = 0;

  while (command[read] != '\0'){
    char c = command[read];

    
    if (c == '\'' && !doubleq){
      singleq = !singleq;
      read++;
      continue;
    }

    
    if (c == '"' && !singleq){
      doubleq = !doubleq;
      read++;
      continue;
    }

    if (c == '\\'){
      if (singleq){
        
        command[write++] = c;
        read++;
        continue;
      }

      if (doubleq){
        char next = command[read+1];
        if (next == '"' || next == '\\' || next == '$' || next == '`'){
          command[write++] = next;
          read += 2;
        } else {
        
          command[write++] = c;
          read++;
        }
        continue;
      }

      char next = command[read+1];
      if (next != '\0'){
        command[write++] = next;
        read += 2;
      } else {
        read++; 
      }
      continue;
    }

    
    if (c == ' ' && !singleq && !doubleq){
      while (command[read] == ' ') read++;
      command[write++] = ' ';
      continue;
    }

    command[write++] = c;
    read++;
  }

  command[write] = '\0';
}
char *fullpathS(char *command){
  char *path = getenv("PATH");
  char *copy = strdup(path);
  char *dir = strtok(copy,":");
        while (dir != NULL){
          static char fullpoint[1024]; 
          strcpy(fullpoint,dir);
          strcat(fullpoint,"/");
          strcat(fullpoint,command);
          if (access(fullpoint, X_OK) == 0){
            
            free(copy);
            return fullpoint;
          }
          dir = strtok(NULL,":");
        }
        free(copy);
        return NULL;
}
char const *cmdass[] = {
  "echo",
  "exit",
  "BladeRiver",
  "complete",
  NULL
};
static int completing_first_word = 0;
char *options(const char *cmds, int state) {
  
  static int O;
  static DIR *dp = NULL;
  static char *cpy = NULL;
  static char *dirs = NULL;
  struct dirent *de;
  if (completing_first_word){
  if (state == 0) {
    O = 0;
    
    if (dp) {
      closedir(dp);
      dp = NULL;
    }
    if (cpy) {
      free(cpy);
      cpy = NULL;
    }
    dirs = NULL;

    char *paths = getenv("PATH");
    if (paths) {
      cpy = strdup(paths);
      if (cpy) {
        dirs = strtok(cpy, ":");
      }
    }
  }
  
  while (dirs || dp) {
    if (dp == NULL) {
      dp = opendir(dirs);
      if (dp == NULL) {
        dirs = strtok(NULL, ":");
        continue;
      }
    }

    while ((de = readdir(dp)) != NULL) {
      if (strncmp(de->d_name, cmds, strlen(cmds)) == 0) {
        return strdup(de->d_name);
      }
    }

  
    closedir(dp);
    dp = NULL;
    dirs = strtok(NULL, ":");
  }
  
  
  if (cpy) {
    free(cpy);
    cpy = NULL;
  }
  
  while (cmdass[O]) {
    if (strncmp(cmdass[O], cmds, strlen(cmds)) == 0) {
      return strdup(cmdass[O++]); 
    }
    O++;
  }}

  static DIR *dir = NULL;
  static char *d = NULL;
  static char *surf = NULL;
  static int a = 0;

  if (state == 0) {
    if (dir) {
      closedir(dir);
      dir = NULL;
    }

    free(d);
    d = NULL;
    surf = NULL;
    d = strdup(cmds);

    if (!d)
      return NULL;
    char *pref = strrchr(d, '/');

    if (pref != NULL) {
      *pref = '\0';
      surf = pref + 1;
      dir = opendir(d);
      a = 1;
    }
    else {
      surf = d;
      dir = opendir(".");
      a = 0;
    }
  }

  if (dir) {
    while ((de = readdir(dir)) != NULL) {
      if (strcmp(de->d_name, ".") == 0 ||
        strcmp(de->d_name, "..") == 0) {
        continue;
      }
      if (strncmp(de->d_name,surf,strlen(surf)) != 0) {
        continue;
      }
      if (a == 1) {
        char *new_d = NULL;
          if (de->d_type == DT_DIR){
            asprintf(&new_d,"%s/%s/",d,de->d_name);
            rl_completion_append_character = '\0';
          }
          else{
            asprintf(&new_d,"%s/%s",d,de->d_name);
            rl_completion_append_character = ' ';
          }
          return new_d;
      }
      if (de->d_type == DT_DIR){
        rl_completion_append_character = '\0';
        char *n_d = NULL;
        asprintf(&n_d,"%s/",de->d_name);
        return n_d;
      }
      else{
        rl_completion_append_character = ' ';
      }
      return strdup(de->d_name);
    }

    closedir(dir);
    dir = NULL;
}

return NULL;
}
char **completion(const char *cmds, int start, int end) {
  rl_attempted_completion_over = 1;
  if (start == 0)
    completing_first_word = 1;
  else
    completing_first_word = 0;
  return rl_completion_matches(cmds, options);
}
int main(int argc, char *argv[]) {
  char *home = getenv("HOME");
  
  setbuf(stdout, NULL);
  rl_attempted_completion_function = completion;
  
  while (1 == 1)
  {
    int a = 1;
    char *command;
    command = readline("$ ");
    char *args[100];
    for (int k = 0; k < 100; k++){
    args[k] = NULL;
  }
      char commands[1024];
    strcpy(commands,command);
    tokenization(commands,args);
    normlization(command);
    if (strcmp(args[0],"exit") == 0){
      break;
    }
    else if (strcmp(args[0],"echo") == 0)
    {
      char toprint[1024];
      toprint[0] = '\0';          
      int k = 1;
      while (args[k] != NULL && (strcmp(args[k], ">") != 0 && strcmp(args[k], "1>") != 0 && strcmp(args[k], "2>") != 0 && strcmp(args[k],">>") != 0 && strcmp(args[k],"1>>") != 0 && strcmp(args[k],"2>>") != 0)){
        if (k > 1) strcat(toprint, " ");
          strcat(toprint, args[k]);
          k++;
          } 
      int l = 0;
      int f = 0;
      while (args[l] != NULL){
        if (strcmp(args[l],">") == 0 || strcmp(args[l],"1>") == 0){
          if(args[l+1] != NULL){
            if (access(args[l+1],F_OK) == 0){
              FILE *fptr;
              fptr = fopen(args[l+1],"w");
              fprintf(fptr, "%s\n", toprint);
              fclose(fptr);
              f = 1;
              break;
            }
            else{
              FILE *fptr;
              fptr = fopen(args[l+1],"w");
              fprintf(fptr, "%s\n", toprint);
              fclose(fptr);
              f = 1;
              break;
            }
            
          }
        }
        if (strcmp(args[l], "2>") == 0){
              f = 0;
              FILE *fptr;
              fptr = fopen(args[l+1],"w");
              fprintf(fptr, "%s", "");
              fclose(fptr);
              break;
        }
        if (strcmp(args[l],">>") == 0 || strcmp(args[l],"1>>") == 0){
          if (args[l+1] != NULL){
            if(access(args[l+1],F_OK) == 0){
              FILE *fptr;
              fptr = fopen(args[l+1],"a");
              fprintf(fptr, "%s\n", toprint);
              fclose(fptr);
              f = 1;
              break;
            }
            else{
              FILE *fptr;
              fptr = fopen(args[l+1],"w");
              fprintf(fptr, "%s\n", toprint);
              fclose(fptr);
              f = 1;
              break;
            }
          }
        }
        l++;
      }
      if (f == 0){
        printf("%s\n", toprint);
      }
    }
    else if (strcmp(args[0],"pwd") == 0)
    {
      char path[1024];
      if(getcwd(path,sizeof(path)) != NULL){
        int l = 0;
      int f = 0;
      while (args[l] != NULL){
        if (strcmp(args[l],">") == 0 || strcmp(args[l],"1>") == 0){
          if(args[l+1] != NULL){
            if (access(args[l+1],F_OK) == 0){
              FILE *fptr;
              fptr = fopen(args[l+1],"w");
              fprintf(fptr, "%s\n", path);
              fclose(fptr);
              f = 1;
              break;
            }
            else{
              FILE *fptr;
              fptr = fopen(args[l+1],"w");
              fprintf(fptr, "%s\n", path);
              fclose(fptr);
              f = 1;
              break;
            }
            
          }
        }
        if (strcmp(args[l],">>") == 0 || strcmp(args[l],"1>>") == 0){
          if(args[l+1] != NULL){
            if (access(args[l+1],F_OK) == 0){
              FILE *fptr;
              fptr = fopen(args[l+1],"a");
              fprintf(fptr, "%s\n", path);
              fclose(fptr);
              f = 1;
              break;
            }
            else{
              FILE *fptr;
              fptr = fopen(args[l+1],"a");
              fprintf(fptr, "%s\n", path);
              fclose(fptr);
              f = 1;
              break;
            }
            
          }
        }
        l++;
      }
      if (f == 0){
        printf("%s\n",path);
      }
        
      }
      else{
        perror("getcwd");
      }
    }
    else if (strcmp(args[0],"cd") == 0){
      
      if (strcmp(command + 3,"~") == 0){
        if(chdir(home) == 0){

        }
      }
      else if (chdir(command + 3) == 0 ){

      }
      else{
        printf("cd: %s: No such file or directory\n",command + 3);
      }
    }
    else if (strcmp(args[0],"type") == 0){
      if (strcmp(args[1],"exit") == 0 ||  strcmp(args[1],"echo") == 0 || strcmp(args[1],"type") == 0 || strcmp(args[1],"pwd") == 0 || strcmp(args[1],"cd") == 0 || strcmp(args[1],"complete")){
        printf("%s is a shell builtin\n",command + 5);
      }
      else{
        char *fpaths = fullpathS(command + 5);  
        if (fpaths){
          printf("%s is %s\n",command + 5,fpaths);
        }
        else {
          char buffer[1024];

          snprintf(buffer, sizeof(buffer), "%s: not found\n", command + 5);
          printf("%s",buffer);
        }
      }
    } 
    else{
      

      char *fpath = fullpathS(args[0]);
      if (fpath == NULL)
      {
      printf("%s: command not found\n",command);
      continue;
      }
      int redirectindex = -1;
      char *outfile = NULL;
      int r = 0;
      int a = 0;
      for (int k = 0; args[k] != NULL; k++) {
        if (strcmp(args[k],">") == 0 || strcmp(args[k],"1>") == 0){
          if (args[k + 1] != NULL){
            redirectindex = k;
            outfile = args[k + 1];
            r = 0;
            a = 0;
            break;
            
          }
        }
        if (strcmp(args[k],">>") == 0 || strcmp(args[k],"1>>") == 0){
          if (args[k + 1] != NULL){
            redirectindex = k;
            outfile = args[k + 1];
            r = 0;
            a = 1;
            break;
          }
        }
        if (strcmp(args[k],"2>>") == 0){
          if (args[k+1] != NULL){
            redirectindex = k;
            outfile = args[k + 1];
            r = 1;
            a = 1;
            break;
          }
        }
        if (strcmp(args[k],"2>") == 0){
          if (args[k+1] != NULL){
            redirectindex = k;
            outfile = args[k+ 1];
            r = 1;
            a = 0;
            break;
          }
        }
      }
      if (redirectindex != -1){
       args[redirectindex] = NULL;
      }

      pid_t pid = fork();
      if (fpath){
        if (pid == 0){
          if (outfile != NULL){
            FILE *fptr;
            if (a != 1){
            fptr = fopen(outfile,"w");
            }
            else{
              fptr = fopen(outfile,"a");
            }
            if(fptr == NULL){
              perror("fopen");
              exit(0);
            }
            if (r == 0){
            dup2(fileno(fptr), STDOUT_FILENO);}   
            if(r == 1){
              dup2(fileno(fptr), STDERR_FILENO);
            }
            fclose(fptr);
        }
          execv(fpath,args);
          perror("execv");
          exit(0);
        }
        wait(NULL);
      }
      
    }
  }
  
  
  return 0;
}

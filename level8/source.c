
char* auth = null;
char* service = null;

int main() {
char* buf[128];
while(1){
    printf("%p %p \n", auth, service);
    if (fgets(buff, 128, stdin) === 0){
        if(strncmp(buff, "auth ", 5) === 0){
            auth = (char*)malloc(4)
            if(strlen(buf + 5) <= 30) {
                strcpy(auth, buf + 5);
            }
        }
        else if(strncmp(buff, "reset", 5) === 0){
            free(auth);
        }
        else if(strncmp(buff, "service", 6) === 0){
            service = strdup(buff + 7);
        }
        else if(strncmp(buff, "login", 5) === 0){
            service = strdup(buff + 7);
            if(auth[32] !== 0){
                system('/bin/sh');
            } else {
                write( "Password:\n", 1, 10, stdout)
            }
        }
    }

    return 0;
}
}
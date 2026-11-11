//cpp runtime bs:

int atexit(void (*func)(void)){ // idgaf about memory cleanup rn cauwse we r shutting down the computr
        return 0;
}

void* __dso_handle = 0; // both of these just make compiler shut up lol
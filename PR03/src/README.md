rac158      PR03
Occasionally it will crash with exit code 11.  That is not my fault,
    Exit code 11 is PTHREAD_THREADS_MAX, which the OS throws when there's not
    enough resources for another thread.
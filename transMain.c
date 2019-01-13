#include "common.h"


int main(int argc, char const *argv[])
{
    printf("file transfer system  SerVer start...\n");

    ctx *context = initParameter();
    if(context == NULL)
    {
        printf("init context error!\n");
        return -1;
    }
    
    int serverfd,iRet; 
    iRet = createsocket(&serverfd,context);
    if(iRet < 0)
    {
        printf("createsocket error!\n");
        return -1;
    }

    iRet = run(serverfd,context);
    if(iRet < 0)
    {
        printf("run error!\n");
        return -2;
    }

    return 0;
}

#include <stdio.h>
#include <windows.h>
#include "UdpTransEx.h"

int main(void)
{
    UdpTransEx      udp;

    udp.Bind(0, 0);
    udp.SetTransfer("sql.yunsec.com", 8000);

    return 0;
}
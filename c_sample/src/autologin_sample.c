#include "stdio.h"




/* modify etc/inittab
ttyS0::respawn:/bin/getty -L ttyS0 115200 vt102 -n -l /sbin/autologin */

int main(int argc, char *argv[])
{
  execlp( "login", "login", "-f", "root", 0);
  return 0;
}



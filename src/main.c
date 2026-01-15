#include <rtems.h>

#include <stdio.h>
#include <stdlib.h>


rtems_task Init(rtems_task_argument ignored)
{
    printf("Hello Rtems\n");


    exit(0);
}

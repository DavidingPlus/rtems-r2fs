#include <rtems.h>

#include <stdio.h>
#include <stdlib.h>

#include "unit_test.h"


rtems_task Init(rtems_task_argument ignored)
{
    runAllTests();


    exit(0);
}

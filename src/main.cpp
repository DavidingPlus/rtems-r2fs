#include <rtems.h>

#include <iostream>


extern "C" rtems_task Init(rtems_task_argument ignored)
{
    std::cout << "Hello Rtems " << __cplusplus << std::endl;


    exit(0);
}

/* Read a text file and display it */
#include "ff.h"
#include <stdio.h>

FATFS FatFs;   /* Work area (filesystem object) for logical drive */

int main (void)
{
    printf("Hello");

    FIL fil;        /* File object */
    char line[100]; /* Line buffer */
    FRESULT fr;     /* FatFs return code */


    /* Register work area to the default drive */
    f_mount(&FatFs, "", 1);

    /* Open a text file */
    fr = f_open(&fil, "message.txt", FA_READ);
    if (fr) return (int)fr;

    /* Read every line and display it */
    // while (f_gets(line, sizeof line, &fil)) {
    //     printf(line);
    // }
    /* Close the file */
    f_close(&fil);

    return 0;
}

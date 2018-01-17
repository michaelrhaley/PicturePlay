#include <stdio.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    //standard error checking
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    
    //open the desired file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }
    
    uint8_t buffer[512];
    int n = 0;
    FILE *image = NULL;
    
    if(file != NULL);
    {
        while(fread(buffer, 512, 1, file))
        {
                if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) ==  0xe0) //
                    {
                        char filename[8];
                        sprintf(filename, "%03d.jpg", n);
                        image = fopen(filename, "w");
                        n++;
                    }
                    
                    if(image != NULL)fwrite(buffer, 512, 1, image);
        }
    }
    if(image != NULL)fclose(image);
    
    fclose(file);
    
    return 0;
}
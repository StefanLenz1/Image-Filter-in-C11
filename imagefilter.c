#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>

#include "filters.h"

int checkInput(int argc, char *argv[], const char filters[]);
void selectFilter(const int filter, int height, int width, RGBTRIPLE image[height][width]);

int main(int argc, char *argv[])
{
    const char filters[] = "mbegrn"; // Erlaubte Filter flags
    const int filter_flag = checkInput(argc, argv, filters);
    if (filter_flag == 0)
    {
        return 1;
    }

    char input_image_source[100] = "images/";
    char *infile_name = argv[optind];
    strcat(input_image_source, infile_name);

    char output_image_source[100] = "output/";
    char *output_name = argv[optind + 1];
    strcat(output_image_source, output_name);

    FILE *inptr = fopen(input_image_source, "r");
    if (inptr == NULL)
    {
        printf("Input File konnte nicht geöffnet werden: %s.\n", input_image_source);
        return 2;
    }

    FILE *outptr = fopen(output_image_source, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        printf("Output File konnte nicht erstellt werden: %s.\n", output_image_source);
        return 3;
    }

    BITMAPFILEHEADER input_bm_file_header;
    fread(&input_bm_file_header, sizeof(BITMAPFILEHEADER), 1, inptr);

    BITMAPINFOHEADER input_bm_info_header;
    fread(&input_bm_info_header, sizeof(BITMAPINFOHEADER), 1, inptr);

    // BMP 4.0
    if (input_bm_file_header.bfType != 0x4d42 || input_bm_file_header.bfOffBits != 54 || input_bm_info_header.biSize != 40 ||
        input_bm_info_header.biBitCount != 24 || input_bm_info_header.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        printf("Falsches Dateiformat.\n");
        return 4;
    }

    int height = abs(input_bm_info_header.biHeight);
    int width = input_bm_info_header.biWidth;

    RGBTRIPLE(*image)
    [width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (image == NULL)
    {
        printf("Nicht Genug Speicher für das Bild.\n");
        fclose(outptr);
        fclose(inptr);
        return 5;
    }

    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

    for (int i = 0; i < height; i++)
    {
        fread(image[i], sizeof(RGBTRIPLE), width, inptr);

        fseek(inptr, padding, SEEK_CUR);
    }

    selectFilter(filter_flag, height, width, image);

    fwrite(&input_bm_file_header, sizeof(BITMAPFILEHEADER), 1, outptr);

    fwrite(&input_bm_info_header, sizeof(BITMAPINFOHEADER), 1, outptr);

    // Das Bild wird erstellt
    for (int i = 0; i < height; i++)
    {
        // Erstellt Reihe
        fwrite(image[i], sizeof(RGBTRIPLE), width, outptr);

        // Padding
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    free(image);
    fclose(inptr);
    fclose(outptr);
    return 0;
}

int checkInput(int argc, char *argv[], const char filters[])
{
    const char *incorrect_usage_error_message = "Korrekte Verwendung: ./imagefilter -(filter flag) input_file.bmp output_file.bmp\n";

    char filter = getopt(argc, argv, filters);
    if (filter == '?')
    {
        printf("Kein Filter flag eingegebenn");
        printf("%s", incorrect_usage_error_message);
        return 0;
    }

    if (getopt(argc, argv, filters) != -1) // Testet ob mehr als ein Filter flag vorhanden ist
    {
        printf("Zu viele Filter flags.\n");
        printf("%s", incorrect_usage_error_message);
        return 0;
    }

    if (argc != optind + 2) // Unzulässige Anzahl an Argumenten
    {
        printf("Falsche Anzahl an Argumenten.\n");
        printf("%s", incorrect_usage_error_message);
        return 0;
    }

    return filter;
}

void selectFilter(const int filter, int height, int width, RGBTRIPLE image[height][width])
{
    switch (filter)
    {
    case 'g':
        grayscale(height, width, image);
        break;
    case 'r':
        reflect(height, width, image);
        break;
    case 'b':
        blur(height, width, image);
        break;
    case 'm':
        motionBlur(height, width, image);
        break;
    case 'e':
        edges(height, width, image);
        break;
    case 'n':
        nofilter();
        break;
    }
}
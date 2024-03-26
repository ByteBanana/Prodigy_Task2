#include <stdio.h>
#include <stdlib.h>

// Function prototypes
void decryptImage(const char* inputFilePath, const char* outputFilePath, unsigned char key);

int main() {
    // Hardcode file paths and key
    const char* inputFilePath = "download_encrypted.bmp";
    const char* outputFilePath = "download_decrypted.bmp";
    unsigned char key = 123; // Same key used for encryption

    decryptImage(inputFilePath, outputFilePath, key);

    printf("Process completed. Check the output file: %s\n", outputFilePath);
    return 0;
}

void decryptImage(const char* inputFilePath, const char* outputFilePath, unsigned char key) {
    FILE* inputFile = fopen(inputFilePath, "rb");
    FILE* outputFile = fopen(outputFilePath, "wb");

    if (inputFile == NULL || outputFile == NULL) {
        fprintf(stderr, "Error opening file.\n");
        exit(1);
    }

    unsigned char header[54]; // BMP header is 54 bytes
    fread(header, sizeof(unsigned char), 54, inputFile); // Read the header
    fwrite(header, sizeof(unsigned char), 54, outputFile); // Write the header to output

    // The width and height are stored starting at byte 18 and 22 respectively, and are 4 bytes each.
    int width, height;
    fseek(inputFile, 18, SEEK_SET);
    fread(&width, 4, 1, inputFile);
    fread(&height, 4, 1, inputFile);

    // Calculate padding
    int padding = (4 - (width * 3) % 4) % 4;

    fseek(inputFile, 54, SEEK_SET); // Move the file pointer to the beginning of the bitmap data

    // Process each pixel
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            unsigned char pixel[3];
            fread(pixel, sizeof(unsigned char), 3, inputFile); // Read a pixel (3 bytes)
            
            // XOR each byte of the pixel with the key again to decrypt
            for (int k = 0; k < 3; k++) {
                pixel[k] ^= key;
            }

            fwrite(pixel, sizeof(unsigned char), 3, outputFile); // Write the decrypted pixel
        }
        // Handle padding
        unsigned char paddingBytes[3] = {0};
        fread(paddingBytes, sizeof(unsigned char), padding, inputFile);
        fwrite(paddingBytes, sizeof(unsigned char), padding, outputFile);
    }

    fclose(inputFile);
    fclose(outputFile);
}


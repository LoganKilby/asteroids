static void
InsertionSort(int *arr, int count)
{
    int i, key, j;
    for (i = 1; i < count; i++) {
        key = arr[i];
        j = i - 1;
        
        while (j >= 0 && arr[j] < key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}


static int
ReadEntireFile(file_buffer *fileBuffer, char *filename)
{
    char *buffer = 0;
    long length;
    
    FILE *fileHandle = fopen(filename, "rb");
    
    if (fileHandle)
    {
        fseek (fileHandle, 0, SEEK_END);
        length = ftell (fileHandle);
        fseek (fileHandle, 0, SEEK_SET);
        buffer = (char *)malloc(length + 1);
        if (buffer)
        {
            int bytesRead = fread(buffer, 1, length, fileHandle);
            buffer[bytesRead] = '\0';
            fileBuffer->buffer = buffer;
            fileBuffer->size = bytesRead;
        }
        else
        {
            return 0;
        }
        fclose(fileHandle);
    }
    else
    {
        // TODO: Logging
        printf("An error occured while opening the file \"%s\"\n", filename);
        return 0;
    }
    
    return 1;
}

static int
WriteEntireFile(char *buffer, int size, char *filename)
{
    FILE *fileHandle = fopen(filename, "w+");
    int error;
    if(fileHandle)
    {
        for(int i = 0; i < size; i++)
        {
            error = fputc(buffer[i], fileHandle);
        }
        
        fclose(fileHandle);
    }
    else
    {
        // TODO: Logging
        printf("An error occured while opening the file \"%s\"\n", filename);
        return 0;
    }
    
    return 1;
}

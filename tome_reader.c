#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

// Custom strlen will counte the length
int my_strlen(const char *str)
{
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}

// Custom strcpy that will cpy the string src in dest
char *my_strcpy(char *dest, const char *src)
{
    int i=0;
    while (src[i] != '\0')
    {
      	dest[i] = src[i];
	i++;
    }
    dest[i] = '\0';
    return dest;
}

// Custom strcat that will add the string src to dest
char *my_strcat(char *dest, const char *src)
{
    int i=0;
    int j=0;
    while (dest[i] != '\0')
    {
        i++;
    }
   
    while (src[j] != '\0')
    {
        dest[i] = src[j];
        i++;
        j++;
    }
    dest[i] = '\0';
    return dest;
}

// This function returns the index of the \n ,
// or -1 if no newline is found.

int end_exist(char *buff, int pos, int buff_s)
{
    while (pos < buff_s)
    {
        if (buff[pos] == '\n')
            return pos;//if a new line exist ret the pos
        pos++;
    }
    return -1;//if it didn t find \n will return -1 so i can use it later
	      //in a test!
}

char *tome_reader(int fd)
{
    static char *buffer = NULL;//will stor the value read will give it
   
    static int pos = 0;//pos will remember the position of the /n if 
		       //it exist in the != end of buffer
    
    static int bytes_in_buffer = 0;//so we will save here the nember of
				   //bytes later! 
    char *line = NULL;
    char *debt = NULL;//is what left after the \n in the buffer

    int nl_index;//the index of the new line \n
    int len;//lentgh of remaining bytes

    // the buffer if it's NULL
    if (buffer == NULL)
    {
        buffer = malloc(sizeof(char) * BUFFER_SIZE);
        if (buffer == NULL)//that will mean error happend in allocation
        {
            return NULL;//if an error hapend just return null
        }
        bytes_in_buffer = read(fd, buffer, BUFFER_SIZE);
	// int the bytes....bufer var we will storte the number of bytes 
        if (bytes_in_buffer <= 0)//if it s negative that means error(empty)
        {
            free(buffer);//error -> free buff and ret null
            buffer = NULL;
            return NULL;
        }
        pos = 0;//to start from the start of buffer next time
    }

    // Handle case where we have read all bytes in the buffer
    if (pos >= bytes_in_buffer)
    {
        bytes_in_buffer = read(fd, buffer, BUFFER_SIZE);
        if (bytes_in_buffer <= 0)
        {
            return NULL;
        }
        pos = 0;
    }

    nl_index = end_exist(buffer, pos, bytes_in_buffer);
    //nl_index will store the index of new line

    //(look the cmnts in the end_exist func)
    
    if (nl_index == -1)
    {
        // No newline found, allocate memory for the remaining content
        len = bytes_in_buffer - pos;
        line = malloc(len + 1);
        if (line == NULL)//prblm with allocating just ret null
        {
            return NULL;
        }
        
        // Copy characters to the line
        for (int i = 0; i < len; i++)
        {
            line[i] = buffer[pos + i];
        }
        line[len] = '\0';
        pos = bytes_in_buffer;

        //debt(credit) will start again
        debt = tome_reader(fd);
        if (debt != NULL)//if its null that means there is no credit
        {
            int total = my_strlen(line) + my_strlen(debt);
            char *new_line = malloc(total + 1);//to store the whole line
            if (new_line == NULL)//prblm happend return null
            {
                free(line);
                free(debt);
                return NULL;
            }
            my_strcpy(new_line, line);//copy line cuz its the first one
            my_strcat(new_line, debt);//and then add debt next to line
            free(line);
            free(debt);//free everthing
            line = new_line;//store in line everything
        }
    }
    else
    {
        // Newline founded,so we will allocate memory for content till 
	// newline
        
	len = nl_index - pos + 1;
        line = malloc(len + 1); // we allocat for the line and +1for the'\0'
        if (line == NULL)//problem happend ret null
        {
            return NULL;
        }

        // Copy characters
        for (int i = 0; i < len; i++)
        {
            line[i] = buffer[pos + i];
        }
        line[len] = '\0';
        pos = nl_index + 1;//so we mov for the next line if it exist
    }
    return line;//finally we return the result line
}

int main(int ac, char **av)
{
	int fd;
	char continu,confirm;
	char *line;

    if (ac < 2)
    {
        printf("Please mention a file\n");
        return 1;
    }

    fd = open(av[1], O_RDONLY);
    if (fd < 0)
    {
        printf(" wewewewewe Error!!!!\n");
        return 1;
    }

   do
    {
        line = tome_reader(fd);
	
	if (line == NULL)
       		break;
	
	printf("u call me !!i will show u only the 1st word noww !!\n");
	printf("%s", line);
        
	free(line);
	
	printf("if u can't handel the truth just click anyletter don't risck it!! if u are able to finish press (y) and press it (enter) to confirm!!:");
        scanf("%c%c",&continu,&confirm);

    }while(continu == 'y' && confirm =='\n');
   
    close(fd);
    return 0;
}

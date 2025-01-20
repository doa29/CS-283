#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SZ 50

void usage(char *);
void print_buff(char *, int);
int setup_buff(char *, char *, int);
int count_words(char *, int);
void reverse_string(char *, int);
void word_print(char *, int);
void capitalize_words(char *, int);
void replace_substring(char *, int, char *, char *);

int setup_buff(char *buff, char *user_str, int len) {
    int count = 0;
    char last_char = '\0';
    char *str_ptr = user_str;
    
    
    int clean_length = 0;
    int in_space = 0;
    
    while (*str_ptr != '\0') {
        if (*str_ptr != ' ' && *str_ptr != '\t') {
            clean_length++;
            in_space = 0;
        } else if (!in_space && clean_length > 0) {
            clean_length++;  // Add one space
            in_space = 1;
        }
        str_ptr++;
    }
    
    
    if (clean_length > len) {
        return -1;  // String too large
    }
    
    
    str_ptr = user_str;
    
   
    while (*str_ptr != '\0' && count < len) {
        if (*str_ptr != ' ' && *str_ptr != '\t') {
            buff[count++] = *str_ptr;
            last_char = *str_ptr;
        } else if (last_char != ' ' && last_char != '\0') {
            buff[count++] = ' ';
            last_char = ' ';
        }
        str_ptr++;
    }
    
    
    if (count > 0 && buff[count-1] == ' ') {
        count--;
    }
    
    
    for (int i = count; i < len; i++) {
        buff[i] = '.';
    }
    
    return count; 
}


int count_words(char *buff, int len) {
    int words = 0, in_word = 0;

    for (int i = 0; i < len; i++) {
        if (buff[i] != ' ' && buff[i] != '.') {
            if (!in_word) {
                in_word = 1;
                words++;
            }
        } else {
            in_word = 0;
        }
    }

    return words;
}

// Reversing the string in place
void reverse_string(char *buff, int len) {
    int start = 0, end = len - 1;
    while (start < end) {
        char temp = buff[start];
        buff[start] = buff[end];
        buff[end] = temp;
        start++;
        end--;
    }
}

// Printing words and their lengths
void word_print(char *buff, int len) {
    int word_start = -1;

    printf("Words and lengths:\n");
    for (int i = 0; i <= len; i++) {
        if ((buff[i] != ' ' && buff[i] != '.' && buff[i] != '\0') && word_start == -1) {
            word_start = i;
        } else if ((buff[i] == ' ' || buff[i] == '.' || buff[i] == '\0') && word_start != -1) {
            printf("%d. ", word_start + 1);
            for (int j = word_start; j < i; j++) {
                printf("%c", buff[j]);
            }
            printf(" (%d)\n", i - word_start);
            word_start = -1;
        }
    }
}

// Capitalizing the first letter of each word
void capitalize_words(char *buff, int len) {
    int in_word = 0;

    for (int i = 0; i < len; i++) {
        if (buff[i] != ' ' && buff[i] != '.' && !in_word) {
            // This is the main line to capitalize the first character of a word
            if (buff[i] >= 'a' && buff[i] <= 'z') {
                buff[i] -= 32;
            }
            in_word = 1;
        } else if (buff[i] == ' ' || buff[i] == '.') {
            in_word = 0;
        }
    }
}


void replace_substring(char *buff, int len, char *target, char *replacement) {
    int target_len = 0;
    while (target[target_len] != '\0') {
        target_len++;
    }

    int replacement_len = 0;
    while (replacement[replacement_len] != '\0') {
        replacement_len++;
    }


    int content_len = 0;
    while (content_len < len && buff[content_len] != '.') {
        content_len++;
    }

    int i = 0;
    while (i < content_len - target_len + 1) {
        int j = 0;
        while (j < target_len && buff[i + j] == target[j]) {
            j++;
        }
        if (j == target_len) {
            
            int remaining_len = content_len - (i + target_len);
            
            // If replacement is longer than target, check if it fits in buffer
            if (i + replacement_len + remaining_len > len) {
                remaining_len = len - (i + replacement_len);
            }

            // Move the remaining content to make space for replacement if needed
            if (replacement_len != target_len && remaining_len > 0) {
                int shift = replacement_len - target_len;
                int from = i + target_len;
                int to = i + replacement_len;
                
                if (shift > 0) {
                    // Moving content right (replacement longer than target)
                    for (int k = remaining_len - 1; k >= 0; k--) {
                        if (to + k < len) {
                            buff[to + k] = buff[from + k];
                        }
                    }
                } else {
                    // Moving content left (replacement shorter than target)
                    for (int k = 0; k < remaining_len; k++) {
                        buff[to + k] = buff[from + k];
                    }
                }
            }

            // Inserting the replacement
            for (int k = 0; k < replacement_len && (i + k) < len; k++) {
                buff[i + k] = replacement[k];
            }

            // Filling the rest with dots if ever needed
            for (int k = i + replacement_len + remaining_len; k < len; k++) {
                buff[k] = '.';
            }

            return;
        }
        i++;
    }
}

// Printing usage information
void usage(char *exename) {
    printf("%s -[h|c|r|w|x] \"sample string\" [other options]\n", exename);
    printf("\t-h: Display this help message\n");
    printf("\t-c: Count the number of words\n");
    printf("\t-r: Reverse the string\n");
    printf("\t-w: Print each word and its length\n");
    printf("\t-x: Capitalize the first letter of each word\n");
    printf("\t-x with target and replacement: Replace target with replacement\n");
}

// Prints the buffer without padding characters
void print_buff(char *buff, int len) {
    printf("Buffer: ");
    for (int i = 0; i < len; i++) {
        // Only print characters that are not padding ('.')
        if (buff[i] != '.') {
            printf("%c", buff[i]);
        }
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    char *buff;             // placeholder for the internal buffer
    char *input_string;     // holds the string provided by the user on cmd line
    char opt;               // used to capture user option from cmd line
    int rc;                 // used for return codes
    int user_str_len;       // length of user-supplied string

    // Oh, this is safe because the `argc < 2` part makes sure we have at least
    // two arguments before trying to look at `argv[1]`. In the case that the user doesn't 
    // unfortunately give enough arguments, we call `usage()` and exit, so we never actually 
    // touch `argv[1]` when it doesn't exist. This prevents crashes or weird 
    // behavior like segmentation faults (which we all hate).

    if ((argc < 2) || (*argv[1] != '-')) {
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1] + 1);  // get the option flag

    // Handle the help flag and exit normally
    if (opt == 'h') {
        usage(argv[0]);
        exit(0);
    }

    // Yeah, This checks if the user has provided enough arguments for the program 
    // to work. We know the first argument is the flag (like `-c`), and the 
    // second should be the string to process. If the string is missing, we 
    // call `usage()` to show them how to run the program (PROPERLY) and then exit.
    // Basically, it's making sure we don't try to use `argv[2]` if it's not 
    // there, which would cause problems.
    input_string = argv[2];  
    // Oh, here we allocate space for the buffer dynamically using `malloc`. 
    // We know the buffer size is `BUFFER_SZ`, so we ask for that much memory.
    
    buff = (char *)malloc(BUFFER_SZ * sizeof(char));
    if (buff == NULL) {
        printf("Memory allocation didnt work.\n");
        exit(99);
    }

    // Set up the buffer and ensure it was initialized correctly
    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);
    if (user_str_len <= 0) {
        printf("Failed to set up buffer\n");
        free(buff);
        exit(2);
    }

    // Process options based on user input
    switch (opt) {
        case 'c':
            rc = count_words(buff, user_str_len);  // count the words in the buffer
            if (rc < 0) {
                printf("Error counting words, rc = %d\n", rc);
                free(buff);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        case 'r':
            reverse_string(buff, user_str_len);
            printf("Reversed String: %s\n", buff);
            break;

        case 'w':
            word_print(buff, user_str_len);
            break;

        case 'x':
            if (argc == 5) {
                replace_substring(buff, BUFFER_SZ, argv[3], argv[4]);
                printf("Modified String: ");
                for (int i = 0; i < BUFFER_SZ && buff[i] != '.'; i++) {
                    printf("%c", buff[i]);
                }
                printf("\n");
            } else {
                capitalize_words(buff, user_str_len);
                printf("Capitalized String: %s\n", buff);
            }
            break;

        default:
            usage(argv[0]);
            free(buff);
            exit(1);
    }

    // Print the buffer contents and free memory before exiting
    print_buff(buff, BUFFER_SZ);
    free(buff);
    exit(0);
}

    // Yeahh, we provide both the pointer (`buff`) and the length to helper 
    // functions because it makes the code safer and more flexible. 
    // Even though we know `BUFFER_SZ` is always 50, this approach prevents 
    // bugs like buffer overflows if something changes. Also, the functions 
    // can be reused for buffers of different sizes in other programs, which 
    // is very good

    //Thanks.

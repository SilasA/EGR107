#include <stdio.h>
#include <string.h>
#include <ctype.h>

char _string[256];

int main()
{
    int alpha = 0;
    int digit = 0;
    int punct = 0;
    int words = 0;
    
    printf("Enter a String: ");
    scanf("%[^\n]%*c", _string);

    printf("\n\n");

    int i = 0;
    for (i = 0; i < strlen(_string); i++)
    {
        char ch = _string[i];
        if (isalpha(ch)) alpha++;
        else if (isdigit(ch)) digit++;
        else if (ispunct(ch)) punct++;
        else if (ch == ' ') words++;
    }


    printf("------------------------------------------------------------\n");
    printf(" String to Analyze: %s\n", _string);
    printf(" Length: %d\n", strlen(_string));
    printf(" Words: %d\n", words + 1);
    printf(" Letters: %d\n", alpha);
    printf(" Digit: %d\n", digit);
    printf(" Punctuation: %d\n", punct);
    printf("------------------------------------------------------------\n");

    return 0;
}


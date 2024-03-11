#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 309

struct bigInteger
{
    int size;
    int sign;
    char* value;
};

void print(struct bigInteger* data);
void swap(struct bigInteger* a, struct bigInteger* b);
void initialize(struct bigInteger* num, int n, int size);
struct bigInteger convertion(char s[SIZE + 2]);
int compare(struct bigInteger* number1, struct bigInteger* number2);
struct bigInteger addition(struct bigInteger* number1, struct bigInteger* number2);
struct bigInteger subtraction(struct bigInteger* number1, struct bigInteger* number2);
struct bigInteger multiplication(struct bigInteger* number1, struct bigInteger* number2);

void print(struct bigInteger* data)
{
    if(data->sign == 1)
        printf("-");
    for(int i = data->size - 1; i >= 0; i--)
        printf("%d", data->value[i]);
    printf("\n");
}

void swap(struct bigInteger* a, struct bigInteger* b)
{
    struct bigInteger temp = *a;
    *a = *b;
    *b = temp;
}

void initialize(struct bigInteger* num, int n, int size)
{
    num->size = size;
    num->value = (char*)malloc(n * sizeof(char));
    num->sign = 0;
}

struct bigInteger convertion(char s[SIZE + 2])
{
    struct bigInteger num;
    initialize(&num, SIZE, strlen(s));

    int i = 0, j = strlen(s) - 1;
    if(s[0] == '-')
    {
        num.size--;
        num.sign = 1;
    }
    else
    {
        num.sign = 0;
    }
    
    while(i < num.size)
    {
        num.value[i] = s[j] - '0'; 
        i++;
        j--;
    }
    return num;
}

int compare(struct bigInteger* number1, struct bigInteger* number2)
{
    int return_value = 0;
    
    if(number1->size > number2->size)
        return_value = 1;
    else if(number1->size < number2->size)
        return_value = -1;
    else
    {
        for(int i = number1->size - 1; i >= 0 && return_value == 0; i--)
        {
            if(number1->value[i] > number2->value[i])
            {
                return_value = 1;
            }
            if(number1->value[i] < number2->value[i])
            {
               return_value = -1;
            }
        }
        
    }
    return return_value;
}

struct bigInteger addition(struct bigInteger* number1, struct bigInteger* number2)
{    
    struct bigInteger answer;
    initialize(&answer, SIZE, 0);


    if(number1->sign != number2->sign)
    {
       number2->sign = !number2->sign;
        answer = subtraction(number1, number2);
        number2->sign = !number2->sign;
        return answer;
    }
    
    int i = 0, carry = 0;

    while(i < number1->size && i < number2->size)
    {
        int sum = number1->value[i] + number2->value[i] + carry;
        
        answer.value[i] = sum % 10;
        carry = sum / 10;
        
        answer.size++;
        i++;
    }

    while(i < number1->size)
    {
        int sum = number1->value[i] + carry;
        
        answer.value[i] = sum % 10;
        carry = sum / 10;
        
        answer.size++;
        i++;
    }


    while(i < number2->size)
    {
        int sum = number2->value[i] + carry;
        
        answer.value[i] = sum % 10;
        carry = sum / 10;
        
        answer.size++;
        i++;
    }

    if(carry != 0)
    {
        answer.value[i] = carry;
        answer.size++;
    }

    if(number1->sign == 1 && number2->sign == 1)
        answer.sign = 1;
    else
        answer.sign = 0;
    
    return answer;
}

struct bigInteger subtraction(struct bigInteger* number1, struct bigInteger* number2)
{
    struct bigInteger answer;
    initialize(&answer, SIZE, 0);
    
    if(number1->sign != number2->sign)
    {
        number2->sign = !number2->sign;
        answer = addition(number1, number2);
        number2->sign = !number2->sign;
        
        return answer;
    }
    else
    {
        if(number1->sign == 0 && number2->sign == 0)
        {
            if(compare(number1, number2) >= 0)
                answer.sign = 0;
            else
            {
                swap(number1, number2);
                answer.sign = 1;
            }
        }
        else
        {
            if(compare(number1, number2) >= 0)
            {
               
                answer.sign = 1;
            }
            else
            {
                answer.sign = 0;
                swap(number1, number2);
            }
        
        }
    }
    

    int i = 0, borrow = 0;
    while(i < number1->size && i < number2->size)
    {
        int difference = number1->value[i] - number2->value[i] - borrow;

        if(difference < 0)
        {
            difference += 10;
            borrow = 1;
        }
        else
            borrow = 0;

        answer.value[i] = difference;
        answer.size++;
        i++;
    }

    
    while(i < number1->size)
    {
        int difference = number1->value[i] - borrow;

        if(difference <= 0)
        {
            difference += 10;
            borrow = 1;
        }
        else
            borrow = 0;

        answer.value[i] = difference;
        answer.size++;
        i++;
    }
    

    if(borrow != 0)
    {
        answer.size--;
    }

    for(int i = answer.size - 1; i > 0; i--)
    {
        if(answer.value[i] == 0)
            answer.size--;
        else
            break;
    }
    
    
    return answer;
}

struct bigInteger multiplication(struct bigInteger* number1, struct bigInteger* number2)
{
    struct bigInteger answer;
    initialize(&answer, 2 * SIZE, 0);
    
    int carry = 0, c2 = 0;
    for(int i = 0; i < number2->size; i++)
    {
        int j = 0, k;
        while(j < number1->size)
        {
            int prod = number2->value[i] * number1->value[j] + carry;
            carry = prod / 10;
            k = j + c2;
            if(k < answer.size)
            {
                answer.value[k] += prod % 10;

                if(answer.value[k] > 9)
                {
                    if(k + 1 < answer.size)
                    {
                        answer.value[k + 1] += answer.value[k] / 10;
                    }
                    else
                    {
                        answer.value[k + 1] = answer.value[k] / 10;
                        answer.size++;
                    }
                    answer.value[k] %= 10;
                }
            }
            else
            {
                answer.value[k] = prod % 10;
                answer.size++;
            }
            j++;
        }
        
        k = j + c2;
        
        if(carry != 0)
        {
            if(k < answer.size)
            {
                answer.value[k] += carry;

                if(answer.value[k] > 9)
                {
                    if(k + 1 < answer.size)
                    {
                        answer.value[k + 1] += answer.value[k] / 10;
                    }
                    else
                    {
                        answer.value[k + 1] = answer.value[k] / 10;
                        answer.size++;
                    }
                }
                answer.value[k] %= 10;

            }
            else
            {
                answer.value[k] = carry;
                answer.size++;
            }
        }
        carry = 0;
        c2++;
    }

    if(number1->sign == number2->sign)
        answer.sign = 0;
    else
        answer.sign = 1;
    return answer;
}

int main(){

char s1[SIZE + 2] ,s2[SIZE+2];

printf("Enter the first number:\n");
scanf("%s",s1);

printf("Enter the second number:\n");
scanf("%s",s2);
     

struct bigInteger a = convertion(s1),
               b= convertion(s2),
              c;

c = addition(&a, &b);
printf("Addition of the numbers is :\n");
print(&c);
free(c.value);

c = subtraction(&a, &b);
printf("Subtraction of the numbers is :\n");
print(&c);
free(c.value);

c = multiplication(&a, &b);
printf("Multiplication of the numbers is :\n");
print(&c);
free(c.value);

return 0;
}
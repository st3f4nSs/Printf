#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int write_stdout(const char *token, int length) {
	int rc;
	int bytes_written;

	bytes_written = 0;
	do {
		rc = write(1, token + bytes_written, length - bytes_written);
		if (rc < 0)
			return rc;

		bytes_written += rc;
	} while (bytes_written < length);

	return bytes_written;
}

/*
 	- functie care returneaza caracterul asociat unei valori.
	- folosita in implementarea functiei hexa care transforma
 un numar din baza 10 in baza 16.
*/
char get_value(int number) {
    if(number == 10)
        return 'a';
    if(number == 11)
        return 'b';
    if(number == 12)
        return 'c';
    if(number == 13)
        return 'd';
    if(number == 14)
        return 'e';
    return 'f';
}

/*
	- detemina numarul de cifre dintr un numar
de tip int;
*/
int number_digit_int(int number) {
	int counter;

	counter = 0;
	while (number) {
		counter++;
		number = number / 10;
	}
	return counter;
}

/*
	- determina numarul de cifre dintr un numar
de tip unsigned;
*/
int number_digit_unsigned(unsigned int number) {
	int counter;

	counter = 0;
	while (number) {
		counter++;
		number = number / 10;
	}
	return counter;
}

/*
	- functie care determina inversul unui sir;
*/
void reverse(char *string) {
	int i, size;
	char change;

	size = strlen(string);
	for (i = 0; i < size / 2; i++) {
		change = string[i];
		string[i] = string[size - i - 1];
		string[size - i - 1] = change;
	}
}

/*
	- transforma un numar din baza 10 in baza 16;
	- impartim succesiv numarul la 16, si stocam
in vectorul hexadecimal resturile;
	- oglindim sirul de caractere pentru a obtine 
reprezentarea in baza 16.
*/
char *hexa(unsigned int number) {
    char *hexadecimal;
    int size;

    hexadecimal = (char *)malloc(30 * sizeof(char));
    size = 0;
    while (number) {
        unsigned int remainder, quotient;

        remainder = number % 16;
        quotient = number / 16;
        if (remainder <= 9)
            hexadecimal[size++] = remainder + '0';
        else
            hexadecimal[size++] = get_value(remainder);
        number = quotient;
    }
    hexadecimal[size] = '\0';
    reverse(hexadecimal);
    return hexadecimal;
}

/*
	- tinem cont daca numarul( number) este pozitiv sau negativ:
		a) daca numarul este negativ atunci adaugam in sir
	pe rand fiecare cifra si la final caracterul minus
	(obtinem inversul numarului);
		b) daca numarul este pozitiv atunci adaugam in sir
	cifra cu cifra (obtinem inversului numarului);
	- inversam sirul de caractere pentru a obtine numarul;
*/
char *construct_int(int number) {
	int digit_number;
    char *int_constr;
    int is_negative;

    is_negative = 0;
    digit_number = number_digit_int(number);
    int_constr = (char *)malloc((digit_number + 2) * sizeof(char));
    if (number < 0)
    	is_negative = 1;
    for (int j = 0; j < digit_number; j++) {
     		int_constr[j] = abs(number % 10) + '0';
        	number = number / 10;
    }
    if (is_negative)
    	int_constr[digit_number++] = '-';
    int_constr[digit_number] = '\0';
    reverse(int_constr);
    return int_constr;
}

/*
	- selectam fiecare cifra din numar si o inseram intr un 
sir de caractere (obtinem inversul numarului);
	- inversam sirul de caractere pentru a obtine numarul;
*/
char *construct_unsigned(unsigned int number) {
	unsigned int digit_number;
    char *unsigned_constr;

   	digit_number = number_digit_unsigned(number);
    unsigned_constr = (char *)malloc((digit_number + 2) * sizeof(char));

    for (int j = 0; j < digit_number; j++) {
        unsigned_constr[j] = number % 10 + '0';
        number = number / 10;
    }
    unsigned_constr[digit_number] = '\0';
    reverse(unsigned_constr);

    return unsigned_constr;	
}

/*
	- parcurgem format ul caracter cu caracter;
	- daca intalnim caracterul '%' avem 6 cazuri pentru caracterul care-i urmeaza:
		a) pentru 'd' construim un string cu int-ul dat ca parametru, apoi
	adaugam in stringul rezultat(result) caracter cu caracter; 
		b) pentru 'u' construim un string cu unsigned int ul dat ca parametru,
	apoi adaugam in stringul rezultat(result) caracter cu caracter; 
		c) pentru 's' adaugam in string-ul rezultat(result) caracter cu caracter string
	ul dat ca parametru;
		d) pentru 'c' adaugam la string-ul rezultat(result) caracterul dat ca
	parametru;
		e) pentru 'x' folosim functia hexa, pentru a converti int-ul dat ca
	parametru din baza 10 in baza 16 (functia returneaza un string), apoi
	adaugam in string-ul rezultat(result) caracter cu caracter;
		f) pentru '%' adaugam in string-ul rezultat(result) caracterul %;
	- daca nu intalnim caracterul '%', adaugam caracterul respectiv in sir;

*/
int my_printf(const char *format, ...)
{
	char *result;
	int dim_result, i, dim_format;
	va_list args;

	va_start(args, format);
	result = (char *)malloc(30000 * sizeof(char));
	dim_format = strlen(format);
	dim_result = 0;
	for (i = 0; i < dim_format; i++) {
	    if (format[i] == '%') {
            if (format[i + 1] == 'd') {
            	int value;
            	char *string_int;

            	value = va_arg(args, int);								
            	string_int = construct_int(value);						
                for(int j = 0; j < strlen(string_int); j++)				
                    result[dim_result++] = string_int[j];
                free(string_int);										
                i++;												

            } else if (format[i + 1] == 'c') {
                char character;

                character = va_arg(args, int);							
                result[dim_result++] = character;						
                i++;

            } else if (format[i + 1] == 'u') {
            	unsigned int value;
            	char *string_unsigned;

            	value = va_arg(args, unsigned int);						
                string_unsigned = construct_unsigned(value);
                for (int j = 0; j < strlen(string_unsigned); j++)
                    result[dim_result++] = string_unsigned[j];
                free(string_unsigned);
                i++;

            } else if (format[i + 1] == 'x') {
                char *string_hexa;
                int size, j;
                unsigned int value;										

                value = va_arg(args, int);								
                string_hexa = hexa(value);								
                size = strlen(string_hexa);								
                for (j = 0; j < size; j++)								
                    result[dim_result++] = string_hexa[j];
                free(string_hexa);										
                i++;

            } else if (format[i + 1] == 's') {
                const char *string = va_arg(args, const char *);		
                int size;

                size = strlen(string);
                for (int j = 0; j < size; j++)
                    result[dim_result++] = string[j];									
                i++;

            } else if (format[i + 1] == '%') {
                result[dim_result++] = format[i];						
                i++;
            } 

	    } else 
	    	result[dim_result++] = format[i];							
	}
	result[dim_result] = '\0';											
	write_stdout(result, dim_result);									
	va_end(args);
	free(result);
	return dim_result;												
}


int main(void) {
	my_printf("%d %s\n", 5, "Testing");
}

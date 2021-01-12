/* CIS 2520 Assignment 4
 * a4.c
 *
 * Bryn Deering
 * 1063540
 * bdeering@uoguelph.ca
 * November 17, 2019
 */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int hash1(char *string, int hash_size) {
  char *c;
  unsigned long number, column, new;
  
  column = 1;
  number = 0;
  
  for (c=string;(*c);c++)
  {
    /*-no discrimination between capital and lowercase letters,
       this means characters may exceed their column value,
       but increases dispersion because capital letters are counted differently
      -a number is added to each character so that 'a', 'aa' don't give the same results,
       9 gave the best experimental results
     -41 is used as the column value as it gives experimentally better results than 27 */
    number += (*c - 'A' + 9) * column;
    column *= 41;
  }
  
  new = 0;
  while (number)
  {
    new = ( number + (new % hash_size) ) % hash_size;
    number = number / hash_size;
  }
  
  return (int)new;
}

int hash2(char *string, int hash_size) {
  unsigned long number, char_number, char_column, new;
  long digs_column, digs_number;
  int c;
  
  char_column = 1;
  char_number = 0;
  digs_column = 1;
  digs_number = 0;
  
  /* digits and other characters sorted seperately */
  for (c = 0;c < strlen(string);c++) {
    if (isdigit(string[c])) {
      digs_number += (string[c] - '0' + 41) * digs_column;
      digs_column *= 10;
    }
    else {
      char_number += (string[c] - 'A' + 19) * char_column;
      char_column *= 41;
    }
  }
  
  /* Modified mid square hash function for the digits in license_no */
  digs_number *= digs_number;
  digs_number = digs_number / 10000;
  digs_number = digs_number % 1000000; /* extracting 6 digits */
  
  if (digs_number != 0) {
    number = char_number * digs_number;
  } else {
    number = char_number;
  }
  
  new = 0;
  while (number)
  {
    new = ( number + (new % hash_size) ) % hash_size;
    number = number / hash_size;
  }
  
  return (int)new;
}

int hash3(char *string, int hash_size){
  char *c;
  unsigned long number, column, new;
  
  column = 1;
  number = 0;
  
  for (c=string;(*c);c++) {
    number += (*c - '0' + 6) * column;
    column *= 41;
  }
  
  new = 0;
  while (number)
  {
    new = ( number + (new % hash_size) ) % hash_size;
    number = number / hash_size;
  }
  
  return (int)new;
}
